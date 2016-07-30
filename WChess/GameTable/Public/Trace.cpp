#include "StdAfx.h"
#include "Trace.h"

int					 trcboard[65];

int					 g_nPRest[13]; //棋子剩余个数

TTrace				 TrList1[MAX_TRACE];
TTrace				 TrList2[MAX_TRACE * 4];

int					 TrCount1;
int					 TrCount2;

bool				 g_bLimitary;

static int			 g_nIdxBlk;

static int			 midboard[65];

static int			 TrIndex1;

static void			 TraceBianRed(int x, int y);
static void			 TraceBianBlk(int x, int y);

void InitTraceList()
{
	static int TMax[13] = { 0, 10, 10, 10, 9, 8, 1, 10, 10, 10, 9, 8, 1, };
	//统计个数
	memcpy(g_nPRest, TMax, sizeof(TMax));
	for (int i = 0; i < 64; i++)
	{
		if (board[i] > 0)
			g_nPRest[board[i]]--;
	}
}

bool FetchTrList1(int &nIndex)
{
	if (nIndex != 0)
		RestoreBoard(&TrList1[nIndex - 1]);

	if (nIndex == TrCount1)
		return false;

	//更新盘面
	UpdateBoard(&TrList1[nIndex++]);
	return true;
}

bool FetchTrList2(int &nIndex)
{
	if (nIndex != 0)
	{
		RestoreBoard(&TrList2[nIndex - 1]);
		RestoreBoard(&TrList1[TrList2[nIndex - 1].index]);
	}

	if (nIndex == TrCount2)
		return false;

	//更新盘面
	UpdateBoard(&TrList1[TrList2[nIndex].index]);
	UpdateBoard(&TrList2[nIndex++]);
	return true;
}

void UpdateBoard(TTrace *pTrace)
{
	board[pTrace->src] = pTrace->actor;
	board[pTrace->dest] = pTrace->death;
}

void RestoreBoard(TTrace *pTrace)
{
	board[pTrace->src] = 0;
	board[pTrace->dest] = pTrace->actor;
}

void AddTraceBlk(int kind, int x1, int y1, int x2, int y2, bool bBian, bool bPawnEat)
{
	if (x1 >= 8 || x1 < 0 || y1 >= 8 || y1 < 0)
		return;

	int src = x1 + (y1 << 3);
	int dest = x2 + (y2 << 3);
	if (board[src] == 0)
	{
		for (int i = 0; i < R_KING; i++)
		{
			int j = i;
			if (j != 0 && g_bLimitary == true)
				continue;
			if (kind == B_PAWN) //兵斜吃或直走的判断
			{
				if (j == 0 && bPawnEat || j != 0 && !bPawnEat)
					continue;
				if (j != 0 && y1 == 6 && y2 == 4) //原位兵
					continue;
				if (j == 0 && y1 == 6 && y2 == 4 && board[x1 + (5 << 3)] != 0)
					continue;
			}
			//更新盘面
			int nDestKind = board[dest];
			board[src] = bBian ? R_PAWN : kind;
			board[dest] = j;
			//要求黑方被将并且红方不被将
			if (InCheckBlk() && !InCheckRed())
			{
				TrList1[TrCount1].src   = src;
				TrList1[TrCount1].dest  = dest;
				TrList1[TrCount1].actor = bBian ? R_PAWN : kind;
				TrList1[TrCount1].death = j;
				TrCount1++;
			}
			//还原盘面
			board[dest] = nDestKind;
			board[src] = 0;
		}
	}
}

void TraceRookBlk(int x, int y)
{
	TraceBianBlk(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y) == 0; i++)
		AddTraceBlk(B_ROOK, x - i - 1, y, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y) == 0; i++)
		AddTraceBlk(B_ROOK, x + i + 1, y, x, y);
	for (i = 0; i < y && Board(x, y - i - 1) == 0; i++)
		AddTraceBlk(B_ROOK, x, y - i - 1, x, y);
	for (i = 0; i < 7 - y && Board(x, y + i + 1) == 0; i++)
		AddTraceBlk(B_ROOK, x, y + i + 1, x, y);
}

void TraceHorseBlk(int x, int y)
{
	TraceBianBlk(x, y);
	AddTraceBlk(B_HORSE, x + 1, y + 2, x, y);
	AddTraceBlk(B_HORSE, x + 2, y + 1, x, y);
	AddTraceBlk(B_HORSE, x + 2, y - 1, x, y);
	AddTraceBlk(B_HORSE, x + 1, y - 2, x, y);
	AddTraceBlk(B_HORSE, x - 1, y - 2, x, y);
	AddTraceBlk(B_HORSE, x - 2, y - 1, x, y);
	AddTraceBlk(B_HORSE, x - 2, y + 1, x, y);
	AddTraceBlk(B_HORSE, x - 1, y + 2, x, y);
}

