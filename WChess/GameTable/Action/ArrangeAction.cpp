#include "StdAfx.h"
#include "ArrangeAction.h"
#include "ChessPiece.h"
#include "ChessBox.h"
#include "ChessBoard.h"

CArrangeAction::CArrangeAction() : CAction()
{
}

CArrangeAction::~CArrangeAction()
{
}

void CArrangeAction::ArrangePiece(int nSrcGrid, int nDestGrid, int nKind)
{
	int nLostKind = board[nDestGrid];

	//取消选择
	WChessPiece *pPiece = m_pSelected;
	UnSelect();

	//删除棋子
	if (pPiece->m_pHome == g_pChessBoard)
	{
		g_pChessBoard->DelPiece(nSrcGrid);
		g_pChessBoard->ClearFixed(nSrcGrid);
	}
	if (pPiece->m_pHome == g_pChessBox)
		g_pChessBox->DelPiece(nSrcGrid);

	//增加棋子
	g_pChessBoard->AddPiece(nDestGrid, nKind);

	if (nLostKind != 0)
		g_pChessBox->AddPiece(nLostKind, nLostKind);
}

void CArrangeAction::ChoicePiece(WChessPiece *pPiece)
{
	if (pPiece == m_pSelected)
	{
		//取消选择
		UnSelect();
	}
	else if (pPiece->m_pHome == g_pChessBoard)
	{
		if (m_pSelected)
		{
			//玩家摆棋
			ArrangePiece(m_pSelected->GetGrid(), pPiece->GetGrid(), m_pSelected->GetKind());
		}
		else
		{
			//选择棋子
			Select(pPiece);
		}
	}
	else if (pPiece->m_pHome == g_pChessBox)
	{
		//切换选择
		UnSelect();
		Select(pPiece);
	}
}

void CArrangeAction::ChoiceBoard(int nGrid)
{
	if (m_pSelected == NULL)
		return;

	if (nGrid == INVALID_GRID)
		return UnSelect();

	//玩家摆棋
	ArrangePiece(m_pSelected->GetGrid(), nGrid, m_pSelected->GetKind());
}

void CArrangeAction::ClearPiece(WChessPiece *pPiece)
{
	if (m_pSelected == pPiece)
		UnSelect();

	//删除棋子
	g_pChessBoard->DelPiece(pPiece->GetGrid());
	g_pChessBoard->ClearFixed(pPiece->GetGrid());

	//增加棋子
	g_pChessBox->AddPiece(pPiece->GetKind(), pPiece->GetKind());
}

void CArrangeAction::ToggleFixed(WChessPiece *pPiece)
{
	int nGrid = pPiece->GetGrid();

	//切换固定状态
	g_pChessBoard->ToggleFixed(nGrid);
}
