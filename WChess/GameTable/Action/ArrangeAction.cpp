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

	//ȡ��ѡ��
	WChessPiece *pPiece = m_pSelected;
	UnSelect();

	//ɾ������
	if (pPiece->m_pHome == g_pChessBoard)
	{
		g_pChessBoard->DelPiece(nSrcGrid);
		g_pChessBoard->ClearFixed(nSrcGrid);
	}
	if (pPiece->m_pHome == g_pChessBox)
		g_pChessBox->DelPiece(nSrcGrid);

	//��������
	g_pChessBoard->AddPiece(nDestGrid, nKind);

	if (nLostKind != 0)
		g_pChessBox->AddPiece(nLostKind, nLostKind);
}

void CArrangeAction::ChoicePiece(WChessPiece *pPiece)
{
	if (pPiece == m_pSelected)
	{
		//ȡ��ѡ��
		UnSelect();
	}
	else if (pPiece->m_pHome == g_pChessBoard)
	{
		if (m_pSelected)
		{
			//��Ұ���
			ArrangePiece(m_pSelected->GetGrid(), pPiece->GetGrid(), m_pSelected->GetKind());
		}
		else
		{
			//ѡ������
			Select(pPiece);
		}
	}
	else if (pPiece->m_pHome == g_pChessBox)
	{
		//�л�ѡ��
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

	//��Ұ���
	ArrangePiece(m_pSelected->GetGrid(), nGrid, m_pSelected->GetKind());
}

void CArrangeAction::ClearPiece(WChessPiece *pPiece)
{
	if (m_pSelected == pPiece)
		UnSelect();

	//ɾ������
	g_pChessBoard->DelPiece(pPiece->GetGrid());
	g_pChessBoard->ClearFixed(pPiece->GetGrid());

	//��������
	g_pChessBox->AddPiece(pPiece->GetKind(), pPiece->GetKind());
}

void CArrangeAction::ToggleFixed(WChessPiece *pPiece)
{
	int nGrid = pPiece->GetGrid();

	//�л��̶�״̬
	g_pChessBoard->ToggleFixed(nGrid);
}
