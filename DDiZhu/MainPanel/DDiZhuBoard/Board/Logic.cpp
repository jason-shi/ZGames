#include "StdAfx.h"
#include "Logic.h"

using namespace std;

void CLogic::TransToBitmap(word bitmap[4], VECBYTE &vecCardValue)
{
	byte byCount = 0;
	for (int i = 0; i < (int)vecCardValue.size(); i += byCount)
	{
		//计算具有相同牌点的牌数
		byCount = 1;
		for (int j = i + 1; j < (int)vecCardValue.size(); j++)
		{
			if (vecCardValue[i] != vecCardValue[j])
				break;
			byCount++;
		}
		bitmap[byCount - 1] |= 1 << vecCardValue[i];
	}
}

void CLogic::TransToValue(VECBYTE &vecCardValue, word bitmap[4])
{
	for (int i = 0; i < 3; i++)
	{
		word _bitmap = bitmap[i];
		while (_bitmap)
		{
			int nIndex = TblBitIndex[(_bitmap & -_bitmap) % 37];
			for (int j = 0; j <= i; j++)
				vecCardValue.push_back(nIndex);
		}
	}
}

byte CLogic::GetType(word bitmap[4])
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];

	int nBitCount = WORD_BIT_COUNT(_bitmap);
	//检测顺子
	if (nBitCount >= 5)
	{
		if (TblShunZi5[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[1] | bitmap[2] | bitmap[3]) == 0)
			return TYPE_SHUNZI;
	}
	//检测连对
	if (nBitCount >= 3)
	{
		if (TblShunZi3[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[0] | bitmap[2] | bitmap[3]) == 0)
			return TYPE_LIANDUI;
	}
	//检测飞机
	if (nBitCount >= 2)
	{
		if (TblShunZi2[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[0] | bitmap[1] | bitmap[3]) == 0)
			return TYPE_PLANE;
	}
	//至此，不会是顺子，不会是连对，不会是飞机
	if (nBitCount == 1)
	{
		if (bitmap[0] != 0) return TYPE_SINGLE;
		if (bitmap[1] != 0) return TYPE_PAIR;
		if (bitmap[2] != 0) return TYPE_TRIP;
		if (bitmap[3] != 0) return TYPE_BOMB;
	}
	else if (nBitCount == 2)
	{
		if (_bitmap == MASK_ROCKET)
			return TYPE_ROCKET;
		if (bitmap[2] != 0)
		{
			if (bitmap[0] != 0) return TYPE_TRIP_S;
			if (bitmap[1] != 0) return TYPE_TRIP_P;
		}
	}
	else if (nBitCount == 3)
	{
		if (WORD_BIT_COUNT(bitmap[3]) == 1)
		{
			if (WORD_BIT_COUNT(bitmap[0]) == 2 && (bitmap[0] & MASK_ROCKET) != MASK_ROCKET) return TYPE_QUAD_S;
			if (WORD_BIT_COUNT(bitmap[1]) == 2) return TYPE_QUAD_P;
		}
	}
	else if (nBitCount >= 4 && (nBitCount & 1) == 0)
	{
		int nHalf = nBitCount / 2;
		if (TblShunZi2[bitmap[2]] == bitmap[2] && WORD_BIT_COUNT(bitmap[2]) == nHalf)
		{
			if (WORD_BIT_COUNT(bitmap[0]) == nHalf && (bitmap[0] & MASK_ROCKET) != MASK_ROCKET) return TYPE_PLANE_S;
			if (WORD_BIT_COUNT(bitmap[1]) == nHalf) return TYPE_PLANE_P;
		}
	}
	return TYPE_ERROR;
}

byte CLogic::GetGrade(byte byRspType, word bitmap[4])
{
	word _bitmap = 0;
	switch (byRspType)
	{
	case TYPE_SINGLE:  _bitmap = bitmap[0]; break;
	case TYPE_PAIR:    _bitmap = bitmap[1]; break;
	case TYPE_TRIP:    _bitmap = bitmap[2]; break;
	case TYPE_TRIP_S:  _bitmap = bitmap[2]; break;
	case TYPE_TRIP_P:  _bitmap = bitmap[2]; break;
	case TYPE_PLANE:   _bitmap = bitmap[2]; break;
	case TYPE_PLANE_S: _bitmap = bitmap[2]; break;
	case TYPE_PLANE_P: _bitmap = bitmap[2]; break;
	case TYPE_QUAD_S:  _bitmap = bitmap[3]; break;
	case TYPE_QUAD_P:  _bitmap = bitmap[3]; break;
	case TYPE_SHUNZI:  _bitmap = bitmap[0]; break;
	case TYPE_LIANDUI: _bitmap = bitmap[1]; break;
	case TYPE_BOMB:    _bitmap = bitmap[3]; break;
	case TYPE_ROCKET:  _bitmap = bitmap[0]; break;
	}
	return TblBitIndex[(_bitmap & -_bitmap) % 37];
}

