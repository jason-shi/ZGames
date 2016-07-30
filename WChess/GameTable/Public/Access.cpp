#include "StdAfx.h"
#include "Access.h"
#include "ChessBoard.h"

#define MaxLength			 1024

bool LoadByFen(TCHAR *fen)
{
	InitBoard();
	g_pChessBoard->ClearFixed();
	//从棋盘的左上开始
	int x = 0, y = 7;
	int i = 0;
	while (TCHAR ch = fen[i++])
	{
		if (x > 8 || y < 0)
			return false;
		if (ch == L'/') //新的一行
			x = 0, y--;
		else if (TblCharToPiece[(byte)ch] != 0) //棋子
			board[x++ + (y << 3)] = TblCharToPiece[(byte)ch];
		else if (ch >= L'1' && ch <= L'8') //跳过
			x += ch - L'0';
		else if (ch == L'_' && x > 0) //固定
			g_pChessBoard->SetFixed(x - 1 + (y << 3));
		else if (ch == L' ')
			return true;
		else
			return false;
	}
	return true;
}

void WritePgnHeader(FILE *fp)
{
	fputws(L"[Game \"Chess\"]\n", fp);
	fputws(L"[Event \"\"]\n", fp);
	fputws(L"[Round \"\"]\n", fp);
	fputws(L"[Date \"\"]\n", fp);
	fputws(L"[Site \"\"]\n", fp);
	fputws(L"[RedTeam \"\"]\n", fp);
	fputws(L"[Red \"\"]\n", fp);
	fputws(L"[BlackTeam \"\"]\n", fp);
	fputws(L"[Black \"\"]\n", fp);
	fputws(L"[Result \"\"]\n", fp);
	fputws(L"[ECCO \"\"]\n", fp);
	fputws(L"[Opening \"\"]\n", fp);
	fputws(L"[Variation \"\"]\n", fp);
	fputws(L"[FEN \"", fp);
}

void SaveToFen(TCHAR *fen)
{
	//从棋盘的左上开始
	for (int y = 7; y >= 0; y--)
	{
		int skip = 0;
		for (int x = 0; x < 8; x++)
		{
			int kind = Board(x, y);
			if (kind == 0)
				skip++;
			else
			{
				if (skip != 0)
				{
					*fen++ = skip + L'0';
					skip = 0;
				}
				if (TblPieceToChar[kind])
					*fen++ = TblPieceToChar[kind];
				if (g_pChessBoard->IsFixed(x + (y << 3)))
					*fen++ = L'_';
			}
		}
		//每行结束
		if (skip != 0)
			*fen++ = skip + L'0';
		if (y != 0)
			*fen++ = L'/';
	}
	*fen++ = L' ';
	*fen++ = L'\0';
}

void SetVirginFen(TCHAR* str)
{
	wsprintf(str, L"%s", L"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR r - - 0 1");
}

bool LoadByPgn(const TCHAR* wcsFileName)
{
	FILE *fp;
	if ((fp = _wfopen(wcsFileName, L"r, ccs=UTF-8")) == NULL)
		return false;
	//查找文件中的FEN串
	TCHAR wcsBuff[MaxLength];
	while (fgetws(wcsBuff, MaxLength, fp))
	{
		if (wcsBuff[1] == L'F' && wcsBuff[2] == L'E' && wcsBuff[3] == L'N')
		{
			fclose(fp);
			TCHAR *fen = wcsBuff + 4;
			while (TCHAR ch = *fen++)
			{
				//PGN中的FEN以引号开始
				if (ch == '"')
					return LoadByFen(fen);
			}
			return false;
		}
	}
	fclose(fp);
	return false;
}

bool SaveToPgn(const TCHAR* wcsFileName)
{
	//盘面存储为FEN串
	TCHAR wcsBuff[MaxLength];
	SaveToFen(wcsBuff);

	FILE *fp;
	if ((fp = _wfopen(wcsFileName, L"w+, ccs=UTF-8")) == NULL)
		return false;
	WritePgnHeader(fp);
	fputws(wcsBuff, fp);
	fputws(L"r - - 0 1\"]", fp);
	fclose(fp);
	return true;
}
