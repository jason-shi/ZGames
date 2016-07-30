#include "StdAfx.h"
#include "WCommon.h"

TCHAR				 ModulePath[_MAX_PATH];

CImage				 ImagePiece;

bool LoadImage()
{
	return ImagePiece.LoadImage(L"PNG\\Piece.png");
}
