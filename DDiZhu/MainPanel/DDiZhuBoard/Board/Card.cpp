#include "StdAfx.h"
#include "Card.h"

CCard::CCard(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
		: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
	OnCardSelect = NULL;
}

CCard::~CCard()
{
}

void CCard::InitWith(byte pips, byte suit)
{
	m_bSelected = false;
	m_byRound = 0;
	m_byIndex = 0;
	OnCardSelect = NULL;

	this->pips = pips;
	this->suit = suit;
	SetSrcPos(pips * CardWidth, 0);
}

bool CCard::operator==(const CCard& tCard) const
{
	return suit == tCard.suit && pips == tCard.pips;
}

byte CCard::GetSortIndex() const
{
	return 14 - pips;
}

void CCard::OnSelect()
{
	if (IsSelected() || GetRound() != 0)
		return;

	Offset(0, -CardLift);
	m_bSelected = true;
}

void CCard::UnSelect()
{
	if (!IsSelected() || GetRound() != 0)
		return;

	Offset(0, CardLift);
	m_bSelected = false;
}

void CCard::OnPut(byte byRound, byte byIndex)
{
	m_byRound = byRound;
	m_byIndex = byIndex;
	SetSrcPos(pips * CardWidth, m_byRound * CardHeight);
}

bool CCard::UnPut(byte byRound, byte byIndex)
{
	if (byRound != m_byRound || byIndex != m_byIndex)
		return false;

	m_byRound = 0;
	m_byIndex = 0;
	SetSrcPos(pips * CardWidth, 0);
	return true;
}

void CCard::OnLClick(UINT nFlags, CPoint point)
{
	if (GetRound() != 0)
		return;

	IsSelected() ? UnSelect() : OnSelect();
}

void CCard::OnRClick(UINT nFlags, CPoint point)
{
}
