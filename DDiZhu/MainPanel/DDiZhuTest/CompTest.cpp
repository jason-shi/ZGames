#include "StdAfx.h"
#include "CompTest.h"
#include "..\DDiZhuBoard\DDiZhuBoard.h"
#include "..\DDiZhuBoard\DDiZhuPlayer.h"
#include <boost\bind.hpp>

using namespace std;

#define SAME_RAND_SEED			20151031

CCompTest::CCompTest(CDDiZhuBoard *pDDiZhuBoard)
{
	m_pDDiZhuBoard = pDDiZhuBoard;
}

CCompTest::~CCompTest()
{
	m_pDDiZhuBoard = NULL;
}

bool CCompTest::Initial()
{
	return true;
}

void CCompTest::OnTestBegin()
{
	m_nProgram = 0;
	m_byChair = 0;
	m_nCaseNo = 0;
	memset(m_nResult, 0, sizeof(m_nResult));

	srand(SAME_RAND_SEED); //相同的时间种子

	m_pDDiZhuBoard->OnStartGame();
	m_pDDiZhuBoard->ShufflePoke();
	g_pZTimer->AddTimer(TM_COMP_TEST, 50, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
}

bool CCompTest::OnTestNext()
{
	if (++m_nCaseNo == 1)
	{
		m_nCaseNo = 0;
		if (++m_byChair == MAX_CHAIR)
		{
			m_byChair = 0;
			if (++m_nProgram == MAX_PROGRAM)
			{
				OnTestEnd();
				return false;
			}
			else
			{
				srand(SAME_RAND_SEED); //相同的时间种子
				g_pZTimer->AddTimer(TM_COMP_TEST, 50, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
				return true;
			}
		}
	}
	g_pZTimer->AddTimer(TM_COMP_TEST, 50, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
	return true;
}

void CCompTest::OnTestEnd()
{
	FILE *fp = _wfopen(L"CompTest.txt", L"a+, ccs=UTF-8");
	if (fp == NULL)
		return;

	TCHAR wcsBuff[64];
	//标题
	wsprintf(wcsBuff, L"%s", L"       庄家      上家      下家\n");
	fputws(wcsBuff, fp);

	//旧的结果
	wsprintf(wcsBuff, L"%s", L"  旧   ");
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d      ", m_nResult[0][0]);
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d      ", m_nResult[0][1]);
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d\n", m_nResult[0][2]);
	fputws(wcsBuff, fp);

	//新的结果
	wsprintf(wcsBuff, L"%s", L"  新   ");
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d      ", m_nResult[1][0]);
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d      ", m_nResult[1][1]);
	fputws(wcsBuff, fp);
	wsprintf(wcsBuff, L"%4d\n", m_nResult[1][2]);
	fputws(wcsBuff, fp);

	fclose(fp);
	fp = NULL;
}

void CCompTest::DoDiZhu()
{
	m_pDDiZhuBoard->OnDiZhu(0);
	g_pZTimer->AddTimer(TM_COMP_TEST, 100, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
}

void CCompTest::DoPutCard()
{
	if (m_pDDiZhuBoard->CheckGameOver())
	{
		byte byDiZhu = m_pDDiZhuBoard->GetDiZhu();
		byte byChair = m_pDDiZhuBoard->GetChair();
		byte byChair1 = m_pDDiZhuBoard->GetChair1();
		if ((byDiZhu == byChair) == (byDiZhu == byChair1))
			m_nResult[m_nProgram][byChair]++;

		if (OnTestNext())
		{
			m_pDDiZhuBoard->OnStartGame();
			m_pDDiZhuBoard->ShufflePoke();
			g_pZTimer->AddTimer(TM_COMP_TEST, 100, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
		}
		else
		{
			OnTestEnd();
		}
	}
	else
	{
		m_pDDiZhuBoard->OnTrust(NULL, 0, CPoint(0, 0));
		g_pZTimer->AddTimer(TM_COMP_TEST, 100, boost::bind(&CCompTest::OnTimer, this, _1), NULL);
	}
}

void CCompTest::OnTimer(int nTimerId)
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
