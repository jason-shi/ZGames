#include "StdAfx.h"
#include "Record.h"
#include "Manual.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "ChessBox.h"
#include "ChessManual.h"
#include "GameTable.h"
#include "Action\BattleAction.h"
#include "Action\ArrangeAction.h"

#include "Handle\AccessZler.h"
#include "Handle\CrackZler.h"
#include "Handle\ArrangeZler.h"
#include "Handle\BatchZler.h"
#include "Handle\PtnZler.h"
#include "Handle\ManualZler.h"
#include "Handle\TraceZler.h"
#include "Handle\SourceZler.h"
#include "Handle\RegZler.h"

//命令处理者
CAccessZler			 g_tAccessZler(g_pGameTable);
CCrackZler			 g_tCrackZler(g_pGameTable);
CArrangeZler		 g_tArrangeZler(g_pGameTable);
CBatchZler			 g_tBatchZler(g_pGameTable);
CPtnZler			 g_tPtnZler(g_pGameTable);
CManualZler			 g_tManualZler(g_pGameTable);
CTraceZler			 g_tTraceZler(g_pGameTable);
CSourceZler			 g_tSourceZler(g_pGameTable);
CRegZler			 g_tRegZler(g_pGameTable);

CGameTable::CGameTable() : CMainWin()
{
	m_pLblThinkDepth = NULL;
	m_pLblAstodeSize = NULL;
	m_pLblSpendTime = NULL;
	m_pLblLoseDegree = NULL;
	m_pLblRound = NULL;

	m_pZlerBase = NULL;
	m_pAction = NULL;
}

CGameTable::~CGameTable()
{
}

bool CGameTable::Initial()
{
	g_pZ2coo->SetWindow(this);
	if (!g_pZ2coo->Initial())
		return false;

	g_pZ2mov->SetWindow(this);
	if (!g_pZ2mov->Initial())
		return false;

	g_pZTimer->SetWindow(this);
	if (!g_pZTimer->Initial())
		return false;

	//加载视类资源
	if (!LoadImage())
		return false;

	//创建棋盘
	if (!g_pChessBoard->Initial())
		return false;
	g_pChessBoard->Set(NULL, CPoint(42, 33), CPoint(0, 0), CSize(306, 306), 0);
	g_pZ2coo->AddObject(g_pChessBoard);

	//创建棋盒
	if (!g_pChessBox->Initial())
		return false;
	g_pChessBox->Set(NULL, CPoint(400, 80), CPoint(0, 0), CSize(75, 231), 0);
	g_pZ2coo->AddObject(g_pChessBox);

	//创建棋谱
	if (!g_pChessManual->Initial())
		return false;
	g_pChessManual->Set(NULL, CPoint(380, 100), CPoint(0, 0), CSize(200, 300), 0);
	g_pZ2coo->AddObject(g_pChessManual);

	//思考深度
	m_pLblThinkDepth = new CTextBlk(false, 18, CPoint(40, 420), CSize(140, 18), 0);
	if (!m_pLblThinkDepth || !m_pLblThinkDepth->Initial())
		return false;
	m_pLblThinkDepth->ClearImage();
	g_pZ2coo->AddObject(m_pLblThinkDepth);

	//数据库使用量
	m_pLblAstodeSize = new CTextBlk(false, 18, CPoint(180, 420), CSize(220, 18), 0);
	if (!m_pLblAstodeSize || !m_pLblAstodeSize->Initial())
		return false;
	m_pLblAstodeSize->ClearImage();
	g_pZ2coo->AddObject(m_pLblAstodeSize);

	//用时
	m_pLblSpendTime = new CTextBlk(false, 18, CPoint(400, 420), CSize(240, 18), 0);
	if (!m_pLblSpendTime || !m_pLblSpendTime->Initial())
		return false;
	m_pLblSpendTime->ClearImage();
	g_pZ2coo->AddObject(m_pLblSpendTime);

	//红必负阶值
	m_pLblLoseDegree = new CTextBlk(false, 18, CPoint(100, 420), CSize(200, 18), 0);
	if (!m_pLblLoseDegree || !m_pLblLoseDegree->Initial())
		return false;
	m_pLblLoseDegree->ClearImage();
	g_pZ2coo->AddObject(m_pLblLoseDegree);

	//回合数
	m_pLblRound = new CTextBlk(false, 60, CPoint(380, 30), CSize(260, 120), 0);
	if (!m_pLblRound || !m_pLblRound->Initial())
		return false;
	m_pLblRound->ClearImage();
	g_pZ2coo->AddObject(m_pLblRound);
	return true;
}

