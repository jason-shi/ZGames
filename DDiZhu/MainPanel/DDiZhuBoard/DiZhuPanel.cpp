#include "StdAfx.h"
#include "DiZhuPanel.h"
#include "DDiZhuBoard.h"
#include "Board\Card.h"
#include <boost\bind.hpp>

CDiZhuPanel::CDiZhuPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	for (int i = 0; i < 3; i++)
		m_pBtnDiZhu[i] = NULL;
}

CDiZhuPanel::~CDiZhuPanel()
{
	for (int i = 0; i < 3; i++)
	{
		delete m_pBtnDiZhu[i];
		m_pBtnDiZhu[i] = NULL;
	}
}

bool CDiZhuPanel::Initial(CDDiZhuBoard *pDDiZhuBoard)
{
	if (!CPanel::Initial())
		return false;

	CRect rect(0, 0, 50, 35);
	for (int i = 0; i < 3; i++)
	{
		m_pBtnDiZhu[i] = new CButton(&ImageDDiZhu, rect + CPoint(0, i * PlayerHeight), CRect(200, 0, 400, 35), 0);
		m_pBtnDiZhu[i]->SetObjId(i);
		AddObject(m_pBtnDiZhu[i]);
		m_pBtnDiZhu[i]->SetMouseProc(MP_LCLICK, boost::bind(&CDDiZhuBoard::OnDiZhu, pDDiZhuBoard, _1, _2, _3));
	}
	return true;
}

void CDiZhuPanel::OnStartGame()
{
	m_vecDiCard.clear();
	Show(false);
}

void CDiZhuPanel::OnSendCard(int nIndex, CCard *pCard)
{
	pCard->SetDrawPos(60 + nIndex * (CardWidth + 5), 37);
	m_vecDiCard.push_back(pCard);
}
