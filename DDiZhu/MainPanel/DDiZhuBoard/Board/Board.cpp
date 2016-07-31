#include "StdAfx.h"
#include "Board.h"
#include "Poke.h"
#include "Player.h"
#include "Record.h"
#include "OperPanel.h"
#include "Logic.h"
#include <boost\bind.hpp>

using namespace std;

CBoard::CBoard(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pPoke = NULL;

	for (int i = 0; i < 3; i++)
		m_pPlayer[i] = NULL;

	m_pRecord = NULL;
	m_pOperPanel = NULL;

	m_byStage = STAGE_START;
}

CBoard::~CBoard()
{
	delete m_pPoke;
	m_pPoke = NULL;

	for (int i = 0; i < 3; i++)
	{
		delete m_pPlayer[i];
		m_pPlayer[i] = NULL;
	}

	delete m_pRecord;
	m_pRecord = NULL;

	delete m_pOperPanel;
	m_pOperPanel = NULL;
}

CPoke *CBoard::CreatePoke()
{
	CPoke *pPoke = new CPoke();
	if (!pPoke || !pPoke->Initial())
		return NULL;

	return pPoke;
}

CPlayer *CBoard::CreatePlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair)
{
	CPlayer *pPlayer = new CPlayer(pImage, xDrawPos, xSize, byChair);
	if (!pPlayer || !pPlayer->Initial())
		return NULL;
	AddObject(pPlayer);

	return pPlayer;
}

CRecord *CBoard::CreateRecord()
{
	CRecord *pRecord = new CRecord(L"Replay.dat");
	if (!pRecord)
		return NULL;

	return pRecord;
}

COperPanel *CBoard::CreateOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
{
	COperPanel *pOperPanel = new COperPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth);
	pOperPanel->Show(false);
	if (!pOperPanel->Initial(this))
		return NULL;
	AddObject(pOperPanel);

	return pOperPanel;
}

bool CBoard::Initial()
{
	if (!CPanel::Initial())
		return false;

	//创建扑克
	m_pPoke = CreatePoke();
	if (!m_pPoke)
		return false;

	//创建手牌面板
	for (int i = 0; i < 3; i++)
	{
		if (!(m_pPlayer[i] = CreatePlayer(NULL, CPoint(0, (CardHeight + 5) * i), CSize(m_xSize.cx, CardHeight + 5), 0)))
			return false;
	}

	//创建录像
	m_pRecord = CreateRecord();
	if (!m_pRecord)
		return false;

	//创建操作面板
	m_pOperPanel = CreateOperPanel(NULL, CPoint(420, 5), CPoint(0, 0), CSize(350, 148), 0);
	if (!m_pOperPanel)
		return false;

	return true;
}

bool CBoard::OnStartGame()
{
	m_byChair = 0;
	m_byChair1 = 0;
	m_byType = TYPE_GVP;
	m_byRspType = TYPE_GVP;
	m_byGrade = GRADE_INIT;
	m_byCount = 0;
	m_wAttach = 0;
	m_byRound = 1;
	m_byIndex = 1;
	m_byStage = STAGE_DIZHU;
	m_byDiZhu = 0xFF;

	m_pRecord->Clear();
	m_vecAction.clear();

	m_pOperPanel->OnStartGame();
	for (int i = 0; i < 3; i++)
		m_pPlayer[i]->OnStartGame();

	return true;
}

bool CBoard::ShufflePoke()
{
	//洗牌并保存
	m_pPoke->ShuffleCards();
	if (!m_pPoke->SaveCards(L"Cards.dat"))
		return false;

	SendCards();
	SendDiCards();

	OnSendCardsOver();
	return true;
}

bool CBoard::RecoverPoke()
{
	//加载已有卡牌
	if (!m_pPoke->LoadCards(L"Cards.dat"))
		return false;

	SendCards();
	SendDiCards();

	OnSendCardsOver();
	return true;
}

void CBoard::SendCards()
{
	for (int i = 0; i < 54; i++)
	{
		if (m_pPoke->IsEmpty())
			break;

		CCard *pCard = m_pPoke->GetCard();
		m_pPlayer[(i + m_byChair) % 3]->OnSendCard(pCard);
	}
}

void CBoard::SendDiCards()
{
}

void CBoard::OnSendCardsOver()
{
	for (int i = 0; i < 3; i++)
		m_pPlayer[i]->OnSendCardsOver();

	m_pOperPanel->OnSendCardsOver(m_byChair);

	//更新玩家状态
	m_pPlayer[m_byChair]->UpdateStatus(m_byType, m_byGrade, m_byCount, m_wAttach);
}

bool CBoard::CheckGameOver()
{
	for (int i = 0; i < 3; i++)
	{
		if (!m_pPlayer[i]->IsAlive())
			return true;
	}
	return false;
}

void CBoard::CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	DDiZhu_CallCrack(pCrackParam, pCrackResult);
}

bool CBoard::OnPut(CObject *pObject, UINT nFlags, CPoint point)
{
	if (CheckGameOver())
		return false;

	if (pObject)
	{
		bool bLegal = m_pPlayer[m_byChair]->OnPut(m_byRound, m_byIndex);
		if (!bLegal)
			return false;

		//玩家应答
		m_pPlayer[m_byChair]->GetResponse(m_byRspType, m_byGrade, m_byCount, m_wAttach);
	}
	else
	{
		VECBYTE vecCardValue;
		CLogic::GetValues(vecCardValue, m_byRspType, m_byGrade, m_byCount, m_wAttach);

		m_pPlayer[m_byChair]->OnPut(m_byRound, m_byIndex, vecCardValue);
	}

	//保存录像
	SaveRecord(RECORD_PUT);
	SaveAction();

	m_byType = m_byRspType;
	m_byIndex++;
	m_byChair1 = m_byChair;
	m_byChair = GetNextPlayer(m_byChair);

	//更新玩家状态
	m_pPlayer[m_byChair]->UpdateStatus(m_byType, m_byGrade, m_byCount, m_wAttach);

	m_pOperPanel->OnPut(CheckGameOver(), m_byChair);
	return true;
}

