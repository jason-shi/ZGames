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
		//ȡ��ѡ��
		UnSelect();
	}
	else if (pPiece->GetKind() >= B_ROOK)
	{
		//�л�ѡ��
		UnSelect();
		Select(pPiece);
	}
	else
	{
		if (m_pSelected)
		{
			//�������
			WChessPiece *pSelected = m_pSelected;
			UnSelect();

			int nBian = 0;
			if (board[pSelected->GetGrid()] == B_PAWN && pPiece->GetGrid() < 8)
			{
				//����ģ̬ѡ��򲢵���λ��
				CBianChoice tBianChoice(&ImagePiece, g_pZ2coo->GetLDown(), CPoint(252, 0));
				if (!tBianChoice.Initial())
					return;
				CRect rect;
				g_pGameTable->GetClientRect(&rect);
				tBianChoice.AdjustAnchor(g_pZ2coo->GetLDown(), rect);

				//����ģ̬ѡ���
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

	//�������
	WChessPiece *pSelected = m_pSelected;
	UnSelect();

	int nBian = 0;
	if (CheckBlackBian(pSelected->GetGrid(), nGrid, pSelected->GetKind()))
	{
		//����ģ̬ѡ��򲢵���λ��
		CBianChoice tBianChoice(&ImagePiece, g_pZ2coo->GetLDown(), CPoint(252, 0));
		if (!tBianChoice.Initial())
			return;
		CRect rect;
		g_pGameTable->GetClientRect(&rect);
		tBianChoice.AdjustAnchor(g_pZ2coo->GetLDown(), rect);

		//����ģ̬ѡ���
		nBian = tBianChoice.DoModal();
	}

	BlackMove(pSelected->GetGrid(), nGrid, pSelected->GetKind(), nBian);
}
