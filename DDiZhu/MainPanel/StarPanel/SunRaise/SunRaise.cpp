#include "StdAfx.h"
#include "SunRaise.h"
#include <boost\bind.hpp>

CSunRaise::CSunRaise(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth)
				: CButton(pImage, xDrawRect, xSrcRect, nDepth)
{
	g_pZTimer->AddTimer(TM_SUN_RAISE, 30, boost::bind(&CSunRaise::OnTimer, this), NULL);
}

CSunRaise::~CSunRaise()
{
}

void CSunRaise::OnTimer()
{
	//SetDrawPos(m_xDrawPos.x + 3, m_xDrawPos.y);
	//m_xDrawPos.x += 10;
	//m_xDrawPos.y;
}

void CSunRaise::OnLClick(UINT nFlags, CPoint point)
{
	if (IsDisable())
		return;

	int x = rand() % 500;
	int y = rand() % 500;
	SetDrawPos(x, y);
}

void CSunRaise::OnRClick(UINT nFlags, CPoint point)
{
	Disable();
}
