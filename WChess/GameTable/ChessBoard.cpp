#include "StdAfx.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "GameTable.h"
#include "Action\Action.h"
#include <boost\bind.hpp>

static int TMul38[10] = { 0, 38, 76, 114, 152, 190, 228, 266, 304, 342, };

//利用快查计算棋子位置
#define PIECE_SRC(nKind)	CPoint(36 * (nKind), 0)
#define PIECE_POS(nGrid)	CPoint(TMul38[(nGrid) & 7] + 2, TMul38[7 - ((nGrid) >> 3)] + 2)

WChessBoard::WChessBoard() : CPanel()
{
	//使能够响应鼠标
	SetCease();

	//board和m_pPiece初始化
	memset(board, 0, sizeof(board));
	memset(m_pPiece, NULL, sizeof(m_pPiece));
}

WChessBoard::~WChessBoard()
{
	ClearFixed();

	//通过清空棋盘释放
	memset(board, 0, sizeof(board));
	FastSyncBoard();

	if (m_pPiecePool)
	{
		m_pPiecePool->ForEach(boost::bind(&WChessPiece::Release, _1), NULL);

		delete m_pPiecePool;
		m_pPiecePool = NULL;
	}

	if (m_pFixedPool)
	{
		m_pFixedPool->ForEach(boost::bind(&CSprite::Release, _1), NULL);

		delete m_pFixedPool;
		m_pFixedPool = NULL;
	}

	delete m_pMoveTagA;
	m_pMoveTagA = NULL;

	delete m_pMoveTagB;
	m_pMoveTagB = NULL;
}

bool WChessBoard::Initial()
{
	if (!CPanel::Initial())
		return false;

	//初始化棋子对象池
	WChessPiece tPieceData(CPoint(0, 0), 0, 0);
	tPieceData.m_pHome = this;
	tPieceData.SetMouseProc(MP_LCLICK, boost::bind(&WChessBoard::OnLClick, this, _1));
	tPieceData.SetMouseProc(MP_RCLICK, boost::bind(&WChessBoard::OnRClick, this, _1));
	tPieceData.SetMouseProc(MP_MCLICK, boost::bind(&WChessBoard::OnMClick, this, _1));
	m_pPiecePool = CreateZPool(sizeof(WChessPiece), 32, &tPieceData);
	if (!m_pPiecePool)
		return false;
	m_pPiecePool->ForEach(boost::bind(&WChessPiece::Initial, _1), NULL);

	//m_bFixed和m_pFixed初始化
	memset(m_bFixed, 0, sizeof(m_bFixed));
	memset(m_pFixed, NULL, sizeof(m_pFixed));

	//初始化光标对象池
	CSprite tFixedData(&ImagePiece, CPoint(0, 0), CPoint(0, 0), CSize(36, 36), 0);
	m_pFixedPool = CreateZPool(sizeof(CSprite), 32, &tFixedData);
	if (!m_pFixedPool)
		return false;
	m_pFixedPool->ForEach(boost::bind(&CSprite::Initial, _1), NULL);

	//移动标记初始化
	m_pMoveTagA = new CSprite(&ImagePiece, CPoint(0, 0), CSize(36, 36), 0);
	if (!m_pMoveTagA || !m_pMoveTagA->Initial())
		return false;
	m_pMoveTagA->Show(false);
	AddObject(m_pMoveTagA);

	m_pMoveTagB = new CSprite(&ImagePiece, CPoint(0, 0), CSize(36, 36), 0);
	if (!m_pMoveTagB || !m_pMoveTagB->Initial())
		return false;
	m_pMoveTagB->Show(false);
	AddObject(m_pMoveTagB);
	return true;
}

bool WChessBoard::Release()
{
	return true;
}

void WChessBoard::_AddPiece(int nGrid, int nKind)
{
	m_pPiece[nGrid] = (WChessPiece *)m_pPiecePool->GetNode();
	m_pPiece[nGrid]->SetGrid(nGrid);
	m_pPiece[nGrid]->SetKind(nKind);
	m_pPiece[nGrid]->SetDrawPos(PIECE_POS(nGrid));
	m_pPiece[nGrid]->SetSrcPos(PIECE_SRC(nKind));

	AddObject(m_pPiece[nGrid]);
}

void WChessBoard::_DelPiece(int nGrid)
{
	m_pPiecePool->RtnNode((char *)m_pPiece[nGrid]);
	DelObject(m_pPiece[nGrid]);

	m_pPiece[nGrid] = NULL;
}

void WChessBoard::_ChgPiece(int nGrid, int nKind)
{
	m_pPiece[nGrid]->SetKind(nKind);
	m_pPiece[nGrid]->SetSrcPos(PIECE_SRC(nKind));
}

