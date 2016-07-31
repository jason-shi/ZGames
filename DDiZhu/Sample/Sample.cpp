#include "StdAfx.h"
#include "Sample.h"
#include "MainWin\MainWin.h"

CSampleApp			 theApp;

CSampleApp::CSampleApp() : CWinApp()
{
}

CSampleApp::~CSampleApp()
{
	delete MainWnd;
	MainWnd = NULL;
}

BOOL CSampleApp::InitInstance()
{
	MainWnd = new CMainWin();
	if (!MainWnd)
		return FALSE;

	if (!MainWnd->Create(this, L"DDiZhu", LoadMenu(IDC_APPMENU)))
		return FALSE;

	MainWnd->ShowWindow(CmdShow);
	return TRUE;
}
