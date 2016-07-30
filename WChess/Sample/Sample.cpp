#include "StdAfx.h"
#include "Sample.h"
#include "SplashWin\SplashWin.h"
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
	MainWnd = &SplashWin;

	if (!MainWnd->Create(this, L"Loading...", LoadMenu(NULL)))
		return FALSE;

	if (MainWnd)
		MainWnd->ShowWindow(CmdShow);

	return TRUE;
}

BOOL CSampleApp::InitGameTable()
{
	if (!g_pGameTable->Initial())
		return FALSE;

	MainWnd = (CWindow *)g_pGameTable;

	if (!MainWnd->Create(this, L"WChess Puzzle", LoadMenu(IDC_APPMENU)))
		return FALSE;

	if (MainWnd)
		MainWnd->ShowWindow(CmdShow);

	return TRUE;
}
