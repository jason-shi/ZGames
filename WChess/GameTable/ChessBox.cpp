#include "StdAfx.h"
#include "ChessPiece.h"
#include "ChessBox.h"
#include "GameTable.h"
#include "Action\Action.h"
#include <boost\bind.hpp>

static int TMax[13] = { 0, 10, 10, 10, 9, 8, 1, 10, 10, 10, 9, 8, 1, };

static int TMul38[8] = { 0, 38, 76, 114, 152, 190, 228, 266, };

//利用快查计算棋子位置
#define PIECE_SRC(nKind)	CPoint(36 * (nKind), 0)
#define PIECE_POS(nKind)	(nKind < B_ROOK ? CPoint(0, TMul38[(nKind) - 1]) : CPoint(38, TMul38[(nKind) - 7]))

WChessBox::WChessBox() : CPanel()
{
	Show(false); //初始化为不显示

	memset(m_nRemain, 0, sizeof(m_nRemain));
	memset(m_pPiece, 0, sizeof(m_pPiece));
}

WChessBox::~WChessBox()
{
	for (int nKind = R_ROOK; nKind <= B_KING; nKind++)
	{
		delete m_pPiece[nKind];
		m_pPiece[nKind] = NULL;
	}
}

bool WChessBox::Initial()
{
	if (!CPanel::Initial())
		return false;

	for (int nKind = R_ROOK; nKind <= B_KING; nKind++)
	{
		m_pPiece[nKind] = new WChessPiece(PIECE_POS(nKind), nKind, nKind);
		if (!m_pPiece[nKind] || !m_pPiece[nKind]->Initial())
			return false;
		m_pPiece[nKind]->m_pHome = this;
		m_pPiece[nKind]->SetSrcPos(PIECE_SRC(nKind));
		m_pPiece[nKind]->SetMouseProc(MP_LCLICK, boost::bind(&WChessBox::OnLClick, this, _1));

		AddObject(m_pPiece[nKind]);
	}
	return true;
}

bool WChessBox::Release()
{
	for (int nKind = R_ROOK; nKind <= B_KING; nKind++)
	{
		delete m_pPiece[nKind];
		m_pPiece[nKind] = NULL;
	}
	return true;
}

void WChessBox::AddPiece(int nGrid, int nKind)
{
	//ASSERT(nGrid == nKind);

	m_nRemain[nKind]++;
	m_pPiece[nKind]->Show();
}

void WChessBox::DelPiece(int nGrid)
{
	int nKind = nGrid;

	if (--m_nRemain[nKind] == 0)
		m_pPiece[nKind]->Show(false);
}

void WChessBox::SetAvailPiece()
{
	memcpy(m_nRemain, TMax, sizeof(TMax));
	for (int nGrid = 0; nGrid < 64; nGrid++)
	{
		if (board[nGrid] > 0)
			m_nRemain[board[nGrid]]--;
	}

	for (int nKind = R_ROOK; nKind <= B_KING; nKind++)
	{
		bool bShow = m_nRemain[nKind] != 0;
		m_pPiece[nKind]->Show(bShow);
	}
}

void WChessBox::OnLClick(CObject *pPiece)
{
	CAction *pAction = g_pGameTable->GetAction();
	if (pAction)
		pAction->ChoicePiece((WChessPiece *)pPiece);
}
