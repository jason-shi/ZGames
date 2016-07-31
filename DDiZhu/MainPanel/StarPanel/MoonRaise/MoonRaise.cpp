#include "StdAfx.h"
#include "MoonRaise.h"
#include <boost\bind.hpp>

CMoonRaise::CMoonRaise(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
				: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
}

CMoonRaise::~CMoonRaise()
{
}

void CMoonRaise::OnTimer()
{
	//SetDrawPos(m_xDrawPos.x + 3, m_xDrawPos.y);
	//m_xDrawPos.x += 10;
	//m_xDrawPos.y;
}

void CMoonRaise::OnLClick(UINT nFlags, CPoint point)
{
	if (IsDisable())
		return;

	int x = rand() % 100;
	int y = rand() % 50;
	SetDrawPos(x, y);
}

void CMoonRaise::OnRClick(UINT nFlags, CPoint point)
{
	Disable();
}
