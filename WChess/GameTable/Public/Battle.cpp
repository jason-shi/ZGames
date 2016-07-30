#include "StdAfx.h"
#include "Battle.h"

int CountHorz(int nRank, int nFrom, int nTo)
{
	if (nFrom > nTo)
	{
		int nTemp = nFrom;
		nFrom = nTo;
		nTo = nTemp;
	}
	int nCount = 0;
	for (int i = nFrom + 1; i < nTo; i++)
	{
		if (board[i + (nRank << 3)] != 0)
			nCount++;
	}
	return nCount;
}

int CountVert(int nFile, int nFrom, int nTo)
{
	if (nFrom > nTo)
	{
		int nTemp = nFrom;
		nFrom = nTo;
		nTo = nTemp;
	}
	int nCount = 0;
	for (int i = nFrom + 1; i < nTo; i++)
	{
		if (board[nFile + (i << 3)] != 0)
			nCount++;
	}
	return nCount;
}

int CountSlash(int X, int Y, int nX, int nY)
{
	if (X > nX)
	{
		int nTemp = X;
		X = nX;
		nX = nTemp;
	}
	int nCount = 0;
	for (int i = X + 1; i < nX; i++)
	{
		if (board[i + ((Y + (i - X)) << 3)] != 0)
			nCount++;
	}
	return nCount;
}

int CountBackSlash(int X, int Y, int nX, int nY)
{
	if (X > nX)
	{
		int nTemp = X;
		X = nX;
		nX = nTemp;
	}
	int nCount = 0;
	for (int i = X + 1; i < nX; i++)
	{
		if (board[i + ((Y - (i - X)) << 3)] != 0)
			nCount++;
	}
	return nCount;
}

bool CheckBlackBian(int nSrcGrid, int nDestGrid, int nKind)
{
	if (board[nDestGrid] >= B_ROOK)
		return false;

	int X = nSrcGrid & 7;
	int Y = nSrcGrid >> 3;
	int nX = nDestGrid & 7;
	int nY = nDestGrid >> 3;

	if (nKind != B_PAWN)
		return false;
	if (nY != 0)
		return false;

	return board[nX + (nY << 3)] == 0 && X == nX && Y - nY == 1
		|| board[nX + (nY << 3)] != 0 && abs(X - nX) == 1 && Y - nY == 1;
}

bool CheckBlackMove(int nSrcGrid, int nDestGrid, int nKind, int nBian)
{
	if (board[nDestGrid] >= B_ROOK)
		return false;

	int X = nSrcGrid & 7;
	int Y = nSrcGrid >> 3;
	int nX = nDestGrid & 7;
	int nY = nDestGrid >> 3;

	switch (nKind)
	{
	case B_ROOK:
		return X == nX && CountVert(X, Y, nY) == 0 || Y == nY && CountHorz(Y, X, nX) == 0;
	case B_HORSE:
		return abs(X - nX) == 2 && abs(Y - nY) == 1
			|| abs(X - nX) == 1 && abs(Y - nY) == 2;
	case B_BISHOP:
		return X - nX == Y - nY && CountSlash(X, Y, nX, nY) == 0
			|| X - nX == nY - Y && CountBackSlash(X, Y, nX, nY) == 0;
	case B_QUEEN:
		return X == nX && CountVert(X, Y, nY) == 0 || Y == nY && CountHorz(Y, X, nX) == 0
			|| X - nX == Y - nY && CountSlash(X, Y, nX, nY) == 0
			|| X - nX == nY - Y && CountBackSlash(X, Y, nX, nY) == 0;
	case B_PAWN:
		if (nY == 0 && nBian == 0 || nY != 0 && nBian >= B_ROOK && nBian <= B_QUEEN)
			return false;
		return board[nX + (nY << 3)] == 0 && X == nX && Y - nY == 1
			|| board[nX + (nY << 3)] != 0 && abs(X - nX) == 1 && Y - nY == 1
			|| Y == 6 && nY == 4 && X == nX && board[nX + (nY << 3)] == 0 && board[nX + ((nY + 1) << 3)] == 0;
	case B_KING:
		return abs(X - nX) <= 1 && abs(Y - nY) <= 1;
	}
	return false;
}