bool CGameTable::Release()
{
	delete m_pLblThinkDepth;
	m_pLblThinkDepth = NULL;

	delete m_pLblAstodeSize;
	m_pLblAstodeSize = NULL;

	delete m_pLblSpendTime;
	m_pLblSpendTime = NULL;

	delete m_pLblLoseDegree;
	m_pLblLoseDegree = NULL;

	delete m_pLblRound;
	m_pLblRound = NULL;

	g_pZ2coo->Release();
	g_pZ2mov->Release();
	g_pZTimer->Release();

	WChess_Release();

	g_pChessBoard->Release();
	return true;
}

LRESULT	CGameTable::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
		OnTimer(wParam);
		break;

	case WM_ANIME_END:
		g_pZ2coo->DelObject((CObject *)wParam);
		break;

	case WM_CRACK_END:
		WChess_WaitForThreadCrack();
		m_pZlerBase->OnCrackEnd(wParam, lParam);
		break;

	case WM_MOVE_PIECE:
		{
			TBlkMove *pBlkMove = (TBlkMove *)wParam;
			SaveRecord(pBlkMove->nSrcGrid, pBlkMove->nDestGrid, pBlkMove->nBian, WChess_GetThinkDepth() / 2);
			SaveManual(pBlkMove->nSrcGrid, pBlkMove->nDestGrid, pBlkMove->nBian);
			g_pChessManual->Update();
			g_pChessBoard->MovePiece(pBlkMove->nSrcGrid, pBlkMove->nDestGrid, pBlkMove->nBian);
		}
		break;

	case WM_PSEUDO_CRACK:
		g_tCrackZler.PseudoCrack();
		break;

	case WM_VIEW_IND:
		RecvViewInd(wParam, lParam);
		break;

	default:
		return CMainWin::WindowProc(uMsg, wParam, lParam);
	}
	return 0L;
}

void CGameTable::OnCommand(UINT notifyCode, UINT id, HWND ctrl)
{
	//控件消息不处理
	if (ctrl != NULL)
		return;

	//退出特殊处理
	if (id == ID_APP_EXIT)
	{
		m_pZlerBase ? m_pZlerBase->OnMenuExit() : SendMessage(WM_CLOSE);
		return;
	}
	//交由命令链处理
	if (g_pCmdManage->ProcessCmd(id))
		return;
}

bool CGameTable::OnFirstAction()
{
	if (!CMainWin::OnFirstAction())
		return false;

	//设定基准路径
	GetCurrentDirectory(_MAX_PATH, ModulePath);

	CreateAllTable();

	WChess_SetWindow(this);
	WChess_SetMsgId(WM_CRACK_END);

#ifndef _DEBUG
	//设置随机种子
	srand((int)time(NULL));
#endif
	return true;
}

void CGameTable::OnClose()
{
	WChess_ExitThreadCrack();

	return CMainWin::OnClose();
}

void CGameTable::OnTimer(WPARAM wParam)
{
	switch (wParam)
	{
	case TM_UPDATE_VIEW:
		UpdateThinkDepth(WChess_GetThinkDepth());
		UpdateAstodeSize(WChess_GetAstodeSize());
		UpdateSpendTime(WChess_GetSpendTime());
		break;
	case TM_BATCH_GUARD:
	case TM_PATTERN_GUARD:
	case TM_TRACE_GUARD:
	case TM_SOURCE_GUARD:
		m_pZlerBase->OnCrackTout();
		break;
	}
}

void CGameTable::ShowChessBox(bool bShow)
{
	g_pChessBox->Show(bShow);
}

void CGameTable::ShowChessManual(bool bShow)
{
	g_pChessManual->Show(bShow);
}

