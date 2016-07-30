#include "StdAfx.h"
#include "Light.h"
#include "LightImage.h"

#define FMUL24(n)					(((n) << 4) + ((n) << 3)) //¿ì³Ë24

CLight::CLight() : CSprite()
{
	m_pImageLight = NULL;
	Set(NULL, CPoint(0, 0), CPoint(0, 0), CSize(361, 361), 2);
}

CLight::~CLight()
{
	delete m_pImageLight;
	m_pImageLight = NULL;
}

bool CLight::Initial()
{
	if (!CSprite::Initial())
		return false;

	m_pImageLight = new CLightImage(361, 361);
	m_pImageLight->Clear();
	SetImage(m_pImageLight);

	return true;
}

void CLight::DrawLight(int nBmpLight[256])
{
	static CPoint TBeamEnd[8] = {
			CPoint(0,    12),
			CPoint(-12,  12),
			CPoint(-12,   0),
			CPoint(-12, -12),
			CPoint(0,   -12),
			CPoint(12,  -12),
			CPoint(12,    0),
			CPoint(12,   12),
	};
	static int TEccentX[8] = { 0, -1, -1, -1,  0,  1, 1, 1, };
	static int TEccentY[8] = { 1,  1,  0, -1, -1, -1, 0, 1, };

	m_pImageLight->Clear();
	for (int nGrid = 0; nGrid < 256; nGrid++)
	{
		int nBitmap = nBmpLight[nGrid];
		for (int dir = 0; dir < 8; dir++)
		{
			int color = nBitmap & 7;
			nBitmap >>= 3;
			if (color == 0)
				continue;

			int x1 = FMUL24(nGrid & 15) + 12;
			int y1 = FMUL24(nGrid >> 4) + 12;
			int nNextGrid = TblNextGrid[(nGrid << 3) + dir];
			if (nNextGrid == 255)
			{
				int x2 = x1 + TBeamEnd[dir].x;
				int y2 = y1 + TBeamEnd[dir].y;
				DrawLightSpan(dir, x1 + TEccentX[dir], y1 + TEccentY[dir], x2, y2, color);
			}
			else
			{
				int x2 = FMUL24(nNextGrid & 15) + 12;
				int y2 = FMUL24(nNextGrid >> 4) + 12;
				DrawLightSpan(dir, x1 + TEccentX[dir], y1 + TEccentY[dir], x2, y2, color);
			}
		}
	}
	g_pZ2coo->Redraw(this);
}

void CLight::DrawLightSpan(int dir, int x1, int y1, int x2, int y2, int color)
{
	static COLORREF TColor[8] = {
			BGRA(0,     0,   0,   0),
			BGRA(255,   0,   0, 255),
			BGRA(0,   255,   0, 255),
			BGRA(255, 255,   0, 255),
			BGRA(0,     0, 255, 255),
			BGRA(255,   0, 255, 255),
			BGRA(0,   255, 255, 255),
			BGRA(255, 255, 255, 255),
	};

	switch (dir)
	{
	case 0:
	case 4:
		m_pImageLight->FastLineV(x1, y1, y2, TColor[color]);
		break;
	case 1:
	case 5:
		m_pImageLight->FastLineL(x1, y1, x2, y2, TColor[color]);
		break;
	case 2:
	case 6:
		m_pImageLight->FastLineH(x1, x2, y1, TColor[color]);
		break;
	case 3:
	case 7:
		m_pImageLight->FastLineB(x1, y1, x2, y2, TColor[color]);
		break;
	}
}
