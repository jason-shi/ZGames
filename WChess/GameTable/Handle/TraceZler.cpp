#include "StdAfx.h"
#include "Trace.h"
#include "TraceZler.h"
#include "ChessBoard.h"
#include "GameTable.h"

extern bool			 SaveToPgn(const TCHAR* filename);

CTraceZler::CTraceZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	//小倒推次数初始化
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
		window->MessageBox(L"盘面不合法\n黑方处于将军状态", L"小倒推", MB_OK);
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
	//更新盘面
	UpdateBoard(&TrList1[TrList2[m_nIdxVisist].index]);
	UpdateBoard(&TrList2[m_nIdxVisist]);

	SendViewInd_BrowseState();
	SendMenuInd_BrowseState();
}

void CTraceZler::OnMenuLast()
{
	memcpy(board, trcboard, sizeof(board));
	m_nIdxVisist--;
	//更新盘面
	UpdateBoard(&TrList1[TrList2[m_nIdxVisist].index]);
	UpdateBoard(&TrList2[m_nIdxVisist]);

	SendViewInd_BrowseState();
	SendMenuInd_BrowseState();
}

void CTraceZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();

	//还原盘面
	memcpy(board, trcboard, sizeof(board));
	SendViewInd_InitState();
	SendMenuInd_InitState();
	g_pGameTable->SetZlerBase(NULL);

	window->MessageBox(L"小倒推已终止", L"小倒推", MB_ICONWARNING | MB_OK);
}

void CTraceZler::GetTraceRound()
{
	SendViewInd_CrackState();
	SendMenuInd_CrackState();

	//初始化破解线程，并启动定时器
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

	//初始化破解线程，并启动定时器
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

	//初始化破解线程，并启动定时器
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
		ProcessTrace(0); //荒唐小倒推
		break;
	case 1:
		m_bToutFlag = true; //标记小倒推超时
		TrList1[m_nIdxBlk - 1].flag = false;
		ProcessTrList1();
		break;
	case 2:
		m_bToutFlag = true; //标记小倒推超时
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
	//输出结果后执行
	if (TrCount2 == 0)
	{
		SendViewInd_BrowseState();
		SendMenuInd_BrowseState();
	}
	else
	{
		memcpy(board, trcboard, sizeof(board));
		//更新盘面
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
		wsprintf(szWarning, L"%s%s", szWarning, L"小倒推超时\n");
	if (m_nTraceRound == 0)
		wsprintf(szWarning, L"%s%s", szWarning, L"荒唐小倒推\n");
	if (TrCount2 == 0)
		wsprintf(szWarning, L"%s%s", szWarning, L"小倒推没有新发现\n");

	if (wcslen(szWarning) != 0)
		window->MessageBox(szWarning, L"小倒推", MB_ICONWARNING | MB_OK);

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
	//进入Trace目录
	if (_wchdir(wcsTraceFolder) != 0)
	{
		_wmkdir(wcsTraceFolder);
		_wchdir(wcsTraceFolder);
	}
	//写入序号文件
	static TCHAR wcsFileName[_MAX_PATH];
	for (int i = 0; i < TrCount2; i++)
	{
		UpdateBoard(&TrList1[TrList2[i].index]);
		UpdateBoard(&TrList2[i]);
		wsprintf(wcsFileName, L"%d_%d.pgn", m_nTraceTimes, i);
		SaveToPgn(wcsFileName);
		//还原盘面
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
	//标签显示
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
	//设置浏览与回览菜单
	if (m_nIdxVisist > 0)
		g_tMenuInd.nMenuBitmap[2] |= EN_MENU_LAST;
	if (m_nIdxVisist < TrCount2 - 1)
		g_tMenuInd.nMenuBitmap[2] |= EN_MENU_NEXT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