void CGameTable::RecvViewInd(WPARAM wParam, LPARAM lParam)
{
	TViewInd *pViewInd = (TViewInd *)lParam;
	TLabelInd &tLabelInd = pViewInd->tLabelInd;

	//显示/隐藏棋盒
	ShowChessBox(pViewInd->bShowBox);
	if (pViewInd->bShowBox)
		g_pChessBox->SetAvailPiece();
	//显示/隐藏棋谱
	ShowChessManual(pViewInd->bShowManual);
	if (pViewInd->bShowManual)
		g_pChessManual->Update();
	//显示/隐藏棋子移动标记，更新
	if (!pViewInd->bShowMoveTag)
		g_pChessBoard->HideMoveTag();
	else
	{
		if (pViewInd->bUpdateMoveTag)
			g_pChessBoard->ShowMoveTag(pViewInd->nMoveTagA, pViewInd->nMoveTagB);
	}
	//按位图显示/隐藏标签
	ShowLabel(tLabelInd.nLabelBitmap);
	//更新各标签
	UpdateThinkDepth(tLabelInd.nThinkDepth);
	UpdateAstodeSize(tLabelInd.nAstodeSize);
	UpdateSpendTime(tLabelInd.nSpendTime);
	UpdateLoseDegree(tLabelInd.nLoseDegree);
	UpdateRound(tLabelInd.nRound, tLabelInd.rgbColor);
	//更新ChessBoard
	if (pViewInd->bSyncBoard)
		g_pChessBoard->FastSyncBoard();
}

void CGameTable::SetAction(int nAction)
{
	static CArrangeAction		tArrangeAction;
	static CBattleAction		tBattleAction;

	//设置行为指针
	switch (nAction)
	{
	case NO_ACTION:
		m_pAction = NULL;
		break;
	case BATTLE_ACTION:
		m_pAction = (CAction *)&tBattleAction;
		break;
	case ARRANGE_ACTION:
		m_pAction = (CAction *)&tArrangeAction;
		break;
	}
}

void CGameTable::UpdateThinkDepth(int nDepth)
{
	static HFONT hFont = CreateFont(18, 18, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	TCHAR wcsBuff[64];
	wsprintf(wcsBuff, L"思考深度  %d", nDepth / 2); //转为回合

	m_pLblThinkDepth->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, BGRA(0, 0, 0, 255));
}

void CGameTable::UpdateAstodeSize(int nSize)
{
	static HFONT hFont = CreateFont(18, 18, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	TCHAR wcsBuff[64];
	wsprintf(wcsBuff, L"数据库使用量  %d", nSize);

	m_pLblAstodeSize->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, BGRA(0, 0, 0, 255));
}

void CGameTable::UpdateSpendTime(int nTime)
{
	static HFONT hFont = CreateFont(18, 18, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	TCHAR wcsBuff[64];
	int nTotalSec = nTime / 1000;
	int nHour = nTotalSec / 3600;
	int nMinute = nTotalSec / 60 % 60;
	int nSecond = nTotalSec % 60;
	int nMiliSec = nTime % 1000;
	wsprintf(wcsBuff, L"用时  %02d:%02d:%02d.%03d", nHour, nMinute, nSecond, nMiliSec);

	m_pLblSpendTime->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, BGRA(0, 0, 0, 255));
}

void CGameTable::UpdateLoseDegree(int nDegree)
{
	static HFONT hFont = CreateFont(18, 18, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	TCHAR wcsBuff[64];
	wsprintf (wcsBuff, L"红方无胜着");

	m_pLblLoseDegree->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, BGRA(0, 0, 255, 255));
}

void CGameTable::UpdateRound(int nRound, COLORREF rgbColor)
{
	static HFONT hFont = CreateFont(60, 120, 0, 0, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	TCHAR wcsBuff[64];
	wsprintf(wcsBuff, L"%d", nRound);

	m_pLblRound->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, rgbColor);
}

void CGameTable::ShowLabel(int nLabelBitmap)
{
	m_pLblThinkDepth->Show(nLabelBitmap & SH_THINKDEPTH);
	m_pLblAstodeSize->Show(nLabelBitmap & SH_ASTODESIZE);
	m_pLblSpendTime->Show(nLabelBitmap & SH_SPENDTIME);
	m_pLblLoseDegree->Show(nLabelBitmap & SH_LOSEDEGREE);
	m_pLblRound->Show(nLabelBitmap & SH_ROUND);
}
