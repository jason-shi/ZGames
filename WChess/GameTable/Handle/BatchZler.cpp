#include "StdAfx.h"
#include "BatchZler.h"
#include "ChessBoard.h"
#include "GameTable.h"

#define BatchFurther	 16 //������Further��ȱ�Ĭ��ֵ����

extern bool			 LoadByPgn(const TCHAR* wcsFileName);

CBatchZler::CBatchZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CBatchZler::~CBatchZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CBatchZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_BATCH:
		OnMenuBatch();
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CBatchZler::OnMenuBatch()
{
	TCHAR wcsBatchPath[_MAX_PATH];
	wsprintf(wcsBatchPath, L"%s\\Batch", ModulePath);
	//����BatchĿ¼
	if (_wchdir(wcsBatchPath) != 0)
	{
		window->MessageBox(L"BatchĿ¼������", L"������", MB_ICONWARNING | MB_OK);
		return OnBatchOver();
	}
	if ((m_hFile = _wfindfirst(L"*.pgn", &m_tFindData)) == -1)
	{
		window->MessageBox(L"BatchĿ¼��û��pgn�ļ�", L"������", MB_ICONWARNING | MB_OK);
		return OnBatchOver();
	}
	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	g_pChessBoard->ClearFixed();
	CrackElement(m_tFindData.name);
}

void CBatchZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	SaveToLog(INTERRUPT);
	//��ѯ�Ƿ�ȫ���˳�
	if (window->MessageBox(L"�Ƿ�ȫ���˳���", L"������", MB_OKCANCEL) == IDOK)
	{
		//ȫ���˳�
		return OnBatchOver();
	}

	FetchElement();
}

void CBatchZler::FetchElement()
{
	if (_wfindnext(m_hFile, &m_tFindData) != 0
		|| m_tFindData.attrib & _A_SUBDIR)
	{
		//�ļ��������
		return OnBatchOver();
	}
	CrackElement(m_tFindData.name);
}

void CBatchZler::CrackElement(const TCHAR* wcsFileName)
{
	if (!LoadByPgn(wcsFileName))
	{
		//�ļ��������������ƽ�
		window->PostMessage(WM_CRACK_END, BADFILE);
		return;
	}
	//��ʼ�ƽ�
	SendViewInd_BatchState();
	SendMenuInd_CrackState();

	//�����ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = 0;
	tCrackParam.nFurther = BatchFurther;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_BATCH_GUARD, 600 * 1000);
}

void CBatchZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();
	//д��������������һ���ļ�
	SaveToLog(wParam);
	FetchElement();
}

void CBatchZler::OnCrackTout()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	//д��������������һ���ļ�
	SaveToLog(TIMEOUT);
	FetchElement();
}

void CBatchZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
	window->KillTimer(TM_BATCH_GUARD);
}

void CBatchZler::SaveToLog(int nResult)
{
	TCHAR wcsLogFile[_MAX_PATH];
	wsprintf(wcsLogFile, L"%s\\log.txt", ModulePath);
	//���ļ�����д����
	FILE *LogFile;
	LogFile = _wfopen(wcsLogFile, L"a+, ccs=UTF-8");

	TCHAR wcsResult[128];
	switch (nResult)
	{
	case REDLOSE:
		wsprintf(wcsResult, L"%-40s  �츺\n", m_tFindData.name);
		break;
	case BLKLOSE:
		wsprintf(wcsResult, L"%-40s  %d\n", m_tFindData.name, WChess_GetThinkDepth() / 2);
		break;
	case TIMEOUT:
		wsprintf(wcsResult, L"%-40s  ********��ʱ********\n", m_tFindData.name);
		break;
	case INTERRUPT:
		wsprintf(wcsResult, L"%-40s  ********�ж�********\n", m_tFindData.name);
		break;
	case BADFILE:
		wsprintf(wcsResult, L"%-40s  ******�ļ�����******\n", m_tFindData.name);
		break;
	}
	fputws(wcsResult, LogFile);
	fclose(LogFile);
}

void CBatchZler::OnBatchOver()
{
	//����Ŀ¼
	_wchdir(ModulePath);

	SendViewInd_InitState();
	SendMenuInd_InitState();
	g_pGameTable->SetZlerBase(NULL);
}

void CBatchZler::SendViewInd_InitState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CBatchZler::SendViewInd_BatchState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CBatchZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_INIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CBatchZler::SendMenuInd_CrackState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_EXIT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
