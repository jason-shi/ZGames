#include "StdAfx.h"
#include "SplashWin.h"
#include "Sample\Sample.h"

CSplashWin			 SplashWin;

static HANDLE		 g_hThreadLoad = NULL;

static UINT WINAPI ThreadLoad()
{
	if (!WChess_Initial())
		return false;

	SplashWin.SendMessage(WM_CLOSE, 0, 0);
	return 0;
}

bool InitThreadLoad()
{
	if (!g_hThreadLoad)
	{
		g_hThreadLoad = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadLoad, NULL, 0, 0);
		if (g_hThreadLoad)
		{
			CloseHandle(g_hThreadLoad);
			g_hThreadLoad = NULL;
			return true;
		}
	}
	return false;
}

BOOL CSplashWin::PreCreateWindow(CREATESTRUCT &cs)
{
	cs.style = WS_POPUP;

	CRect rect(0, 0, 205, 154);
	::AdjustWindowRectEx(&rect, cs.style, FALSE, cs.dwExStyle);

	int	width =	rect.Width();
	int	height = rect.Height();

	CRect ScreenArea;
	SystemParametersInfo(SPI_GETWORKAREA, NULL,	&ScreenArea, NULL);

	int	x =	ScreenArea.left	+ (ScreenArea.Width() -	width) / 2;
	int	y =	ScreenArea.top + (ScreenArea.Height() -	height)	/ 2;

	cs.x = x;
	cs.y = y;
	cs.cx =	width;
	cs.cy =	height;
	cs.lpszClass = L"Splash";

	if (!Application->RegisterWndClass(cs.lpszClass,
		CS_VREDRAW | CS_HREDRAW	| CS_OWNDC, LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)::GetStockObject(BLACK_BRUSH), Application->LoadIcon(IDC_APPICON)))
		return FALSE;

	return TRUE;
}

LRESULT CSplashWin::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_FIRST_ACTION:
		OnFirstAction();
		break;

	case WM_CLOSE:
		OnClose();
		break;

	default:
		return CWindow::WindowProc(uMsg, wParam, lParam);
	}
	return 0L;
}

BOOL CSplashWin::OnCreate(CREATESTRUCT *cs)
{
	CClientDC dc(this);

	if (!ViewImage.Create(dc, 205, 154)) {
		return FALSE;
	}
	if (!BackImage.LoadImage(L"PNG\\Splash.png")) {
		return FALSE;
	}
	ViewImage.Copy(&BackImage);

	//发送触发的第一个事件
	while (!PostMessage(WM_FIRST_ACTION))
		::Sleep(110);	//重送事件的动作再稍等一下

	return TRUE;
}

bool CSplashWin::OnFirstAction()
{
	SendMessage(WM_PAINT);
	return InitThreadLoad();
}

void CSplashWin::OnClose()
{
	::DestroyWindow(hWnd);
}

void CSplashWin::OnDestroy()
{
	if (!theApp.InitGameTable())
		CWindow::OnDestroy();
}

void CSplashWin::OnPaint()
{
	CPaintDC dc(this);

	ViewImage.Copy(&BackImage);
	ViewImage.Draw(dc, dc.ps.rcPaint);
}
