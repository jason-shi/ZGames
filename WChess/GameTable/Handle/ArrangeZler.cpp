#include "StdAfx.h"
#include "ArrangeZler.h"
#include "GameTable.h"
#include "Action\Action.h"

CArrangeZler::CArrangeZler(CWindow *_window) : ICmdZler()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CArrangeZler::~CArrangeZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CArrangeZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_ARRANGE:
		OnMenuArrange();
		break;
	default:
		return false;
	}
	return true;
}

void CArrangeZler::OnMenuArrange()
{
	g_pGameTable->SetAction(ARRANGE_ACTION);

	SendViewInd_ArrangeState();
	SendMenuInd_ArrangeState();
}

void CArrangeZler::SendViewInd_ArrangeState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bShowBox = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CArrangeZler::SendMenuInd_ArrangeState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
