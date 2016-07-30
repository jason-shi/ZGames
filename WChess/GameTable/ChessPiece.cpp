#include "StdAfx.h"
#include "ChessPiece.h"

//∆Â◊”…¡À∏∂Øª≠
CFlickAnime WChessPiece::AnimeFlick(0, 600, 400);

WChessPiece::WChessPiece(CPoint xDrawPos, int nGrid, int nKind)
				: m_nGrid(nGrid), m_nKind(nKind)
{
	static CRect xRect(0, 0, 36, 36);
	Set(&ImagePiece, xRect + xDrawPos, xRect + CPoint(36 * nKind, 0), 0);
}

WChessPiece::~WChessPiece()
{
}

void WChessPiece::OnSelect()
{
	AnimeFlick.Attach(this);
	AnimeFlick.Play();
}

void WChessPiece::OnUnSelect()
{
	AnimeFlick.Stop();
	Show();
}
