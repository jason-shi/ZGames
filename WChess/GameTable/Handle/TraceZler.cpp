#include "StdAfx.h"
#include "Trace.h"
#include "TraceZler.h"
#include "ChessBoard.h"
#include "GameTable.h"

extern bool			 SaveToPgn(const TCHAR* filename);

CTraceZler::CTraceZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	//С���ƴ�����ʼ��
	m_nTraceTimes = 0;
	g_pCmdManage->AddCmdZler(this);
}

CTraceZler::~CTraceZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CTraceZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_TRACE:
		OnMenuTrace();
		break;
	case ID_APP_LIMITARYTRACE:
		OnMenuTrace(true);
		break;
	case ID_APP_NEXT:
		OnMenuNext();
		break;
	case ID_APP_LAST:
		OnMenuLast();
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CTraceZler::OnMenuTrace(bool bLimitary)
{
	if (!IsBlkLegal())
	{
		window->MessageBox(L"���治�Ϸ�\n�ڷ����ڽ���״̬", L"С����", MB_OK);
		return;
	}
	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	g_pChessBoard->ClearFixed();
	g_bLimitary = bLimitary;

	m_bToutFlag = false;
	m_nTraceTimes++;
	memcpy(trcboard, board, sizeof(board));

	m_nTraceStage = 0;
	GetTraceRound();
}

void CTraceZler::OnMenuNext()
{
	memcpy(board, trcboard, sizeof(board));
	m_nIdxVisist++;
	//��������
	UpdateBoard(&TrList1[TrList2[m_nIdxVisist].index]);
	UpdateBoard(&TrList2[m_nIdxVisist]);

	SendViewInd_BrowseState();
	SendMenuInd_BrowseState();
}

void CTraceZler::OnMenuLast()
{
	memcpy(board, trcboard, sizeof(board));
	m_nIdxVisist--;
	//��������
	UpdateBoard(&TrList1[TrList2[m_nIdxVisist].index]);
	UpdateBoard(&TrList2[m_nIdxVisist]);

	SendViewInd_BrowseState();
	SendMenuInd_BrowseState();
}

void CTraceZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();

	//��ԭ����
	memcpy(board, trcboard, sizeof(board));
	SendViewInd_InitState();
	SendMenuInd_InitState();
	g_pGameTable->SetZlerBase(NULL);

	window->MessageBox(L"С��������ֹ", L"С����", MB_ICONWARNING | MB_OK);
}

void CTraceZler::GetTraceRound()
{
	SendViewInd_CrackState();
	SendMenuInd_CrackState();

	//��ʼ���ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = 0;
	tCrackParam.nFurther = 0;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_TRACE_GUARD, 600 * 1000);
}

void CTraceZler::ProcessTrace(int nTraceRound)
{
	m_nIdxVisist = 0;
	m_nIdxBlk = 0;
	m_nIdxRed = 0;
	InitTraceList();

	m_nTraceRound = nTraceRound;
	GenTrList1();
	ProcessTrList1();
}

void CTraceZler::ProcessTrList1()
{
	m_nTraceStage = 1;
	if (FetchTrList1(m_nIdxBlk) == false)
	{
		GenTrList2();
		ProcessTrList2();
		return;
	}
	SendViewInd_CrackState();
	SendMenuInd_CrackState();

	//��ʼ���ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = (m_nTraceRound ? m_nTraceRound : 40) * 2;
	tCrackParam.nFurther = 0;
	tCrackParam.src = TrList1[m_nIdxBlk - 1].src;
	tCrackParam.dest = TrList1[m_nIdxBlk - 1].dest;
	WChess_InitThreadBFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_TRACE_GUARD, 600 * 1000);
}

void CTraceZler::ProcessTrList2()
{
	m_nTraceStage = 2;
	if (FetchTrList2(m_nIdxRed) == false)
	{
		ReorderResult();
		OutputResult();
		OnTraceEnd();
		return;
	}
	SendViewInd_CrackState();
	SendMenuInd_CrackState();

	//��ʼ���ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = (m_nTraceRound ? m_nTraceRound : 40) * 2;
	tCrackParam.nFurther = 0;
	tCrackParam.src = TrList2[m_nIdxRed - 1].src;
	tCrackParam.dest = TrList2[m_nIdxRed - 1].dest;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_TRACE_GUARD, 600 * 1000);
}

void CTraceZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();

	TCrackResult tCrackResult = WChess_GetCrackResult();
	switch (m_nTraceStage)
	{
	case 0:
		if (wParam == REDLOSE)
			ProcessTrace(0);
		else
			ProcessTrace(tCrackResult.nRound);
		break;
	case 1:
		TrList1[m_nIdxBlk - 1].flag = wParam != REDLOSE;
		ProcessTrList1();
		break;
	case 2:
		TrList2[m_nIdxRed - 1].flag = wParam == REDLOSE;
		ProcessTrList2();
		break;
	}
}

void CTraceZler::OnCrackTout()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	switch (m_nTraceStage)
	{
	case 0:
		ProcessTrace(0); //����С����
		break;
	case 1:
		m_bToutFlag = true; //���С���Ƴ�ʱ
		TrList1[m_nIdxBlk - 1].flag = false;
		ProcessTrList1();
		break;
	case 2:
		m_bToutFlag = true; //���С���Ƴ�ʱ
		TrList2[m_nIdxRed - 1].flag = false;
		ProcessTrList2();
		break;
	}
}

void CTraceZler::ReorderResult()
{
	int nCount = 0;
	for (int i = 0; i < TrCount2; i++)
	{
		if (TrList2[i].flag)
		{
			memcpy(&TrList2[nCount], &TrList2[i], sizeof(TTrace));
			nCount++;
		}
	}
	TrCount2 = nCount;
}

void CTraceZler::OutputResult()
{
	SaveToFile();
	//��������ִ��
	if (TrCount2 == 0)
	{
		SendViewInd_BrowseState();
		SendMenuInd_BrowseState();
	}
	else
	{
		memcpy(board, trcboard, sizeof(board));
		//��������
		UpdateBoard(&TrList1[TrList2[0].index]);
		UpdateBoard(&TrList2[0]);

		SendViewInd_BrowseState();
		SendMenuInd_BrowseState();
	}
}

void CTraceZler::OnTraceEnd()
{
	TCHAR szWarning[128] = { 0 };
	if (m_bToutFlag)
		wsprintf(szWarning, L"%s%s", szWarning, L"С���Ƴ�ʱ\n");
	if (m_nTraceRound == 0)
		wsprintf(szWarning, L"%s%s", szWarning, L"����С����\n");
	if (TrCount2 == 0)
		wsprintf(szWarning, L"%s%s", szWarning, L"С����û���·���\n");

	if (wcslen(szWarning) != 0)
		window->MessageBox(szWarning, L"С����", MB_ICONWARNING | MB_OK);

	g_pGameTable->SetZlerBase(NULL);
}

void CTraceZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
	window->KillTimer(TM_TRACE_GUARD);
}

void CTraceZler::SaveToFile()
{
	TCHAR wcsTraceFolder[_MAX_PATH];
	wsprintf(wcsTraceFolder, L"%s\\Trace", ModulePath);
	//����TraceĿ¼
	if (_wchdir(wcsTraceFolder) != 0)
	{
		_wmkdir(wcsTraceFolder);
		_wchdir(wcsTraceFolder);
	}
	//д������ļ�
	static TCHAR wcsFileName[_MAX_PATH];
	for (int i = 0; i < TrCount2; i++)
	{
		UpdateBoard(&TrList1[TrList2[i].index]);
		UpdateBoard(&TrList2[i]);
		wsprintf(wcsFileName, L"%d_%d.pgn", m_nTraceTimes, i);
		SaveToPgn(wcsFileName);
		//��ԭ����
		RestoreBoard(&TrList2[i]);
		RestoreBoard(&TrList1[TrList2[i].index]);
	}
}

void CTraceZler::SendViewInd_InitState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CTraceZler::SendViewInd_CrackState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CTraceZler::SendViewInd_BrowseState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CTraceZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tViewInd.bSyncBoard = true;

	g_tMenuInd.nMenuBitmap[0] = EN_MENU_INIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CTraceZler::SendMenuInd_CrackState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_EXIT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CTraceZler::SendMenuInd_BrowseState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;
	//�������������˵�
	if (m_nIdxVisist > 0)
		g_tMenuInd.nMenuBitmap[2] |= EN_MENU_LAST;
	if (m_nIdxVisist < TrCount2 - 1)
		g_tMenuInd.nMenuBitmap[2] |= EN_MENU_NEXT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
