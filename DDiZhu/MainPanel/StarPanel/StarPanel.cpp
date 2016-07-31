#include "StdAfx.h"
#include "StarPanel.h"
#include "SunRaise\SunRaise.h"
#include "MoonRaise\MoonRaise.h"
#include <boost\bind.hpp>

CStarPanel::CStarPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
}

CStarPanel::~CStarPanel()
{
}

bool CStarPanel::Initial()
{
	m_pSunRaise = new CSunRaise(&ImageSunRaise, CRect(10, 10, 10 + 39, 10 + 39), CRect(0, 0, 156, 39), 2);
	if (!m_pSunRaise)
		return false;
	this->AddObject(m_pSunRaise);

	m_pMoonRaise = new CMoonRaise(&ImageSunRaise, CRect(60, 10, 60 + 39, 10 + 39), CRect(0, 52, 78, 52 + 78), 1);
	if (!m_pMoonRaise)
		return false;
	this->AddObject(m_pMoonRaise);

	return true;
}

void CStarPanel::OnLClick(UINT nFlags, CPoint point)
{
	SetDrawPos(m_xDrawPos.x + 100, m_xDrawPos.y);
}

void CStarPanel::OnRClick(UINT nFlags, CPoint point)
{
}
