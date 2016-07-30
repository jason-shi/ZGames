#include "StdAfx.h"
#include "Sample.h"
#include "GameTable\GameTable.h"

CSampleApp			 theApp;

CSampleApp::CSampleApp() : CWinApp()
{
}

CSampleApp::~CSampleApp()
{
	g_pGameTable->Release();
}

BOOL CSampleApp::InitInstance()
{
	if (!g_pGameTable->Initial())
		return FALSE;

	MainWnd = (CWindow *)g_pGameTable;

	if (!MainWnd->Create(this, L"ZChroma", LoadMenu(IDC_APPMENU)))
		return FALSE;

	if (MainWnd)
		MainWnd->ShowWindow(CmdShow);

	return TRUE;
}
