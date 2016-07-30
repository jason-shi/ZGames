#include "StdAfx.h"
#include "MirPanel.h"
#include "MirAction.h"
#include "GameTable.h"
#include <boost\bind.hpp>

CMirPanel::CMirPanel() : CPanel()
{
}

CMirPanel::~CMirPanel()
{
}

void CMirPanel::OnLClick(UINT nFlags, CPoint point)
{
	if (nFlags != MK_SHIFT)
		return;

	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->ChoiceMirror(this, point, NULL);
}

void CMirPanel::OnLClick(CObject *pMirror, UINT nFlags, CPoint point)
{
	if (nFlags != MK_SHIFT)
		return;

	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->ChoiceMirror(this, point, (CMirror *)pMirror);
}

void CMirPanel::OnLButtonUp(CObject *pMirror, UINT nFlags, CPoint point)
{
	if (nFlags != 0)
		return;

	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->RotateMirror((CMirror *)pMirror, true);
}

void CMirPanel::OnRButtonUp(CObject *pMirror, UINT nFlags, CPoint point)
{
	if (nFlags != 0)
		return;

	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->RotateMirror((CMirror *)pMirror, false);
}

void CMirPanel::OnMousePick(CObject *pMirror, UINT nFlags, CPoint point)
{
	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->PickMirror((CMirror *)pMirror, point);
}

void CMirPanel::OnMouseDrag(CObject *pMirror, UINT nFlags, CPoint point)
{
	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->DragMirror((CMirror *)pMirror, point);
}

void CMirPanel::OnMouseDrop(CObject *pMirror, UINT nFlags, CPoint point)
{
	CMirAction *pAction = g_pGameTable->GetMirAction();
	if (pAction)
		pAction->DropMirror((CMirror *)pMirror, point);
}