void TraceBishopBlk(int x, int y)
{
	TraceBianBlk(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y - i - 1) == 0; i++)
		AddTraceBlk(B_BISHOP, x - i - 1, y - i - 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y  + i + 1) == 0; i++)
		AddTraceBlk(B_BISHOP, x + i + 1, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y + i + 1) == 0; i++)
		AddTraceBlk(B_BISHOP, x - i - 1, y + i + 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y - i - 1) == 0; i++)
		AddTraceBlk(B_BISHOP, x + i + 1, y - i - 1, x, y);
}

void TraceQueenBlk(int x, int y)
{
	TraceBianBlk(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y) == 0; i++)
		AddTraceBlk(B_QUEEN, x - i - 1, y, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y) == 0; i++)
		AddTraceBlk(B_QUEEN, x + i + 1, y, x, y);
	for (i = 0; i < y && Board(x, y - i - 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x, y - i - 1, x, y);
	for (i = 0; i < 7 - y && Board(x, y + i + 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y - i - 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x - i - 1, y - i - 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y  + i + 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x + i + 1, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y + i + 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x - i - 1, y + i + 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y - i - 1) == 0; i++)
		AddTraceBlk(B_QUEEN, x + i + 1, y - i - 1, x, y);
}

void TracePawnBlk(int x, int y)
{
	AddTraceBlk(B_PAWN, x,     y + 1, x, y, false, false);
	AddTraceBlk(B_PAWN, x + 1, y + 1, x, y, false, true);
	AddTraceBlk(B_PAWN, x - 1, y + 1, x, y, false, true);
	if (y == 4)
		AddTraceBlk(B_PAWN, x,     y + 2, x, y, false, false);
}

void TraceBianBlk(int x, int y)
{
	if (y == 0)
	{
		AddTraceBlk(B_PAWN, x,     y + 1, x, y, true, false);
		AddTraceBlk(B_PAWN, x + 1, y + 1, x, y, true, true);
		AddTraceBlk(B_PAWN, x - 1, y + 1, x, y, true, true);
	}
}

void TraceKingBlk(int x, int y)
{
	AddTraceBlk(B_KING, x + 1,     y, x, y);
	AddTraceBlk(B_KING, x - 1,     y, x, y);
	AddTraceBlk(B_KING, x,     y + 1, x, y);
	AddTraceBlk(B_KING, x,     y - 1, x, y);
	AddTraceBlk(B_KING, x + 1, y + 1, x, y);
	AddTraceBlk(B_KING, x + 1, y - 1, x, y);
	AddTraceBlk(B_KING, x - 1, y + 1, x, y);
	AddTraceBlk(B_KING, x - 1, y - 1, x, y);
}

void GenTrList1()
{
	static _TRACEFUNC TraceFuncTable[6] = {
		TraceRookBlk,
		TraceHorseBlk,
		TraceBishopBlk,
		TraceQueenBlk,
		TracePawnBlk,
		TraceKingBlk,
	};
	//得到黑方回退后的所有局面
	TrCount1 = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int kind = Board(i, j);
			if (kind >= B_ROOK)
				TraceFuncTable[kind - B_ROOK](i, j);
		}
	}
}

void AddTraceRed(int kind, int x1, int y1, int x2, int y2, bool bBian, bool bPawnEat)
{
	if (x1 >= 8 || x1 < 0 || y1 >= 8 || y1 < 0)
		return;

	int src = x1 + (y1 << 3);
	int dest = x2 + (y2 << 3);
	if (board[54] == 0 && board[55] == 12)
	{
		x1 = x1;
		if (src == 35 && dest == 37 || src == 37 && dest == 35)
			x1 = x1;
	}
	

	if (board[src] == 0)
	{
		for (int i = 0; i < R_KING; i++)
		{
			int j = i == 0 ? 0 : i + 6;
			if (j != 0 && g_bLimitary == true)
				continue;
			if (kind == R_PAWN) //兵斜吃或直走的判断
			{
				if (j == 0 && bPawnEat || j != 0 && !bPawnEat)
					continue;
				if (j != 0 && y1 == 1 && y2 == 3) //原位兵
					continue;
				if (j == 0 && y1 == 1 && y2 == 3 && board[x1 + (2 << 3)] != 0)
					continue;
			}
			//更新盘面
			int nDestKind = board[dest];
			board[src] = bBian ? B_PAWN : kind;
			board[dest] = j;
			//要求黑方不被将
			if (!InCheckBlk())
			{
				TrList2[TrCount2].src   = src;
				TrList2[TrCount2].dest  = dest;
				TrList2[TrCount2].actor = bBian ? B_PAWN : kind;
				TrList2[TrCount2].death = j;
				TrList2[TrCount2].index = TrIndex1;
				TrCount2++;
			}
			//还原盘面
			board[dest] = nDestKind;
			board[src] = 0;
		}
	}
}

