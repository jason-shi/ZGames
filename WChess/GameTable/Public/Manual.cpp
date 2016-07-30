#include "StdAfx.h"
#include "Manual.h"

using namespace std;

vector<wstring>		 vecManual;


//调用红方棋谱入口
void GetManualRed(TCHAR *szBuff, int x1, int y1, int x2, int y2, int bian)
{
	static TCHAR T[4] = { L'R', L'H', L'B', L'Q', };

	wsprintf(szBuff, L"%c%c%c%c", L'A' + x1, L'1' + y1, L'A' + x2, L'1' + y2);
	if (bian != 0)
		wsprintf(szBuff, L"%s%c", szBuff, T[bian - R_ROOK]);
}

//调用黑方棋谱入口
void GetManualBlk(TCHAR *szBuff, int x1, int y1, int x2, int y2, int bian)
{
	static TCHAR T[4] = { L'r', L'h', L'b', L'q', };

	wsprintf(szBuff, L"%c%c%c%c", L'a' + x1, L'1' + y1, L'a' + x2, L'1' + y2);
	if (bian != 0)
		wsprintf(szBuff, L"%s%c", szBuff, T[bian - B_ROOK]);
}

void SaveManual(int src, int dest, int bian)
{
	int x1 = src & 7;
	int y1 = src >> 3;
	int x2 = dest & 7;
	int y2 = dest >> 3;

	//获得棋谱
	TCHAR szBuff[6];
	if ((vecManual.size() & 1) == 0)
		GetManualRed(szBuff, x1, y1, x2, y2, bian);
	else
		GetManualBlk(szBuff, x1, y1, x2, y2, bian);

	vecManual.push_back(szBuff);
}
