#include "StdAfx.h"
#include "Pattern.h"
#include "ChessBoard.h"

#define MAX_SWAPTIMES	 100 //�������Դ���

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
	//ȥ�������Ѿ�ʹ�õ�����
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

	//û�п������������³������
	if (TotalSwapPiece == 0)
	{
		RobustSwap();
		return;
	}
	//���ݵ�ǰ����ǿ�����б仯
	while (nCount++ < MAX_SWAPTIMES)
	{
		//��ȡ����ɽ���λ��
		nPos = SwapPos[rand() % TotalSwapPos];
		kind = board[nPos];
		force = TForce[kind];
		//��ȡ�����������
		nPiece = rand() % TotalSwapPiece;

		if (bWeaken && RemainPiece[nPiece].force < force
			|| !bWeaken && RemainPiece[nPiece].force > force)
		{
			board[nPos] = RemainPiece[nPiece].kind;
			if (IsBothLegal())
			{
				//��������
				RemainPiece[nPiece].kind = kind;
				RemainPiece[nPiece].force = force;
				return;
			}
			//����ʧ�ܽ��л�ԭ
			board[nPos] = kind;
		}
	}
	//��������Դ��������³������
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
		//��������λ�õ�����
		int temp = board[nPos1];
		board[nPos1] = board[nPos2];
		board[nPos2] = temp;

		if (IsBothLegal())
			return;

		//��ԭ����λ�õ�����
		temp = board[nPos1];
		board[nPos1] = board[nPos2];
		board[nPos2] = temp;
	}
}
