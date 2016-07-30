#include "StdAfx.h"
#include "Record.h"
#include "Manual.h"
#include "CrackZler.h"
#include "ChessBoard.h"
#include "GameTable.h"
#include "Action\Action.h"
#include "Handle\RegZler.h"

static int			 orgboard[65];

CCrackZler::CCrackZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CCrackZler::~CCrackZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CCrackZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_RUN:
		OnMenuRun();
		break;
	case ID_APP_AGAIN:
		OnMenuAgain();
		break;
	case ID_APP_UNDO:
		OnMenuUndo();
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CCrackZler::OnMenuRun()
{
	if (!IsBothLegal())
	{
		window->MessageBox(L"盘面不合法", L"盘面检查", MB_ICONWARNING | MB_OK);
		return;
	}
	vecRecord.clear();
	vecManual.clear();
	memcpy(orgboard, board, sizeof(board));

	g_pChessBoard->ClearFixed();
	ProcessCrack(!PSEUDO, !AGAIN);

#if 0 //取消注册机制
#ifndef _DEBUG
	//一旦运行，则进入注册陷阱
	CRegZler::TrapRegister();
#endif
#endif
}

void CCrackZler::OnMenuAgain()
{
	vecRecord.clear();
	vecManual.clear();
	memcpy(board, orgboard, sizeof(board));

	ProcessCrack(PSEUDO, AGAIN);
}

void CCrackZler::OnMenuUndo()
{
	//红方和黑方各回退一步
	ProcessUndo(true);
	ProcessUndo(false);
	g_pGameTable->SetAction(BATTLE_ACTION);

	SendViewInd_UndoState();
	SendMenuInd_UndoState();
}

void CCrackZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();

	SendViewInd_InitState();
	SendMenuInd_InitState();

	g_pGameTable->SetZlerBase(NULL);
	g_pGameTable->SetAction(0);
}

void CCrackZler::PseudoCrack()
{
	if (InCheckBlk())
	{
		window->MessageBox(L"红方胜", L"提示", MB_OK);

		//黑方回退一步
		ProcessUndo(false);
		g_pGameTable->SetAction(BATTLE_ACTION);

		SendViewInd_UndoState();
		SendMenuInd_UndoState();
		return;
	}
	ProcessCrack(PSEUDO, !AGAIN);
}

void CCrackZler::ProcessCrack(bool bPesudo, bool bAgain)
{
	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	if (bAgain)
	{
		SendViewInd_AgainState();
		SendMenuInd_CrackState();
	}
	else
	{
		SendViewInd_CrackState(bPesudo);
		SendMenuInd_CrackState();
	}
	//初始化破解线程，并启动定时器
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = bPesudo;
	tCrackParam.nMaxDepth = 0;
	tCrackParam.nFurther = 0;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
}

void CCrackZler::ProcessUndo(bool bRedUndo)
{
	//取得最后一步着法
	TRecord tRecord = vecRecord.back();
	vecRecord.pop_back();

	//回退最后一步着法
	int nDestKind = board[tRecord.dest];
	int nSrcKind = tRecord.bian == 0 ? nDestKind : bRedUndo ? R_PAWN : B_PAWN;

	//必须先删除，防止池溢出
	g_pChessBoard->DelPiece(tRecord.dest);

	g_pChessBoard->AddPiece(tRecord.src, nSrcKind);
	g_pChessBoard->AddPiece(tRecord.dest, tRecord.death);

	//回退最后一步棋谱
	vecManual.pop_back();
}

void CCrackZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();

	TCrackResult tCrackResult = WChess_GetCrackResult();
	if (wParam == BLKLOSE)
	{
		g_pGameTable->SetAction(BATTLE_ACTION);

		//只显示时间和回合数
		SendViewInd_BlackLose(tCrackResult.nSpendTime, tCrackResult.nRound);
		SendMenuInd_UndoState();

		memset(&g_tBlkMove, 0, sizeof(g_tBlkMove));
		g_tBlkMove.nSrcGrid = tCrackResult.src;
		g_tBlkMove.nDestGrid = tCrackResult.dest;
		g_tBlkMove.nBian = tCrackResult.bian;

		window->SendMessage(WM_MOVE_PIECE, (WPARAM)&g_tBlkMove);
	}
	else
	{
		//只显示红必负阶值和时间
		SendViewInd_RedLose(tCrackResult.nSpendTime);
		SendMenuInd_InitState();
	}
}

void CCrackZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
}

void CCrackZler::SendViewInd_InitState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_CrackState(bool bPseudo)
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bShowManual = true;
	if (bPseudo)
	{
		//伪破解时，数据库使用量不变，MoveTag不变
		g_tViewInd.tLabelInd.nAstodeSize = WChess_GetAstodeSize();
		g_tViewInd.bShowMoveTag = true;
	}
	else
	{
		g_tViewInd.tLabelInd.nAstodeSize = 0;
		g_tViewInd.bShowMoveTag = false;
	}
	//标签显示
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_BlackLose(int nSpendTime, int nRound)
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bShowManual = true;
	//标签显示
	g_tViewInd.tLabelInd.nLabelBitmap = SH_SPENDTIME | SH_ROUND;
	g_tViewInd.tLabelInd.nSpendTime = nSpendTime;
	g_tViewInd.tLabelInd.nRound = nRound;
	//设置回合数颜色
	g_tViewInd.tLabelInd.rgbColor = IsRoundSkip(nRound) ? BGRA(0, 0, 255, 255) : BGRA(0, 0, 0, 255);

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_RedLose(int nSpendTime)
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	//标签显示
	g_tViewInd.tLabelInd.nLabelBitmap = SH_LOSEDEGREE | SH_SPENDTIME;
	g_tViewInd.tLabelInd.nLoseDegree = 3;
	g_tViewInd.tLabelInd.nSpendTime = nSpendTime;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_AgainState()
{
	//同步棋盘，隐藏移动标记，不清数据库，进入破解状态
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	g_tViewInd.bShowManual = true;
	//标签显示
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;
	g_tViewInd.tLabelInd.nAstodeSize = WChess_GetAstodeSize();

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_UndoState()
{
	//更新移动标记，同步棋盘，更新回合数，隐藏其他标签
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	g_tViewInd.bShowManual = true;
	//回退棋子移动标记
	TRecord tRecord = vecRecord.back();
	g_tViewInd.bShowMoveTag = true;
	g_tViewInd.bUpdateMoveTag = true;
	g_tViewInd.nMoveTagA = tRecord.src;
	g_tViewInd.nMoveTagB = tRecord.dest;
	//回退回合数
	g_tViewInd.tLabelInd.nLabelBitmap = SH_ROUND;
	g_tViewInd.tLabelInd.nRound = tRecord.nRest;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CCrackZler::SendMenuInd_CrackState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_EXIT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CCrackZler::SendMenuInd_UndoState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_AGAIN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_MANUAL | EN_MENU_EXIT;
	if (vecRecord.size() > 1)
		g_tMenuInd.nMenuBitmap[0] |= EN_MENU_UNDO;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
