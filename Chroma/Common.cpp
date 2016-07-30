#include "StdAfx.h"
#include "Common.h"

bool				 g_bEditState = false;

CImage				 ImageBack;
CImage				 ImageMirror;
CImage				 ImageChoice;
CImage				 ImageGate;

bool LoadImage()
{
	return ImageBack.LoadImage(L"PNG\\Back.png")
		&& ImageMirror.LoadImage(L"PNG\\Mirror.png")
		&& ImageChoice.LoadImage(L"PNG\\Choice.png")
		&& ImageGate.LoadImage(L"PNG\\Gate.png");
}
