#include "StdAfx.h"
#include "LightImage.h"

CLightImage::CLightImage()
{
}

CLightImage::CLightImage(int nWidth, int nHeight) : CImage(nWidth, nHeight)
{
}

CLightImage::~CLightImage()
{
}

void CLightImage::FastLineH(int x1, int x2, int y, COLORREF color)
{
	int *p = (int *)GetBits(min(x1, x2), y);
	int *limit = (int *)GetBits(max(x1, x2), y);
	while (p <= limit)
	{
		//°üÀ¨¶Ëµã
		*p++ |= color;
	}
}

void CLightImage::FastLineL(int x1, int y1, int x2, int y2, COLORREF color)
{
	int *p = (int *)GetBits(min(x1, x2), max(y1, y2));
	int *limit = (int *)GetBits(max(x1, x2), min(y1, y2));
	while (p <= limit)
	{
		*p++ |= color;
		p += W;
	}
}

void CLightImage::FastLineV(int x, int y1, int y2, COLORREF color)
{
	int *p = (int *)GetBits(x, max(y1, y2));
	int *limit = (int *)GetBits(x, min(y1, y2));
	while (p <= limit)
	{
		*p |= color;
		p += W;
	}
}

void CLightImage::FastLineB(int x1, int y1, int x2, int y2, COLORREF color)
{
	int *p = (int *)GetBits(max(x1, x2), max(y1, y2));
	int *limit = (int *)GetBits(min(x1, x2), min(y1, y2));
	while (p <= limit)
	{
		*p-- |= color;
		p += W;
	}
}
