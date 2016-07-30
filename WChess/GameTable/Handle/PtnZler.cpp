#include "StdAfx.h"
#include "Pattern.h"
#include "PtnZler.h"
#include "GameTable.h"

static int			 ptnboard[65];

extern void			 SaveToFen(TCHAR *fen);
extern bool			 SaveToPgn(const TCHAR* filename);

CPtnZler::CPtnZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CPtnZler::~CPtnZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CPtnZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_PATTERN:
		OnMenuPattern();
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CPtnZler::OnMenuPattern()
{
	if (!IsBoardLegal())
	{
		window->MessageBox(L"���治�Ϸ�", L"���ξ�", MB_ICONWARNING | MB_OK);
		return;
	}
	if (window->MessageBox(L"ȷ���Ե�ǰ������д�����", L"���ξ�", MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
		return;

	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	//����PatternĿ¼
	TCHAR wcsPatternPath[_MAX_PATH];
	wsprintf(wcsPatternPath, L"%s\\Pattern", ModulePath);
	if (_waccess(wcsPatternPath, 0) == -1)
		_wmkdir(wcsPatternPath);
	_wchdir(wcsPatternPath);
	//�����ļ���
	time_t t = time(NULL);
	wcsftime(m_wcsTimeFolder, sizeof(m_wcsTimeFolder) / sizeof(TCHAR), L"%Y%m%d_%H%M%S", localtime(&t)); //Eg: 20130814_210517
	_wmkdir(m_wcsTimeFolder);
	//������ʼ�ļ���
	m_nFileNumber = 1;
	if (!SavePtnTitle(wcsPatternPath))
		return;

	InitSwapRemain();
	ProcessPattern(true);
}

void CPtnZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	//��ѯ�Ƿ�ȫ���˳�
	if (window->MessageBox(L"�Ƿ�ȫ���˳���", L"���ξ�", MB_OKCANCEL) == IDCANCEL)
	{
		memcpy(board, ptnboard, sizeof(board));
		ProcessPattern(true);
	}
	else
	{
		//ȫ���˳�
		SendViewInd_InitState();
		SendMenuInd_InitState();
		g_pGameTable->SetZlerBase(NULL);
	}
}

void CPtnZler::ProcessPattern(bool bWeaken)
{
	ChangeOffensive(bWeaken);
	memcpy(ptnboard, board, sizeof(board));

	//��ʼ�ƽ�
	SendViewInd_PtnState();
	SendMenuInd_PtnState();

	//��ʼ���ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = PTN_MAXDEPTH;
	tCrackParam.nFurther = 0;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_PATTERN_GUARD, 600 * 1000); //600��
}

void CPtnZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();

	TCrackResult tCrackResult = WChess_GetCrackResult();
	if (wParam == BLKLOSE)
	{
		//��¼����������ļ�
		if (tCrackResult.nRound >= PTN_MINROUND)
		{
			SaveToLog(wParam, tCrackResult.nRound);
			SaveToFile();
		}
		//��������
		ProcessPattern(true);
	}
	else
	{
		//��������
		ProcessPattern(false);
	}
}

void CPtnZler::OnCrackTout()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	//��ԭ����
	memcpy(board, ptnboard, sizeof(board));
	//������
	SaveToLog(TIMEOUT, 0);
	SaveToFile();
	//����ǿ���е�������
	ProcessPattern(true);
}

void CPtnZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
	window->KillTimer(TM_PATTERN_GUARD);
}

bool CPtnZler::SavePtnTitle(TCHAR *wcsPatternPath)
{
	//����־�ļ�
	TCHAR wcsLogFile[_MAX_PATH];
	wsprintf(wcsLogFile, L"%s\\Log.txt", wcsPatternPath);
	FILE *fp = _wfopen(wcsLogFile, L"a+, ccs=UTF-8");
	if (fp == NULL)
	{
		window->MessageBox(L"����־�ļ�ʧ��", L"���ξ�", MB_ICONWARNING | MB_OK);
		return false;
	}
	//д��ָ����ͱ���
	fputws(L"\n-- - ---- -----  -- - -- - -  -- -- --  - - -- - --  ----- ---- - --\n", fp);
	TCHAR wcsFolderInfo[256];
	wsprintf(wcsFolderInfo, L"Folder:  %s\n", m_wcsTimeFolder);
	fputws(wcsFolderInfo, fp);
	//д����ʼ����
	TCHAR wcsBuff[1024];
	SaveToFen(wcsBuff);
	fputws(wcsBuff, fp);
	fputws(L"r - - 0 1\n", fp);
	fclose(fp);
	return true;
}

void CPtnZler::SaveToLog(int nResult, int nRound)
{
	//���ļ�
	TCHAR wcsLogFile[_MAX_PATH];
	wsprintf(wcsLogFile, L"%s\\Pattern\\Log.txt", ModulePath);
	FILE *LogFile;
	if ((LogFile = _wfopen(wcsLogFile, L"a+, ccs=UTF-8")) == NULL)
		return;

	//д����
	TCHAR wcsResult[128], szIdent[7];
	switch (nResult)
	{
	case BLKLOSE:
		wsprintf(szIdent, L"%s", nRound >= 30 ? L"      " : nRound >= 25 ? L"   " : L"");
		wsprintf(wcsResult, L"%6d.pgn         %s%3d\n", m_nFileNumber, szIdent, nRound);
		break;
	case TIMEOUT:
		wsprintf(wcsResult, L"%6d.pgn         ��ʱ\n", m_nFileNumber);
		break;
	}
	fputws(wcsResult, LogFile);

	//�ر��ļ�
	fclose(LogFile);
}

void CPtnZler::SaveToFile()
{
	static TCHAR wcsFileName[_MAX_PATH];
	//д������ļ�
	wsprintf(wcsFileName, L"%s\\%d.pgn", m_wcsTimeFolder, m_nFileNumber++);
	SaveToPgn(wcsFileName);
}

void CPtnZler::SendViewInd_InitState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CPtnZler::SendViewInd_PtnState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CPtnZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CPtnZler::SendMenuInd_PtnState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_EXIT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
