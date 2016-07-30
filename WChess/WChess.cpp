#include "StdAfx.h"
#include "WChess.h"

int					 board[65]; //增加一个无效格
TBlkMove			 g_tBlkMove;

bool IsBoardLegal()
{
	static int T1[10] = { 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, };
	static int T2[11] = { 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, };

	int count[13] = { 0 };
	for (int i = 0; i < 64; i++)
		count[board[i]]++;

	//红方个数检测
	int nCountRed = 0;
	nCountRed += T2[count[R_ROOK]];
	nCountRed += T2[count[R_HORSE]];
	nCountRed += T2[count[R_BISHOP]];
	nCountRed += T1[count[R_QUEEN]];
	nCountRed +=    count[R_PAWN];
	if (nCountRed > 8)
		return false;

	//黑方个数检测
	int nCountBlk = 0;
	nCountBlk += T2[count[B_ROOK]];
	nCountBlk += T2[count[B_HORSE]];
	nCountBlk += T2[count[B_BISHOP]];
	nCountBlk += T1[count[B_QUEEN]];
	nCountBlk +=    count[B_PAWN];
	if (nCountBlk > 8)
		return false;

	//位置检测
	for (int i = 0; i < 64; i++)
	{
		if ((board[i] == R_PAWN || board[i] == B_PAWN) && (i < 8 || i > 55))
			return false;
	}

	//将帅检测
	return count[R_KING] == 1 && count[B_KING] == 1;
}

bool IsBlkLegal()
{
	if (!IsBoardLegal())
		return false;

	if (InCheckBlk())
		return false;

	return true;
}

bool IsBothLegal()
{
	if (!IsBoardLegal())
		return false;

	if (InCheckRed() || InCheckBlk())
		return false;

	return true;
}

void InitBoard()
{
	//效率要求：中低
	for (int i = 0; i < 8; i++)
	{
		int *grid = board + (i << 3);
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
	}
}

static int GetPieceInLine(int x1, int y1, int x2, int y2)
{
	int nCount = 0;
	if (x1 == x2)
	{
		for (int i = min(y1, y2) + 1; i < max(y1, y2); i++)
		{
			if (Board(x1, i) != 0)
				nCount++;
		}
	}
	if (y1 == y2)
	{
		for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
		{
			if (Board(i, y1) != 0)
				nCount++;
		}
	}
	if (x1 - x2 == y1 - y2)
	{
		for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
		{
			if (Board(i, min(y1, y2) + (i - min(x1, x2))) != 0)
				nCount++;
		}
	}
	if (x1 - x2 == y2 - y1)
	{
		for (int i = min(x1, x2) + 1; i < max(x1, x2); i++)
		{
			if (Board(i, max(y1, y2) - (i - min(x1, x2))) != 0)
				nCount++;
		}
	}
	return nCount;
}

static bool IsCheckByHorse(int kx, int ky, int hx, int hy)
{
	return abs(kx - hx) == 2 && abs(ky - hy) == 1
		|| abs(kx - hx) == 1 && abs(ky - hy) == 2;
}

bool InCheckRed()
{
	int kx, ky;
	for (int i = 0; i < 64; i++)
	{
		if (board[i] == R_KING)
		{
			kx = i & 7;
			ky = i >> 3;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Board(i, j) == 0)
				continue;
			switch (Board(i, j))
			{
			case B_ROOK:
				if ((i == kx || j == ky) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case B_HORSE:
				if (IsCheckByHorse(kx, ky, i, j))
					return true;
				break;
			case B_BISHOP:
				if ((i - kx == j - ky || i - kx == ky - j) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case B_QUEEN:
				if ((i == kx || j == ky || i - kx == j - ky || i - kx == ky - j) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case B_PAWN:
				if (ky + 1 == j && (kx == i - 1 || kx == i + 1))
					return true;
				break;
			case B_KING:
				if (abs(i - kx) <= 1 && abs(j - ky) <= 1)
					return true;
				break;
			}
		}
	}
	return false;
}

bool InCheckBlk()
{
	int kx, ky;
	for (int i = 0; i < 64; i++)
	{
		if (board[i] == B_KING)
		{
			kx = i & 7;
			ky = i >> 3;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Board(i, j) == 0)
				continue;

			switch (Board(i, j))
			{
			case R_ROOK:
				if ((i == kx || j == ky) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case R_HORSE:
				if (IsCheckByHorse(kx, ky, i, j))
					return true;
				break;
			case R_BISHOP:
				if ((i - kx == j - ky || i - kx == ky - j) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case R_QUEEN:
				if ((i == kx || j == ky || i - kx == j - ky || i - kx == ky - j) && GetPieceInLine(kx, ky, i, j) == 0)
					return true;
				break;
			case R_PAWN:
				if (ky - 1 == j && (kx == i - 1 || kx == i + 1))
					return true;
				break;
			case R_KING:
				if (abs(i - kx) <= 1 && abs(j - ky) <= 1)
					return true;
				break;
			}
		}
	}
	return false;
}
