#include "StdAfx.h"
#include "BattleAction.h"
#include "Battle.h"
#include "ChessPiece.h"
#include "ChessBox.h"
#include "ChessBoard.h"
#include "GameTable.h"
#include "BianChoice.h"

CBattleAction::CBattleAction() : CAction()
{
}

CBattleAction::~CBattleAction()
{
}

void CBattleAction::BlackMove(int nSrcGrid, int nDestGrid, int nKind, int nBian)
{
	if (CheckBlackMove(nSrcGrid, nDestGrid, nKind, nBian))
	{
		memset(&g_tBlkMove, 0, sizeof(g_tBlkMove));
		g_tBlkMove.nSrcGrid = nSrcGrid;
		g_tBlkMove.nDestGrid = nDestGrid;
		g_tBlkMove.nBian = nBian;

		g_pGameTable->SendMessage(WM_MOVE_PIECE, (WPARAM)&g_tBlkMove);
		g_pGameTable->SendMessage(WM_PSEUDO_CRACK);
	}
}

void CBattleAction::ChoicePiece(WChessPiece *pPiece)
{
	if (pPiece == m_pSelected)
	{
		//取消选择
		UnSelect();
	}
	else if (pPiece->GetKind() >= B_ROOK)
	{
		//切换选择
		UnSelect();
		Select(pPiece);
	}
	else
	{
		if (m_pSelected)
		{
			//玩家走棋
			WChessPiece *pSelected = m_pSelected;
			UnSelect();

			int nBian = 0;
			if (board[pSelected->GetGrid()] == B_PAWN && pPiece->GetGrid() < 8)
			{
				//创建模态选择框并调整位置
				CBianChoice tBianChoice(&ImagePiece, g_pZ2coo->GetLDown(), CPoint(252, 0));
				if (!tBianChoice.Initial())
					return;
				CRect rect;
				g_pGameTable->GetClientRect(&rect);
				tBianChoice.AdjustAnchor(g_pZ2coo->GetLDown(), rect);

				//弹出模态选择框
				nBian = tBianChoice.DoModal();
			}

			BlackMove(pSelected->GetGrid(), pPiece->GetGrid(), pSelected->GetKind(), nBian);
		}
	}
}

void CBattleAction::ChoiceBoard(int nGrid)
{
	if (m_pSelected == NULL)
		return;

	if (nGrid == INVALID_GRID)
		return UnSelect();

	//玩家走棋
	WChessPiece *pSelected = m_pSelected;
	UnSelect();

	int nBian = 0;
	if (CheckBlackBian(pSelected->GetGrid(), nGrid, pSelected->GetKind()))
	{
		//创建模态选择框并调整位置
		CBianChoice tBianChoice(&ImagePiece, g_pZ2coo->GetLDown(), CPoint(252, 0));
		if (!tBianChoice.Initial())
			return;
		CRect rect;
		g_pGameTable->GetClientRect(&rect);
		tBianChoice.AdjustAnchor(g_pZ2coo->GetLDown(), rect);

		//弹出模态选择框
		nBian = tBianChoice.DoModal();
	}

	BlackMove(pSelected->GetGrid(), nGrid, pSelected->GetKind(), nBian);
}
