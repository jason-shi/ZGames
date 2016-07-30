#include "StdAfx.h"
#include "MirChoice.h"

CMirChoice::CMirChoice(CPoint xDrawPos)
{
	Set(&ImageChoice, xDrawPos, CPoint(0, 0), CSize(193, 121), 0);
	SetCease();
}

CMirChoice::~CMirChoice()
{
}

bool CMirChoice::Initial()
{
	if (!CDialog::Initial())
		return false;

	return true;
}

bool CMirChoice::Release()
{
	return true;
}

INT_PTR CMirChoice::DialogResult()
{
	return type_dir;
}

void CMirChoice::OnLClick(UINT nFlags, CPoint point)
{
	int nX = (point.x - m_xDrawPos.x) / 24;
	int nY = (point.y - m_xDrawPos.y) / 24;

	if (nY < 3 || nY == 3 && nX < 1)
		type_dir = (nX + (nY << 3)) << 3;
	else if (nY == 4)
		type_dir = nX + (MT_TARGET << 3);
	else
		type_dir = 0;

	OnClose();
}
