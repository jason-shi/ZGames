#include "StdAfx.h"
#include "Table.h"

int					 TblPieceBitmap[64];
byte				 TblCharToPiece[256];
char				 TblPieceToChar[15];

void CreateTblPieceBitmap()
{
	int RHBQKMask = (1 << R_ROOK) | (1 << R_HORSE) | (1 << R_BISHOP) | (1 << R_QUEEN) | (1 << R_KING)
				  | (1 << B_ROOK) | (1 << B_HORSE) | (1 << B_BISHOP) | (1 << B_QUEEN) | (1 << B_KING);

	memset(TblPieceBitmap, 0, sizeof(TblPieceBitmap));
	for (int i = 0; i < 64; i++)
	{
		if ((i >> 3) != 0 && (i >> 3) != 7)
		{
			TblPieceBitmap[i] |= 1 << R_PAWN;
			TblPieceBitmap[i] |= 1 << B_PAWN;
		}
		TblPieceBitmap[i] |= RHBQKMask; //车马相后王
	}
}

void CreateTblCharToPiece()
{
	memset(TblCharToPiece, 0, sizeof(TblCharToPiece));
	TblCharToPiece['R'] = R_ROOK;
	TblCharToPiece['N'] = R_HORSE;
	TblCharToPiece['B'] = R_BISHOP;
	TblCharToPiece['Q'] = R_QUEEN;
	TblCharToPiece['P'] = R_PAWN;
	TblCharToPiece['K'] = R_KING;
	TblCharToPiece['r'] = B_ROOK;
	TblCharToPiece['n'] = B_HORSE;
	TblCharToPiece['b'] = B_BISHOP;
	TblCharToPiece['q'] = B_QUEEN;
	TblCharToPiece['p'] = B_PAWN;
	TblCharToPiece['k'] = B_KING;
}

void CreateTblPieceToChar()
{
	TblPieceToChar[0] = 0;
	TblPieceToChar[R_ROOK]	 = 'R';
	TblPieceToChar[R_HORSE]	 = 'N';
	TblPieceToChar[R_BISHOP] = 'B';
	TblPieceToChar[R_QUEEN]  = 'Q';
	TblPieceToChar[R_PAWN]	 = 'P';
	TblPieceToChar[R_KING]	 = 'K';
	TblPieceToChar[B_ROOK]	 = 'r';
	TblPieceToChar[B_HORSE]	 = 'n';
	TblPieceToChar[B_BISHOP] = 'b';
	TblPieceToChar[B_QUEEN]  = 'q';
	TblPieceToChar[B_PAWN]	 = 'p';
	TblPieceToChar[B_KING]	 = 'k';
}

void CreateAllTable()
{
	CreateTblPieceBitmap();
	CreateTblCharToPiece();
	CreateTblPieceToChar();
}