byte CLogic::GetCount(byte byRspType, word bitmap[4])
{
	switch (byRspType)
	{
	case TYPE_SHUNZI:
		return WORD_BIT_COUNT(bitmap[0]);
	case TYPE_LIANDUI:
		return WORD_BIT_COUNT(bitmap[1]);
	case TYPE_PLANE:
	case TYPE_PLANE_S:
	case TYPE_PLANE_P:
		return WORD_BIT_COUNT(bitmap[2]);
	default:
		return 0;
	}
}

word CLogic::GetAttach(byte byRspType, word bitmap[4])
{
	switch (byRspType)
	{
	case TYPE_TRIP_S:
	case TYPE_QUAD_S:
	case TYPE_PLANE_S:
		return bitmap[0];
	case TYPE_TRIP_P:
	case TYPE_PLANE_P:
	case TYPE_QUAD_P:
		return bitmap[1];
	default:
		return 0;
	}
}

bool CLogic::CheckLegal(byte byType, byte byRspType, byte byGrade, byte byCount, word bitmap[4])
{
	switch (byType)
	{
	case TYPE_GVP:
		return byRspType != TYPE_ERROR;
	case TYPE_BOMB:
		if (byRspType == TYPE_BOMB)
			return bitmap[3] > (1 << byGrade);
		return byRspType == TYPE_ROCKET;
	case TYPE_ROCKET:
		return false;
	}

	return byType == byRspType && GetGrade(byRspType, bitmap) > byGrade
		|| byRspType == TYPE_BOMB
		|| byRspType == TYPE_ROCKET;
}

void CLogic::GetValues(VECBYTE &vecCardValue, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	switch (byRspType)
	{
	case TYPE_BOMB: //坠落
	case TYPE_QUAD_S:
	case TYPE_QUAD_P:
		vecCardValue.push_back(byGrade);
	case TYPE_TRIP:
	case TYPE_TRIP_S:
	case TYPE_TRIP_P:
		vecCardValue.push_back(byGrade);
	case TYPE_PAIR:
		vecCardValue.push_back(byGrade);
	case TYPE_SINGLE:
		vecCardValue.push_back(byGrade);
	case TYPE_GVP:
		break;
	case TYPE_ROCKET:
		vecCardValue.push_back(13);
		vecCardValue.push_back(14);
		break;
	case TYPE_PLANE:
	case TYPE_PLANE_S:
	case TYPE_PLANE_P:
		for (int i = byGrade; i < byGrade + byCount; i++)
			vecCardValue.push_back(i);
	case TYPE_LIANDUI:
		for (int i = byGrade; i < byGrade + byCount; i++)
			vecCardValue.push_back(i);
	case TYPE_SHUNZI:
		for (int i = byGrade; i < byGrade + byCount; i++)
			vecCardValue.push_back(i);
		break;
	}

	//获得附加的单张
	if (byRspType == TYPE_TRIP_S || byRspType == TYPE_PLANE_S || byRspType == TYPE_QUAD_S)
	{
		while (wAttach)
		{
			byte byGrade = TblBitIndex[(wAttach & -wAttach) % 37];
			wAttach &= wAttach - 1;
			vecCardValue.push_back(byGrade);
		}
	}
	//获得附加的对子
	else
	if (byRspType == TYPE_TRIP_P || byRspType == TYPE_PLANE_P || byRspType == TYPE_QUAD_P)
	{
		while (wAttach)
		{
			byte byGrade = TblBitIndex[(wAttach & -wAttach) % 37];
			wAttach &= wAttach - 1;
			vecCardValue.push_back(byGrade);
			vecCardValue.push_back(byGrade);
		}
	}
}
