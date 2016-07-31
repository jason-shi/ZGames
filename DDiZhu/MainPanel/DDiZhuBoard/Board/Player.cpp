#include "StdAfx.h"
#include "Player.h"
#include "Logic.h"

using namespace std;

CPlayer::CPlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair)
			: CPanel(pImage, xDrawPos, xSize, 0), m_byChair(byChair)
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Initial()
{
	if (!CPanel::Initial())
		return false;

	return true;
}

void CPlayer::OnStartGame()
{
	ClearCards();
	ClearHandBmp();
}

void CPlayer::OnSendCard(CCard *pCard)
{
	pCard->SetDrawPos(m_vecCard.size() * CardWidth, CardLift);

	AddObject(pCard);
	m_vecCard.push_back(pCard);
}

void CPlayer::OnSendCardsOver()
{
	SortCards();
	ShowCards();
	m_tHandBmp.OnInit(m_vecCard);
	UpdateAlive();
}

void CPlayer::ClearCards()
{
	VECCARD::iterator iter;
	for (iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
		DelObject(*iter);

	m_vecCard.clear();
}

void CPlayer::SortCards()
{
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		for (int j = i + 1; j < (int)m_vecCard.size(); j++)
		{
			if (m_vecCard[i]->GetSortIndex() < m_vecCard[j]->GetSortIndex())
				continue;

			CCard *pCard = m_vecCard[i];
			m_vecCard[i] = m_vecCard[j];
			m_vecCard[j] = pCard;
		}
	}
}

void CPlayer::ShowCards()
{
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		byte pips = m_vecCard[i]->pips;
		byte suit = m_vecCard[i]->suit;

		//取消选择状态
		m_vecCard[i]->UnSelect();

		m_vecCard[i]->SetSrcPos(pips * CardWidth, 0);
		m_vecCard[i]->SetDrawPos(i * CardWidth, CardLift);
	}
}

void CPlayer::GetHandBmp(word bitmap[4])
{
	memcpy(bitmap, m_tHandBmp.bitmap, sizeof(word [4]));
}

bool CPlayer::HasCards(byte byGrade, byte byCount)
{
	byte byTemp = 0;
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		if (m_vecCard[i]->GetRound() != 0)
			continue;
		if (m_vecCard[i]->pips == byGrade)
			byTemp++;
	}
	return byTemp >= byCount;
}

void CPlayer::UpdateStatus(byte byType, byte byGrade, byte byCount, word wAttach)
{
	m_byType = byType;
	m_byGrade = byGrade;
	m_byCount = byCount;
	m_wAttach = wAttach;
}

void CPlayer::GetResponse(byte &byRspType, byte &byGrade, byte &byCount, word &wAttach)
{
	byRspType = m_byRspType;
	byGrade = m_byGrade;
	byCount = m_byCount;
	wAttach = m_wAttach;
}

bool CPlayer::OnPut(byte byRound, byte byIndex)
{
	vector<CCard *> vecCard;
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		if (m_vecCard[i]->IsSelected())
			vecCard.push_back(m_vecCard[i]);
	}

	//校验出牌合法性
	if (vecCard.size() == 0)
		return false;

	word bitmap[4] = { 0 };
	VECBYTE vecCardValue;
	for (int i = 0; i < (int)vecCard.size(); i++)
		vecCardValue.push_back(vecCard[i]->GetValue());

	CLogic::TransToBitmap(bitmap, vecCardValue);
	byte byRspType = CLogic::GetType(bitmap);
	if (byRspType == TYPE_ERROR)
		return false;
	if (!CLogic::CheckLegal(m_byType, byRspType, m_byGrade, m_byCount, bitmap))
		return false;

	for (int i = 0; i < (int)vecCard.size(); i++)
	{
		vecCard[i]->UnSelect();
		vecCard[i]->OnPut(byRound, byIndex);
	}

	m_tHandBmp.OnPut(vecCardValue);

	m_byRspType = byRspType;
	m_byGrade = CLogic::GetGrade(byRspType, bitmap);
	m_byCount = CLogic::GetCount(byRspType, bitmap);
	m_wAttach = CLogic::GetAttach(byRspType, bitmap);

	UpdateAlive();
	return true;
}

void CPlayer::UnPut(byte byRound, byte byIndex)
{
	VECBYTE vecCardValue;

	VECCARD::iterator iter;
	for (iter = m_vecCard.begin(); iter != m_vecCard.end(); iter++)
	{
		//根据记录的回合数和应牌索引撤消出牌
		if ((*iter)->UnPut(byRound, byIndex))
			vecCardValue.push_back((*iter)->GetValue());
	}
	m_tHandBmp.UnPut(vecCardValue);

	UpdateAlive();
}

void CPlayer::OnPut(byte byRound, byte byIndex, VECBYTE &vecCardValue)
{
	VECBYTE::iterator iter;
	for (iter = vecCardValue.begin(); iter != vecCardValue.end(); iter++)
	{
		//根据牌值查找并出牌
		VECCARD::iterator iter2;
		for (iter2 = m_vecCard.begin(); iter2 != m_vecCard.end(); iter2++)
		{
			if ((*iter2)->GetValue() == *iter && (*iter2)->GetRound() == 0)
			{
				(*iter2)->UnSelect();
				(*iter2)->OnPut(byRound, byIndex);
				break;
			}
		}
	}
	m_tHandBmp.OnPut(vecCardValue);

	UpdateAlive();
}

void CPlayer::UpdateAlive()
{
	for (int i = 0; i < (int)m_vecCard.size(); i++)
	{
		if (m_vecCard[i]->GetRound() == 0)
		{
			m_bAlive = true;
			return;
		}
	}
	m_bAlive = false;
}
