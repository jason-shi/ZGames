#include "StdAfx.h"
#include "DDiZhuPlayer.h"
#include "Board\Card.h"
#include "Board\Logic.h"

using namespace std;

CDDiZhuPlayer::CDDiZhuPlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair)
					: CPlayer(pImage, xDrawPos, xSize, byChair)
{
}

CDDiZhuPlayer::~CDDiZhuPlayer()
{
}

bool CDDiZhuPlayer::Initial()
{
	if (!CPlayer::Initial())
		return false;

	return true;
}

void CDDiZhuPlayer::OnDiZhu(VECCARD& vecDiCard)
{
	VECCARD::iterator iter;
	for (iter = vecDiCard.begin(); iter != vecDiCard.end(); iter++)
		OnSendCard(*iter);

	SortCards();
	ShowCards();
	m_tHandBmp.OnInit(m_vecCard);
	UpdateAlive();

	UpdateStatus(TYPE_GVP, GRADE_INIT, 0, 0);
}
