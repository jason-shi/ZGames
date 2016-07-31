#include "StdAfx.h"
#include "Poke.h"
#include <algorithm>
#include <boost\bind.hpp>

CPoke::CPoke()
{
	m_pCardPool = NULL;
}

CPoke::~CPoke()
{
	if (m_pCardPool)
	{
		m_pCardPool->ForEach(boost::bind(&CCard::Release, _1), NULL);

		delete m_pCardPool;
		m_pCardPool = NULL;
	}
}

bool CPoke::Initial()
{
	//初始化卡牌对象池
	m_pCardPool = CreateZPool(sizeof(CCard), 54, &CCard(&ImagePoke, CRect(0, 0, CardWidth, CardHeight), CRect(0, 0, CardWidth, CardHeight), 0));
	if (!m_pCardPool)
		return false;
	m_pCardPool->ForEach(boost::bind(&CCard::Initial, _1), NULL);
	return true;
}

CCard *CPoke::GetCard()
{
	CCard *pCard = m_vecCard.back();
	m_vecCard.pop_back();

	return pCard;
}

void CPoke::ShuffleCards()
{
	m_pCardPool->Reclaim(); //重置资源
	m_vecCard.clear();

	CCard *pCard;
	for (byte suit = 1; suit <= 4; suit++)
	{
		for (byte pips = 0; pips < 13; pips++)
		{
			pCard = (CCard *)m_pCardPool->GetNode();
			pCard->InitWith(pips, suit);
			m_vecCard.push_back(pCard);
		}
	}

	//添加小王
	pCard = (CCard *)m_pCardPool->GetNode();
	pCard->InitWith(13, 1);
	m_vecCard.push_back(pCard);

	//添加大王
	pCard = (CCard *)m_pCardPool->GetNode();
	pCard->InitWith(14, 1);
	m_vecCard.push_back(pCard);

	random_shuffle(m_vecCard.begin(), m_vecCard.end());
}

bool CPoke::SaveCards(TCHAR wcsFileName[_MAX_PATH])
{
	FILE *fp = _wfopen(wcsFileName, L"w+");
	if (fp == NULL)
		return false;

	TCHAR wcsBuff[512];
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		wsprintf(wcsBuff, L"%02X%02X,", m_vecCard[i]->pips, m_vecCard[i]->suit);
		fputws(wcsBuff, fp);
	}
	fputws(L"\n", fp);

	fclose(fp);
	return true;
}

bool CPoke::LoadCards(TCHAR wcsFileName[_MAX_PATH])
{
	FILE *fp = _wfopen(wcsFileName, L"r");
	if (fp == NULL)
		return false;

	TCHAR wcsBuff[512];
	fgetws(wcsBuff, 512, fp);

	m_pCardPool->Reclaim(); //重置资源
	m_vecCard.clear();

	CCard *pCard;
	for (int i = 0; i < (int)wcslen(wcsBuff) - 1; i += 5)
	{
		byte pips = HEX_2_DEC(wcsBuff[i + 0]) * 16 + HEX_2_DEC(wcsBuff[i + 1]);
		byte suit = HEX_2_DEC(wcsBuff[i + 2]) * 16 + HEX_2_DEC(wcsBuff[i + 3]);

		pCard = (CCard *)m_pCardPool->GetNode();
		pCard->InitWith(pips, suit);
		m_vecCard.push_back(pCard);
	}

	fclose(fp);
	return true;
}
