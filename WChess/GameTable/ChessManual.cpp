#include "StdAfx.h"
#include "Manual.h"
#include "ChessManual.h"

WChessManual::WChessManual() : CPanel()
{
}

WChessManual::~WChessManual()
{
	for (int i = 0; i < MaxManualLine; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			delete m_pLabel[i * 2 + j];
			m_pLabel[i * 2 + j] = NULL;
		}
	}
}

bool WChessManual::Initial()
{
	static HFONT hFont = CreateFont(20, 10, 0, 0, 3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	if (!CPanel::Initial())
		return false;

	//着法标签
	for (int i = 0; i < MaxManualLine; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			m_pLabel[i * 2 + j] = new CTextBlk(false, 20, CPoint(100 * j, 20 * i), CSize(100, 20), 0);
			if (!m_pLabel[i * 2 + j] || !m_pLabel[i * 2 + j]->Initial())
				return false;
			m_pLabel[i * 2 + j]->ClearImage();
			AddObject(m_pLabel[i * 2 + j]);
		}
	}
	return true;
}

bool WChessManual::Release()
{
	for (int i = 0; i < MaxManualLine; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			delete m_pLabel[i * 2 + j];
			m_pLabel[i * 2 + j] = NULL;
		}
	}
	return true;
}

void WChessManual::Update()
{
	static HFONT hFont = CreateFont(18, 18, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("SimSun.ttc"));

	int nLineExpect = (vecManual.size() + 1) / 2;
	int nIndex = nLineExpect < MaxManualLine ? 0 : (nLineExpect - MaxManualLine) * 2;

	//设置要显示的棋谱
	for (size_t i = nIndex; i < vecManual.size(); i++)
	{
		CTextBlk *pLabel = m_pLabel[i - nIndex];
		pLabel->OutText(vecManual[i].c_str(), wcslen(vecManual[i].c_str()), hFont, true, BGRA(0, 0, 0, 255));
	}
	for (int i = vecManual.size(); i < nIndex + MaxManualLine * 2; i++)
	{
		m_pLabel[i - nIndex]->ClearText();
	}
}
