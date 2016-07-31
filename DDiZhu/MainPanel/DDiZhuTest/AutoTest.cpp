#include "StdAfx.h"
#include "AutoTest.h"
#include "..\DDiZhuBoard\DDiZhuBoard.h"
#include "..\DDiZhuBoard\DDiZhuPlayer.h"
#include <boost\bind.hpp>

using namespace std;

static TScoreParam	 g_tScoreParam;
static TScoreResult	 g_tScoreResult;

CAutoTest::CAutoTest(CDDiZhuBoard *pDDiZhuBoard)
{
	m_pDDiZhuBoard = pDDiZhuBoard;
}

CAutoTest::~CAutoTest()
{
}

bool CAutoTest::Initial()
{
	return true;
}

void CAutoTest::OnTestBegin()
{
#ifndef _DEBUG
	srand((unsigned int)time(NULL));
#endif
	m_pDDiZhuBoard->OnStartGame();
	m_pDDiZhuBoard->ShufflePoke();
	g_pZTimer->AddTimer(TM_AUTO_TEST, 100, boost::bind(&CAutoTest::OnTimer, this, _1), NULL);
}

void CAutoTest::OnTestEnd()
{
	g_pZTimer->DelTimer(TM_AUTO_TEST);
}

void CAutoTest::DoDiZhu()
{
	byte byLastScore = 0; //从0分开始

	byte byBanker;
	for (byte byChair = 0; byChair < 3; byChair++)
	{
		CDDiZhuPlayer *pPlayer = m_pDDiZhuBoard->GetPlayer(byChair);

		//调用AI：叫分
		pPlayer->GetHandBmp(g_tScoreParam.bitmap); //无主
		g_tScoreParam.byLastScore = byLastScore;
		DDiZhu_CallScore(&g_tScoreParam, &g_tScoreResult);

		//最后的叫分
		if (g_tScoreResult.byBidScore != 0)
		{
			byLastScore = g_tScoreResult.byBidScore;
			byBanker = byChair;
		}
	}

	//如果没有人叫分，则随机决定庄家
	if (byLastScore == 0)
		byBanker = rand() % 3;

	m_pDDiZhuBoard->OnDiZhu(byBanker);
	g_pZTimer->AddTimer(TM_AUTO_TEST, 100, boost::bind(&CAutoTest::OnTimer, this, _1), NULL);
}

void CAutoTest::DoPutCard()
{
	if (m_pDDiZhuBoard->CheckGameOver())
	{
		m_pDDiZhuBoard->OnStartGame();
		m_pDDiZhuBoard->ShufflePoke();
		g_pZTimer->AddTimer(TM_AUTO_TEST, 100, boost::bind(&CAutoTest::OnTimer, this, _1), NULL);
	}
	else
	{
		m_pDDiZhuBoard->OnTrust(NULL, 0, CPoint(0, 0));
		g_pZTimer->AddTimer(TM_AUTO_TEST, 100, boost::bind(&CAutoTest::OnTimer, this, _1), NULL);
	}
}

void CAutoTest::OnTimer(int nTimerId)
{
	g_pZTimer->DelTimer(nTimerId);

	switch (m_pDDiZhuBoard->GetStage())
	{
	case STAGE_DIZHU:
		DoDiZhu();
		break;
	case STAGE_PUTCARD:
		DoPutCard();
		break;
	}
}
