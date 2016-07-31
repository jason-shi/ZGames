#include "StdAfx.h"
#include "OperPanel.h"
#include "Board.h"
#include <boost\bind.hpp>

using namespace std;

COperPanel::COperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pBtnPut = NULL;
	m_pBtnGvp = NULL;
	m_pBtnTrust = NULL;
	m_pBtnUndo = NULL;
	m_pSprTurn = NULL;
	m_pSprScore = NULL;
	m_pSpendTime = NULL;
}

COperPanel::~COperPanel()
{
	delete m_pBtnPut;
	m_pBtnPut = NULL;

	delete m_pBtnGvp;
	m_pBtnGvp = NULL;

	delete m_pBtnTrust;
	m_pBtnTrust = NULL;

	delete m_pBtnUndo;
	m_pBtnUndo = NULL;

	delete m_pSprTurn;
	m_pSprTurn = NULL;

	delete m_pSprScore;
	m_pSprScore = NULL;

	delete m_pSpendTime;
	m_pSpendTime = NULL;
}

bool COperPanel::Initial(CBoard *pBoard)
{
	if (!CPanel::Initial())
		return false;

	//创建出牌按钮
	m_pBtnPut = new CButton(&ImageDDiZhu, CRect(50, 0, 100, 35), CRect(0, 0, 200, 35), 0);
	AddObject(m_pBtnPut);
	m_pBtnPut->SetMouseProc(MP_LCLICK, boost::bind(&CBoard::OnPut, pBoard, _1, _2, _3));

	//创建放弃按钮
	m_pBtnGvp = new CButton(&ImageDDiZhu, CRect(50, 35, 100, 70), CRect(0, 35, 200, 70), 0);
	AddObject(m_pBtnGvp);
	m_pBtnGvp->SetMouseProc(MP_LCLICK, boost::bind(&CBoard::OnGvp, pBoard, _1, _2, _3));

	//创建托管按钮
	m_pBtnTrust = new CButton(&ImageDDiZhu, CRect(50, 70, 100, 105), CRect(0, 70, 200, 105), 0);
	AddObject(m_pBtnTrust);
	m_pBtnTrust->SetMouseProc(MP_LCLICK, boost::bind(&CBoard::OnTrust, pBoard, _1, _2, _3));

	//创建撤消按钮
	m_pBtnUndo = new CButton(&ImageDDiZhu, CRect(50, 105, 100, 140), CRect(0, 105, 200, 140), 0);
	AddObject(m_pBtnUndo);
	m_pBtnUndo->SetMouseProc(MP_LCLICK, boost::bind(&CBoard::OnUndo, pBoard, _1, _2, _3));

	//创建次序标记
	m_pSprTurn = new CSprite(&ImageDDiZhu, CPoint(0, 0), CPoint(0, 175), CSize(50, 35), 0);
	m_pSprTurn->Show(false);
	AddObject(m_pSprTurn);

	//创建得分标记
	m_pSprScore = new CSprite(&ImageDDiZhu, CPoint(100, 75), CPoint(250, 140), CSize(50, 25), 0);
	AddObject(m_pSprScore);

	//思考时间
	m_pSpendTime = new CTextBlk(false, 16, CPoint(180, 80), CSize(160, 16), 0);
	if (!m_pSpendTime || !m_pSpendTime->Initial())
		return false;
	m_pSpendTime->ClearImage();
	AddObject(m_pSpendTime);
	return true;
}

void COperPanel::OnStartGame()
{
	Show(false);
}

void COperPanel::ShowSpendTime(int nMS)
{
	static HFONT hFont = CreateFont(16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"simsun.ttc");
	TCHAR wcsBuff[32];

	wsprintf(wcsBuff, L"%d.%03d", nMS / 1000, nMS % 1000);
	m_pSpendTime->OutText(wcsBuff, wcslen(wcsBuff), hFont, true, BGRA(0, 0, 0, 255));
}

void COperPanel::OnSendCardsOver(byte byChair)
{
	Show();
	m_pSprScore->Show(false);
	m_pSprTurn->SetDrawPos(0, byChair * PlayerHeight);
}

void COperPanel::OnPut(bool bGameOver, byte byChair)
{
	m_pSprScore->Show(false);
	m_pSprTurn->Show(!bGameOver);
	m_pSprTurn->SetDrawPos(0, byChair * PlayerHeight);
}

void COperPanel::OnGvp(byte byChair)
{
	m_pSprScore->Show(false);
	m_pSprTurn->Show(true);
	m_pSprTurn->SetDrawPos(0, byChair * PlayerHeight);
}

void COperPanel::OnUndo(byte byChair)
{
	m_pSprScore->Show(false);
	m_pSprTurn->Show();
	m_pSprTurn->SetDrawPos(0, byChair * PlayerHeight);
}

void COperPanel::OnCrackResult(byte byScore, byte byChair)
{
	m_pBtnPut->Disable(false);
	m_pBtnTrust->Disable(false);
	m_pBtnUndo->Disable(false);

	m_pSprScore->Show();
	m_pSprScore->SetSrcPos((byScore - SCORE_1) * 50, 140);
	m_pSprScore->SetDrawPos(100, byChair * PlayerHeight);
}
