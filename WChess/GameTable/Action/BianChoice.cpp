#include "StdAfx.h"
#include "BianChoice.h"

CBianChoice::CBianChoice(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos)
{
	m_nBian = 0;

	Set(pImage, xDrawPos, xSrcPos, CSize(144, 36), 0);
	SetCease();
}

CBianChoice::~CBianChoice()
{
}

bool CBianChoice::Initial()
{
	if (!CPanel::Initial())
		return false;

	return true;
}

bool CBianChoice::Release()
{
	return true;
}

INT_PTR CBianChoice::DialogResult()
{
	return m_nBian;
}

void CBianChoice::OnLClick(UINT nFlags, CPoint point)
{
	int nX = (point.x - m_xDrawPos.x) / 36;
	m_nBian = nX + 7;

	OnClose();
}
