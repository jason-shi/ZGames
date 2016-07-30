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
		window->MessageBox(L"���治�Ϸ�", L"������", MB_ICONWARNING | MB_OK);
		return;
	}
	vecRecord.clear();
	vecManual.clear();
	memcpy(orgboard, board, sizeof(board));

	g_pChessBoard->ClearFixed();
	ProcessCrack(!PSEUDO, !AGAIN);

#if 0 //ȡ��ע�����
#ifndef _DEBUG
	//һ�����У������ע������
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
	//�췽�ͺڷ�������һ��
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
		window->MessageBox(L"�췽ʤ", L"��ʾ", MB_OK);

		//�ڷ�����һ��
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
	//��ʼ���ƽ��̣߳���������ʱ��
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
	//ȡ�����һ���ŷ�
	TRecord tRecord = vecRecord.back();
	vecRecord.pop_back();

	//�������һ���ŷ�
	int nDestKind = board[tRecord.dest];
	int nSrcKind = tRecord.bian == 0 ? nDestKind : bRedUndo ? R_PAWN : B_PAWN;

	//������ɾ������ֹ�����
	g_pChessBoard->DelPiece(tRecord.dest);

	g_pChessBoard->AddPiece(tRecord.src, nSrcKind);
	g_pChessBoard->AddPiece(tRecord.dest, tRecord.death);

	//�������һ������
	vecManual.pop_back();
}

void CCrackZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();

	TCrackResult tCrackResult = WChess_GetCrackResult();
	if (wParam == BLKLOSE)
	{
		g_pGameTable->SetAction(BATTLE_ACTION);

		//ֻ��ʾʱ��ͻغ���
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
		//ֻ��ʾ��ظ���ֵ��ʱ��
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
		//α�ƽ�ʱ�����ݿ�ʹ�������䣬MoveTag����
		g_tViewInd.tLabelInd.nAstodeSize = WChess_GetAstodeSize();
		g_tViewInd.bShowMoveTag = true;
	}
	else
	{
		g_tViewInd.tLabelInd.nAstodeSize = 0;
		g_tViewInd.bShowMoveTag = false;
	}
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_BlackLose(int nSpendTime, int nRound)
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bShowManual = true;
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_SPENDTIME | SH_ROUND;
	g_tViewInd.tLabelInd.nSpendTime = nSpendTime;
	g_tViewInd.tLabelInd.nRound = nRound;
	//���ûغ�����ɫ
	g_tViewInd.tLabelInd.rgbColor = IsRoundSkip(nRound) ? BGRA(0, 0, 255, 255) : BGRA(0, 0, 0, 255);

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_RedLose(int nSpendTime)
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_LOSEDEGREE | SH_SPENDTIME;
	g_tViewInd.tLabelInd.nLoseDegree = 3;
	g_tViewInd.tLabelInd.nSpendTime = nSpendTime;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_AgainState()
{
	//ͬ�����̣������ƶ���ǣ��������ݿ⣬�����ƽ�״̬
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	g_tViewInd.bShowManual = true;
	//��ǩ��ʾ
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;
	g_tViewInd.tLabelInd.nAstodeSize = WChess_GetAstodeSize();

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CCrackZler::SendViewInd_UndoState()
{
	//�����ƶ���ǣ�ͬ�����̣����»غ���������������ǩ
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	g_tViewInd.bShowManual = true;
	//���������ƶ����
	TRecord tRecord = vecRecord.back();
	g_tViewInd.bShowMoveTag = true;
	g_tViewInd.bUpdateMoveTag = true;
	g_tViewInd.nMoveTagA = tRecord.src;
	g_tViewInd.nMoveTagB = tRecord.dest;
	//���˻غ���
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
