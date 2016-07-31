#include "StdAfx.h"
#include "MainWin.h"
#include "MainPanel\MainPanel.h"

#define ClientWidth		840
#define ClientHeight	525

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
	cs.lpszClass = _T("DDiZhu");

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

	case WM_CLOSE:
		OnClose();
		break;

	case WM_ANIME_UPDATE:
		g_pZ2mov->Update(wParam);
		break;

	case WM_ANIME_END:
		OnAnimeEnd((CAnime *)wParam);
		break;

	case WM_TIMER_UPDATE:
		g_pZTimer->Update(wParam);
		break;

	case WM_FIRST_ACTION:
		OnFirstAction();
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
	if (!g_pZ2coo->Initial())
		return false;
	g_pZ2coo->SetWindow(this);

	if (!g_pZ2mov->Initial())
		return false;
	g_pZ2mov->SetWindow(this);

	if (!g_pZTimer->Initial())
		return false;
	g_pZTimer->SetWindow(this);

	SendMessage(WM_PAINT);

	CRect rect;
	GetClientRect(&rect);

	m_pMainPanel = new CMainPanel(NULL, CPoint(0, 0), rect.Size(), 0);
	if (!m_pMainPanel || !m_pMainPanel->Initial())
		return false;

	//斗地主机器人破解初始化
	DDiZhu_Initial();
	return true;
}

void CMainWin::OnClose()
{
	delete m_pMainPanel;
	m_pMainPanel = NULL;

	//斗地主机器人释放资源
	DDiZhu_Release();

	g_pZ2coo->Release();
	g_pZ2mov->Release();
	g_pZTimer->Release();
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

void CMainWin::OnCommand(UINT notifyCode, UINT id, HWND ctrl)
{
	//控件消息不处理
	if (ctrl != NULL)
		return;

	//退出处理
	switch (id)
	{
	case ID_APP_EXIT:
		SendMessage(WM_CLOSE);
		break;

	case ID_APP_START:
		m_pMainPanel->OnStartGame();
		break;

	case ID_APP_SAVE:
		m_pMainPanel->OnKeySave();
		break;

	case ID_APP_LOAD:
		m_pMainPanel->OnRecoverPoke();
		break;

	case ID_APP_UP:
		m_pMainPanel->OnKeyUp();
		break;

	case ID_APP_DOWN:
		m_pMainPanel->OnKeyDown();
		break;

	case ID_APP_LEFT:
		m_pMainPanel->OnKeyLeft();
		break;

	case ID_APP_RIGHT:
		m_pMainPanel->OnKeyRight();
		break;
	}
}
