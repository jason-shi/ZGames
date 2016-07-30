#include "StdAfx.h"
#include <commctrl.h>
#include "MainWin.h"
#include "Mirror\MirBoard.h"
#include "Mirror\MirBox.h"

BOOL CMainWin::PreCreateWindow(CREATESTRUCT	&cs)
{
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.style = WS_OVERLAPPED | WS_CAPTION |	WS_SYSMENU | WS_MINIMIZEBOX;

	CRect rect(0, 0, ClientWidth, ClientHeight);
	::AdjustWindowRectEx(&rect,	cs.style, TRUE,	cs.dwExStyle);

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
	cs.lpszClass = L"ZChroma";

	if (!Application->RegisterWndClass(cs.lpszClass,
		CS_VREDRAW | CS_HREDRAW	| CS_OWNDC | CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)::GetStockObject(BLACK_BRUSH), Application->LoadIcon(IDC_APPICON)))
		return FALSE;

	return TRUE;
}

LRESULT	CMainWin::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int xPos = LOWORD(lParam);
	int yPos = HIWORD(lParam);
	switch (uMsg)
	{
	case WM_ERASEBKGND:
		return FALSE;

	case WM_LBUTTONDOWN:
		g_pZ2coo->OnLButtonDown(wParam, CPoint(xPos, yPos));
		break;

	case WM_LBUTTONUP:
		g_pZ2coo->OnLButtonUp(wParam, CPoint(xPos, yPos));
		break;

	case WM_MOUSEMOVE:
		g_pZ2coo->OnMouseMove(wParam, CPoint(xPos, yPos));
		break;

	case WM_MOUSEWHEEL:
		g_pZ2coo->OnMouseWheel(wParam & 0xFFFF, wParam >> 16, CPoint(xPos, yPos));
		break;

	case WM_RBUTTONDOWN:
		g_pZ2coo->OnRButtonDown(wParam, CPoint(xPos, yPos));
		break;

	case WM_RBUTTONUP:
		g_pZ2coo->OnRButtonUp(wParam, CPoint(xPos, yPos));
		break;

	case WM_MBUTTONDOWN:
		g_pZ2coo->OnMButtonDown(wParam, CPoint(xPos, yPos));
		break;

	case WM_MBUTTONUP:
		g_pZ2coo->OnMButtonUp(wParam, CPoint(xPos, yPos));
		break;

	case WM_ANIME_UPDATE:
		g_pZ2mov->Update(wParam);
		break;

	case WM_ANIME_END:
		OnAnimeEnd((CAnime *)wParam);
		break;

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

BOOL CMainWin::OnCreate(CREATESTRUCT *cs)
{
	LoadAccelTable(IDC_APPACCEL);
	CClientDC dc(this);

	if (!ViewImage.Create(dc, ClientWidth,	ClientHeight)) {
		MessageBox(L"无法创建窗口！", L"主窗口", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	if (!BackImage.LoadImage(L"PNG\\Back.png")) {
		MessageBox(L"无法打开文件 PNG\\Back.png!", L"主窗口", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	ViewImage.Copy(&BackImage);

	//发送触发的第一个事件
	while (!PostMessage(WM_FIRST_ACTION))
		::Sleep(110);	//重送事件的动作再稍等一下

	return TRUE;
}

bool CMainWin::OnFirstAction()
{
	SendMessage(WM_PAINT);
	return true;
}

void CMainWin::OnClose()
{
	::DestroyWindow(hWnd);
}

void CMainWin::OnPaint()
{
	CPaintDC dc(this);
	const CRect& rect = dc.ps.rcPaint;

	//背景重绘
	ViewImage.Copy(&BackImage);
	g_pZ2coo->Draw(&ViewImage, rect);
	ViewImage.Draw(dc, rect);
}
