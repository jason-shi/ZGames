#include "StdAfx.h"
#include "DDiZhuBoard.h"
#include "DDiZhuPlayer.h"
#include "DDiZhuRecord.h"
#include "DDiZhuOperPanel.h"
#include "DiZhuPanel.h"
#include "Board\Poke.h"
#include <boost\bind.hpp>

#define SAME_RAND_SEED			20151031

CDDiZhuBoard::CDDiZhuBoard(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CBoard(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pDiZhuPanel = NULL;
}

CDDiZhuBoard::~CDDiZhuBoard()
{
	delete m_pDiZhuPanel;
	m_pDiZhuPanel = NULL;
}

CPlayer *CDDiZhuBoard::CreatePlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair)
{
	CDDiZhuPlayer *pDDiZhuPlayer = new CDDiZhuPlayer(pImage, xDrawPos, xSize, byChair);
	if (!pDDiZhuPlayer || !pDDiZhuPlayer->Initial())
		return NULL;
	AddObject(pDDiZhuPlayer);

	return pDDiZhuPlayer;
}

CRecord *CDDiZhuBoard::CreateRecord()
{
	CDDiZhuRecord *pDDiZhuRecord = new CDDiZhuRecord(L"Replay.dat");
	if (!pDDiZhuRecord)
		return NULL;

	return pDDiZhuRecord;
}

COperPanel *CDDiZhuBoard::CreateOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	CDDiZhuOperPanel *pDDiZhuOperPanel = new CDDiZhuOperPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth);
	pDDiZhuOperPanel->Show(false);
	if (!pDDiZhuOperPanel->Initial(this))
		return NULL;
	AddObject(pDDiZhuOperPanel);

	return pDDiZhuOperPanel;
}

bool CDDiZhuBoard::Initial()
{
	if (!CBoard::Initial())
		return false;

	m_pDiZhuPanel = new CDiZhuPanel(NULL, CPoint(420, 0), CPoint(0, 0), CSize(300, 148), 0);
	m_pDiZhuPanel->Show(false);
	if (!m_pDiZhuPanel->Initial(this))
		return false;
	AddObject(m_pDiZhuPanel);
	return true;
}

bool CDDiZhuBoard::OnStartGame()
{
	if (!CBoard::OnStartGame())
		return false;

	m_pDiZhuPanel->OnStartGame();
	return true;
}

void CDDiZhuBoard::SendCards()
{
	for (int i = 0; i < 54; i++)
	{
		if (m_pPoke->GetSize() <= 3)
			break;

		CCard *pCard = m_pPoke->GetCard();
		m_pPlayer[(i + m_byChair) % 3]->OnSendCard(pCard);
	}
}

void CDDiZhuBoard::SendDiCards()
{
	for (int i = 0; i < 3; i++)
	{
		CCard *pCard = m_pPoke->GetCard();
		m_pDiZhuPanel->OnSendCard(i, pCard);
	}
}

void CDDiZhuBoard::OnSendCardsOver()
{
	CBoard::OnSendCardsOver();

	m_pDiZhuPanel->Show();
}

void CDDiZhuBoard::CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	DDiZhu_CallCrack(pCrackParam, pCrackResult);
}

void CDDiZhuBoard::OnDiZhu(CObject *pObject, UINT nFlags, CPoint point)
{
	int nObjId = pObject->GetObjId();
	OnDiZhu(nObjId);
}

void CDDiZhuBoard::OnDiZhu(int nObjId)
{
	m_byDiZhu = nObjId;

	//·¢µ×ÅÆ
	GetPlayer(m_byDiZhu)->OnDiZhu(m_pDiZhuPanel->GetDiCards());

	m_pDiZhuPanel->OnDiZhu(m_byDiZhu);
	((CDDiZhuOperPanel *)m_pOperPanel)->OnDiZhu(m_byDiZhu);

	//±£´æÂ¼Ïñ
	SaveRecord(RECORD_DIZHU);

	m_byStage = STAGE_PUTCARD;
	m_byChair = m_byDiZhu;
	m_byChair1 = m_byDiZhu;
}

void CDDiZhuBoard::OnPut(TPutReplay &tPutReplay)
{
	m_byChair   = tPutReplay.byChair;
	m_byRspType = tPutReplay.byRspType;
	m_byGrade   = tPutReplay.byGrade;
	m_byCount   = tPutReplay.byCount;

	CBoard::OnPut(NULL, 0, CPoint(0, 0));
}

void CDDiZhuBoard::OnTimer(WPARAM wParam)
{
}

void CDDiZhuBoard::SaveRecord(byte byType)
{
	CDDiZhuRecord *pDDiZhuRecord = (CDDiZhuRecord *)m_pRecord;
	switch (byType)
	{
	case RECORD_DIZHU:
		pDDiZhuRecord->WriteDiZhu(m_byDiZhu);
		break;
	default:
		return CBoard::SaveRecord(byType);
	}
}