bool CBoard::OnGvp(CObject *pObject, UINT nFlags, CPoint point)
{
	if (CheckGameOver())
		return false;

	if (m_byIndex == 1)
		return false;

	m_byRspType = TYPE_GVP;

	//保存录像
	SaveRecord(RECORD_GVP);
	SaveAction();

	while (true)
	{
		m_byChair = ++m_byChair % 3;
		if (m_byChair == m_byChair1) //全部放弃
		{
			m_byRound++;
			m_byIndex = 1;
			m_byType = TYPE_GVP;
			m_byRspType = TYPE_GVP;
			m_byCount = 0;
			m_byGrade = GRADE_INIT;
			m_wAttach = 0;
		}
		break;
	}

	//更新玩家状态
	m_pPlayer[m_byChair]->UpdateStatus(m_byType, m_byGrade, m_byCount, m_wAttach);

	m_pOperPanel->OnGvp(m_byChair);
	return true;
}

bool CBoard::OnUndo(CObject *pObject, UINT nFlags, CPoint point)
{
	if (m_vecAction.empty())
		return false;

	//撤消出牌
	SaveRecord(RECORD_UNDO);
	BackAction();

	m_byGrade = m_byType != TYPE_GVP ? m_vecAction.back().byGrade : GRADE_INIT;
	m_byCount = m_byType != TYPE_GVP ? m_vecAction.back().byCount : 0;
	m_wAttach = m_byType != TYPE_GVP ? m_vecAction.back().wAttach : 0;

	m_pPlayer[m_byChair]->UnPut(m_byRound, m_byIndex);

	m_pOperPanel->OnUndo(m_byChair);

	//更新玩家状态
	m_pPlayer[m_byChair]->UpdateStatus(m_byType, m_byGrade, m_byCount, m_wAttach);
	return true;
}

bool CBoard::OnTrust(CObject *pObject, UINT nFlags, CPoint point)
{
	if (CheckGameOver())
		return false;

	//填写破解参数
	TCrackParam tCrackParam;
	tCrackParam.byDiZhu   = (m_byDiZhu - m_byChair + 3) % 3;
	tCrackParam.byChair1  = (m_byChair1 - m_byChair + 3) % 3;
	for (int i = 0; i < 3; i++)
		m_pPlayer[i]->GetHandBmp(tCrackParam.bitmap[(i - m_byChair + 3) % 3]);

	tCrackParam.byType    = m_byType;
	tCrackParam.byRspType = m_byRspType;
	tCrackParam.byGrade   = m_byGrade;
	tCrackParam.byCount   = m_byCount;
	tCrackParam.wAttach   = m_wAttach;
	tCrackParam.dwCrackDepth = 1000000;

	//调用破解过程
	UINT st = GetTickCount();

	TCrackResult tCrackResult;
	CallCrack(&tCrackParam, &tCrackResult);

	UINT et = GetTickCount();
	m_pOperPanel->ShowSpendTime(et - st);

	//处理破解结果
	ProcessCrackResult(&tCrackResult);
	return true;
}

void CBoard::ProcessCrackResult(TCrackResult *pCrackResult)
{
	m_byRspType = pCrackResult->byRspType;
	if (m_byRspType != TYPE_GVP)
	{
		//放弃时不改变
		m_byGrade = pCrackResult->byGrade;
		m_byCount = pCrackResult->byCount;
		m_wAttach = pCrackResult->wAttach;
		OnPut(NULL, 0, CPoint(0, 0));
	}
	else
	{
		OnGvp(NULL, 0, CPoint(0, 0));
	}

	m_pOperPanel->OnCrackResult(pCrackResult->byCrackScore, m_vecAction.back().byChair);
}

void CBoard::ClearAction()
{
	m_vecAction.clear();
}

void CBoard::SaveAction()
{
	TAction tAction;
	tAction.byChair = m_byChair;
	tAction.byChair1 = m_byChair1;
	tAction.byType = m_byType;
	tAction.byRspType = m_byRspType;
	tAction.byGrade = m_byGrade;
	tAction.byCount = m_byCount;
	tAction.wAttach = m_wAttach;
	tAction.byRound = m_byRound;
	tAction.byIndex = m_byIndex;

	m_vecAction.push_back(tAction);
}

void CBoard::BackAction()
{
	TAction &tAction = m_vecAction.back();
	m_byChair = tAction.byChair;
	m_byChair1 = tAction.byChair1;
	m_byType = tAction.byType;
	m_byRspType = TYPE_GVP;
	m_byGrade = tAction.byGrade;
	m_byCount = tAction.byCount;
	m_wAttach = tAction.wAttach;
	m_byRound = tAction.byRound;
	m_byIndex = tAction.byIndex;

	m_vecAction.pop_back();
}

void CBoard::SaveRecord(byte byType)
{
	TPutRecord tPutRecord;
	switch (byType)
	{
	case RECORD_PUT:
		tPutRecord.byChair   = m_byChair;
		tPutRecord.byRspType = m_byRspType;
		tPutRecord.byGrade   = m_byGrade;
		tPutRecord.byCount   = m_byCount;
		m_pRecord->WritePut(&tPutRecord);
		break;
	case RECORD_GVP:
		m_pRecord->WriteGvp();
		break;
	case RECORD_UNDO:
		m_pRecord->WriteUndo();
		break;
	}
}
