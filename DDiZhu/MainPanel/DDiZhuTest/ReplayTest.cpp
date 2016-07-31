#include "StdAfx.h"
#include "ReplayTest.h"
#include "..\DDiZhuBoard\DDiZhuBoard.h"
#include "..\DDiZhuBoard\DDiZhuReplay.h"
#include <boost\bind.hpp>

using namespace std;

CReplayTest::CReplayTest(CDDiZhuBoard *pDDiZhuBoard)
{
	m_pDDiZhuBoard = pDDiZhuBoard;
	m_pDDiZhuReplay = NULL;
}

CReplayTest::~CReplayTest()
{
	delete m_pDDiZhuReplay;
	m_pDDiZhuReplay = NULL;
}

CDDiZhuReplay *CReplayTest::CreateReplay()
{
	CDDiZhuReplay *pDDiZhuReplay = new CDDiZhuReplay();
	if (!pDDiZhuReplay)
		return NULL;

	return pDDiZhuReplay;
}

bool CReplayTest::Initial()
{
	m_pDDiZhuReplay = CreateReplay();
	if (!m_pDDiZhuReplay)
		return false;

	return true;
}

bool CReplayTest::OpenReplay(TCHAR wcsFileName[_MAX_PATH])
{
	return m_pDDiZhuReplay->Open(wcsFileName);
}

void CReplayTest::CloseReplay()
{
	m_pDDiZhuReplay->Close();
}

void CReplayTest::ReadReplay(byte byType)
{
	switch (byType)
	{
	case RECORD_DIZHU:
		DoDiZhu();
		break;
	case RECORD_PUT:
		DoPutCard();
		break;
	case RECORD_GVP:
		DoGvp();
		break;
	case RECORD_UNDO:
		DoUndo();
		break;
	}
}

void CReplayTest::DoDiZhu()
{
	byte byDiZhu;

	m_pDDiZhuReplay->ReadDiZhu(byDiZhu);
	m_pDDiZhuBoard->OnDiZhu(byDiZhu);
}

void CReplayTest::DoPutCard()
{
	TPutReplay tPutReplay;

	m_pDDiZhuReplay->ReadPut(&tPutReplay);
	m_pDDiZhuBoard->OnPut(tPutReplay);
}

void CReplayTest::DoGvp()
{
	m_pDDiZhuReplay->ReadGvp();
	m_pDDiZhuBoard->OnGvp(NULL, 0, CPoint(0, 0));
}

void CReplayTest::DoUndo()
{
	m_pDDiZhuReplay->ReadUndo();
	m_pDDiZhuBoard->OnUndo(NULL, 0, CPoint(0, 0));
}

void CReplayTest::OnTimer(int nTimerId)
{
	g_pZTimer->DelTimer(nTimerId);

	byte byType;
	if (!m_pDDiZhuReplay->ReadType(byType))
	{
		CloseReplay();
		return;
	}

	ReadReplay(byType);
	g_pZTimer->AddTimer(TM_REPLAY, 100, boost::bind(&CReplayTest::OnTimer, this, _1), NULL);
}
