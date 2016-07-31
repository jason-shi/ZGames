#include "StdAfx.h"
#include "HandBmp.h"
#include "Card.h"

using namespace std;

CHandBmp::CHandBmp()
{
}

CHandBmp::~CHandBmp()
{
}

void CHandBmp::Clear()
{
	memset(bitmap, 0, sizeof(word [4]));
}

void CHandBmp::OnInit(VECCARD &vecCard)
{
	Clear();

	byte byCount = 0;
	for (int i = 0; i < (int)vecCard.size(); i += byCount)
	{
		//���������ͬ�Ƶ������
		byCount = 1;
		if (vecCard[i]->GetRound() != 0)
			continue;
		for (int j = i + 1; j < (int)vecCard.size(); j++)
		{
			if (vecCard[j]->GetRound() != 0)
				continue;
			if (vecCard[j]->pips != vecCard[i]->pips)
				break;
			byCount++;
		}
		bitmap[byCount - 1] |= 1 << vecCard[i]->pips;
	}
}

void CHandBmp::OnPut(VECBYTE &vecCardValue)
{
	VECBYTE::iterator iter;
	for (iter = vecCardValue.begin(); iter != vecCardValue.end(); iter++)
		DelCard(*iter);
}

void CHandBmp::UnPut(VECBYTE &vecCardValue)
{
	VECBYTE::iterator iter;
	for (iter = vecCardValue.begin(); iter != vecCardValue.end(); iter++)
		AddCard(*iter);
}

void CHandBmp::DelCard(byte byCardValue)
{
	byte pips = byCardValue & 0x0F;
	word mask = 1 << pips;

	if (bitmap[0] & mask) //1�ű�0��
		bitmap[0] ^= mask;
	else if (bitmap[1] & mask) //2�ű�1��
	{
		bitmap[1] ^= mask;
		bitmap[0] |= mask;
	}
	else if (bitmap[2] & mask) //3�ű�2��
	{
		bitmap[2] ^= mask;
		bitmap[1] |= mask;
	}
	else //4�ű�3��
	{
		bitmap[3] ^= mask;
		bitmap[2] |= mask;
	}
}

void CHandBmp::AddCard(byte byCardValue)
{
	byte pips = byCardValue & 0x0F;
	word mask = 1 << pips;

	if (bitmap[0] & mask) //1�ű�2��
	{
		bitmap[0] ^= mask;
		bitmap[1] |= mask;
	}
	else if (bitmap[1] & mask) //2�ű�3��
	{
		bitmap[1] ^= mask;
		bitmap[2] |= mask;
	}
	else if (bitmap[2] & mask) //3�ű�4��
	{
		bitmap[2] ^= mask;
		bitmap[3] |= mask;
	}
	else //0�ű�1��
	{
		bitmap[0] |= mask;
	}
}
