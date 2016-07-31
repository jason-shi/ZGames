#include "StdAfx.h"
#include "DDiZhuOperPanel.h"
#include "DDiZhuBoard.h"
#include <boost\bind.hpp>

CDDiZhuOperPanel::CDDiZhuOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
						: COperPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
}

CDDiZhuOperPanel::~CDDiZhuOperPanel()
{
}

bool CDDiZhuOperPanel::Initial(CBoard *pBoard)
{
	if (!COperPanel::Initial(pBoard))
		return false;

	return true;
}

void CDDiZhuOperPanel::OnSendCardsOver(byte byChair)
{
	Show(false);
}

void CDDiZhuOperPanel::OnDiZhu(byte byChair)
{
	Show();
	m_pSprTurn->Show();
	m_pSprTurn->SetDrawPos(0, byChair * PlayerHeight);
}