void WChessBoard::AddPiece(int nGrid, int nKind)
{
	DelPiece(nGrid);

	//删除后再添加棋子
	board[nGrid] = nKind;
	if (board[nGrid] == 0)
		return;

	_AddPiece(nGrid, nKind);
}

void WChessBoard::DelPiece(int nGrid)
{
	if (board[nGrid] == 0)
		return;
	board[nGrid] = 0;

	_DelPiece(nGrid);
}

void WChessBoard::FastSyncBoard()
{
	//为了防止节点溢出：先删除，改变，然后再增加
	for (int nGrid = 0; nGrid < 64; nGrid++)
	{
		if (m_pPiece[nGrid])
		{
			if (board[nGrid] == 0)
				_DelPiece(nGrid);
			else if (m_pPiece[nGrid]->GetKind() != board[nGrid])
				_ChgPiece(nGrid, board[nGrid]);
		}
	}
	for (int nGrid = 0; nGrid < 64; nGrid++)
	{
		if (board[nGrid] > 0 && !m_pPiece[nGrid])
			_AddPiece(nGrid, board[nGrid]);
	}
}

bool WChessBoard::IsFixed(int nGrid)
{
	return m_bFixed[nGrid];
}

void WChessBoard::SetFixed(int nGrid)
{
	if (IsFixed(nGrid))
		return;

	m_bFixed[nGrid] = true;

	m_pFixed[nGrid] = (CSprite *)m_pFixedPool->GetNode();
	m_pFixed[nGrid]->SetDrawPos(PIECE_POS(nGrid));

	AddObject(m_pFixed[nGrid]);
}

void WChessBoard::ClearFixed(int nGrid)
{
	if (!IsFixed(nGrid))
		return;

	m_bFixed[nGrid] = false;

	m_pFixedPool->RtnNode((char *)m_pFixed[nGrid]);
	DelObject(m_pFixed[nGrid]);

	m_pFixed[nGrid] = NULL;
}

void WChessBoard::ToggleFixed(int nGrid)
{
	m_bFixed[nGrid] ? ClearFixed(nGrid) : SetFixed(nGrid);
}

void WChessBoard::ClearFixed()
{
	for (int nGrid = 0; nGrid < 64; nGrid++)
	{
		if (IsFixed(nGrid))
			ClearFixed(nGrid);
	}
}

void WChessBoard::ShowMoveTag(int nSrcGrid, int nDestGrid)
{
	m_pMoveTagA->SetDrawPos(PIECE_POS(nSrcGrid));
	m_pMoveTagB->SetDrawPos(PIECE_POS(nDestGrid));
	m_pMoveTagA->Show();
	m_pMoveTagB->Show();
}

void WChessBoard::HideMoveTag()
{
	m_pMoveTagA->Show(false);
	m_pMoveTagB->Show(false);
}

void WChessBoard::MovePiece(int nSrcGrid, int nDestGrid, int nBian)
{
	//必须先删除再添加
	int nKind = board[nSrcGrid];
	DelPiece(nSrcGrid);
	AddPiece(nDestGrid, nBian == 0 ? nKind : nBian);

	ShowMoveTag(nSrcGrid, nDestGrid);
}

int WChessBoard::GetGrid(CPoint point) const
{
	//偏移到盘面坐标
	point -= m_xDrawPos + m_xOffset;

	int nX = point.x / 38;
	int nY = point.y / 38;

	if (nX * 38 + 2 <= point.x && nX * 38 + 37 >= point.x && nY * 38 + 2 <= point.y && nY * 38 + 37 >= point.y)
		return nX + ((7 - nY) << 3);

	return INVALID_GRID;
}

void WChessBoard::OnLClick(UINT nFlags, CPoint point)
{
	int nGrid = GetGrid(point);

	CAction *pAction = g_pGameTable->GetAction();
	if (pAction)
		pAction->ChoiceBoard(nGrid);
}

void WChessBoard::OnLClick(CObject *pPiece)
{
	CAction *pAction = g_pGameTable->GetAction();
	if (pAction)
		pAction->ChoicePiece((WChessPiece *)pPiece);
}

void WChessBoard::OnRClick(CObject *pPiece)
{
	CAction *pAction = g_pGameTable->GetAction();
	if (pAction)
		pAction->ClearPiece((WChessPiece *)pPiece);
}

void WChessBoard::OnMClick(CObject *pPiece)
{
	CAction *pAction = g_pGameTable->GetAction();
	if (pAction)
		pAction->ToggleFixed((WChessPiece *)pPiece);
}
