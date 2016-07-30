#include "StdAfx.h"
#include "Pattern.h"
#include "ChessBoard.h"

#define MAX_SWAPTIMES	 100 //交换尝试次数

const int TKind[32] = { 1, 1, 2, 2, 3, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6,
						7, 7, 8, 8, 9, 9, 10, 11, 11, 11, 11, 11, 11, 11, 11, 12, };

const int TForce[13] = { 0, 4, 3, 2, 5, 1, 0, -4, -3, -2, -5, -1, 0 };

static int			 TotalSwapPos;
static int			 TotalSwapPiece;

static int			 SwapPos[32];
static TRemainPiece	 RemainPiece[32];

void InitSwapRemain()
{
	for (int i = 0; i < 32; i++)
	{
		RemainPiece[i].kind = TKind[i];
		RemainPiece[i].force = TForce[TKind[i]];
	}
	TotalSwapPos = 0;
	TotalSwapPiece = 32;
	//去除盘面已经使用的棋子
	for (int i = 0; i < 64; i++)
	{
		if (board[i] == 0)
			continue;
		for (int j = 0; j < TotalSwapPiece; j++)
		{
			if (RemainPiece[j].kind == board[i])
			{
				RemainPiece[j].kind = RemainPiece[TotalSwapPiece - 1].kind;
				RemainPiece[j].force = RemainPiece[TotalSwapPiece - 1].force;
				break;
			}
		}
		if (!g_pChessBoard->IsFixed(i) && board[i] != R_KING && board[i] != B_KING)
			SwapPos[TotalSwapPos++] = i;
		TotalSwapPiece--;
	}
}

void ChangeOffensive(bool bWeaken)
{
	int nPos, nPiece, nCount = 0;
	int kind, force;

	//没有可用棋子则进行鲁棒处理
	if (TotalSwapPiece == 0)
	{
		RobustSwap();
		return;
	}
	//根据当前盘面强弱进行变化
	while (nCount++ < MAX_SWAPTIMES)
	{
		//获取随机可交换位置
		nPos = SwapPos[rand() % TotalSwapPos];
		kind = board[nPos];
		force = TForce[kind];
		//获取随机可用棋子
		nPiece = rand() % TotalSwapPiece;

		if (bWeaken && RemainPiece[nPiece].force < force
			|| !bWeaken && RemainPiece[nPiece].force > force)
		{
			board[nPos] = RemainPiece[nPiece].kind;
			if (IsBothLegal())
			{
				//继续交换
				RemainPiece[nPiece].kind = kind;
				RemainPiece[nPiece].force = force;
				return;
			}
			//交换失败进行还原
			board[nPos] = kind;
		}
	}
	//超过最大尝试次数则进行鲁棒处理
	RobustSwap();
}

void RobustSwap()
{
	int nPos1, nPos2;
	while (true)
	{
		do
		{
			nPos1 = SwapPos[rand() % TotalSwapPos];
			nPos2 = SwapPos[rand() % TotalSwapPos];
		} while (nPos2 == nPos1 || board[nPos2] == board[nPos1]);
		//交换两个位置的棋子
		int temp = board[nPos1];
		board[nPos1] = board[nPos2];
		board[nPos2] = temp;

		if (IsBothLegal())
			return;

		//还原两个位置的棋子
		temp = board[nPos1];
		board[nPos1] = board[nPos2];
		board[nPos2] = temp;
	}
}
