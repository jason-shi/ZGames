#include "StdAfx.h"
#include "Action.h"
#include "ChessPiece.h"

CAction::CAction()
{
}

CAction::~CAction()
{
}

void CAction::Select(WChessPiece *pPiece)
{
	pPiece->OnSelect();
	m_pSelected = pPiece;
}

void CAction::UnSelect()
{
	if (m_pSelected)
	{
		m_pSelected->OnUnSelect();
		m_pSelected = NULL;
	}
}