void TraceRookRed(int x, int y)
{
	TraceBianRed(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y) == 0; i++)
		AddTraceRed(R_ROOK, x - i - 1, y, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y) == 0; i++)
		AddTraceRed(R_ROOK, x + i + 1, y, x, y);
	for (i = 0; i < y && Board(x, y - i - 1) == 0; i++)
		AddTraceRed(R_ROOK, x, y - i - 1, x, y);
	for (i = 0; i < 7 - y && Board(x, y + i + 1) == 0; i++)
		AddTraceRed(R_ROOK, x, y + i + 1, x, y);
}

void TraceHorseRed(int x, int y)
{
	TraceBianRed(x, y);
	AddTraceRed(R_HORSE, x + 1, y + 2, x, y);
	AddTraceRed(R_HORSE, x + 2, y + 1, x, y);
	AddTraceRed(R_HORSE, x + 2, y - 1, x, y);
	AddTraceRed(R_HORSE, x + 1, y - 2, x, y);
	AddTraceRed(R_HORSE, x - 1, y - 2, x, y);
	AddTraceRed(R_HORSE, x - 2, y - 1, x, y);
	AddTraceRed(R_HORSE, x - 2, y + 1, x, y);
	AddTraceRed(R_HORSE, x - 1, y + 2, x, y);
}

void TraceBishopRed(int x, int y)
{
	TraceBianRed(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y - i - 1) == 0; i++)
		AddTraceRed(R_BISHOP, x - i - 1, y - i - 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y  + i + 1) == 0; i++)
		AddTraceRed(R_BISHOP, x + i + 1, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y + i + 1) == 0; i++)
		AddTraceRed(R_BISHOP, x - i - 1, y + i + 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y - i - 1) == 0; i++)
		AddTraceRed(R_BISHOP, x + i + 1, y - i - 1, x, y);
}

void TraceQueenRed(int x, int y)
{
	TraceBianRed(x, y);
	int i;
	for (i = 0; i < x && Board(x - i - 1, y) == 0; i++)
		AddTraceRed(R_QUEEN, x - i - 1, y, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y) == 0; i++)
		AddTraceRed(R_QUEEN, x + i + 1, y, x, y);
	for (i = 0; i < y && Board(x, y - i - 1) == 0; i++)
		AddTraceRed(R_QUEEN, x, y - i - 1, x, y);
	for (i = 0; i < 7 - y && Board(x, y + i + 1) == 0; i++)
		AddTraceRed(R_QUEEN, x, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y - i - 1) == 0; i++)
		AddTraceRed(R_QUEEN, x - i - 1, y - i - 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y  + i + 1) == 0; i++)
		AddTraceRed(R_QUEEN, x + i + 1, y + i + 1, x, y);
	for (i = 0; i < x && Board(x - i - 1, y + i + 1) == 0; i++)
		AddTraceRed(R_QUEEN, x - i - 1, y + i + 1, x, y);
	for (i = 0; i < 7 - x && Board(x + i + 1, y - i - 1) == 0; i++)
		AddTraceRed(R_QUEEN, x + i + 1, y - i - 1, x, y);
}

void TracePawnRed(int x, int y)
{
	AddTraceRed(R_PAWN, x,     y - 1, x, y, false, false);
	AddTraceRed(R_PAWN, x + 1, y - 1, x, y, false, true);
	AddTraceRed(R_PAWN, x - 1, y - 1, x, y, false, true);
	if (y == 3)
		AddTraceRed(R_PAWN, x,     y - 2, x, y, false, false);
}

void TraceBianRed(int x, int y)
{
	if (y == 7)
	{
		AddTraceRed(R_PAWN, x,     y - 1, x, y, true, false);
		AddTraceRed(R_PAWN, x + 1, y - 1, x, y, true, true);
		AddTraceRed(R_PAWN, x - 1, y - 1, x, y, true, true);
	}
}

void TraceKingRed(int x, int y)
{
	AddTraceRed(R_KING, x + 1,     y, x, y);
	AddTraceRed(R_KING, x - 1,     y, x, y);
	AddTraceRed(R_KING, x,     y + 1, x, y);
	AddTraceRed(R_KING, x,     y - 1, x, y);
	AddTraceRed(R_KING, x + 1, y + 1, x, y);
	AddTraceRed(R_KING, x + 1, y - 1, x, y);
	AddTraceRed(R_KING, x - 1, y + 1, x, y);
	AddTraceRed(R_KING, x - 1, y - 1, x, y);
}

void GenTrList2()
{
	static _TRACEFUNC TraceFuncTable[6] = {
		TraceRookRed,
		TraceHorseRed,
		TraceBishopRed,
		TraceQueenRed,
		TracePawnRed,
		TraceKingRed,
	};
	//得到红方回退后的所有局面
	TrCount2 = 0;
	for (TrIndex1 = 0; TrIndex1 < TrCount1; TrIndex1++)
	{
		if (TrList1[TrIndex1].flag == false)
			continue;

		UpdateBoard(&TrList1[TrIndex1]);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				int kind = Board(i, j);
				if (kind >= R_ROOK && kind <= R_KING)
					TraceFuncTable[kind - R_ROOK](i, j);
			}
		}
		RestoreBoard(&TrList1[TrIndex1]);
	}
}
