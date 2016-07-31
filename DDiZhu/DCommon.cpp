#include "StdAfx.h"
#include "Common.h"

CImage				 ImageBack;
CImage				 ImagePoke;
CImage				 ImageDDiZhu;

bool LoadImage()
{
	return ImageBack.LoadImage(L"PNG\\Back.png")
		&& ImagePoke.LoadImage(L"PNG\\SmallPoke.png")
		&& ImageDDiZhu.LoadImage(L"PNG\\DDiZhu.png");
}
