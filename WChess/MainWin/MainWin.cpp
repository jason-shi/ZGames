#include "StdAfx.h"
#include "MainWin.h"
#include "MenuDef.h"

BOOL CMainWin::PreCreateWindow(CREATESTRUCT	&cs)
{
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.style = WS_OVERLAPPED | WS_CAPTION |	WS_SYSMENU | WS_MINIMIZEBOX;

	CRect	rect(0,	0, 640,	480);
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
	cs.lpszClass = L"WChess";

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
	case WM_CLOSE:
		OnClose();
		break;

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

	case WM_TIMER_UPDATE:
		g_pZTimer->Update(wParam);
		break;

	case WM_MENU_IND:
		EnableMenu(wParam);
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

	if (!ViewImage.Create(dc, 640, 480)) {
		MessageBox(L"无法以640 * 480的模式创建窗口！", L"启动窗口", MB_ICONERROR | MB_OK);
		return FALSE;
	}
	if (!BackImage.LoadImage(L"PNG\\Chessboard.png")) {
		MessageBox(L"无法打开文件 PNG\\Chessboard.png!", L"启动窗口", MB_ICONERROR | MB_OK);
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
	//立即绘制背景
	SendMessage(WM_PAINT);

	//初始化菜单
	SendMenuInd_InitState();
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

void CMainWin::EnableMenu(bool bIncr)
{
	static int MFlag[2] = {
		MF_BYCOMMAND | MF_GRAYED | MF_DISABLED, //不可用
		MF_BYCOMMAND | MF_ENABLED, //可用
	};

//根据更新位图和启用位图设置菜单项
#define ENABLEMENUITEM(group, mask, id) \
	if (!bIncr || (g_tMenuInd.nIncrBitmap[group] & mask)) \
		EnableMenuItem(hSubMenu, id, MFlag[g_tMenuInd.nMenuBitmap[group] & mask ? 1 : 0]);

	//第一列子菜单
	HMENU hSubMenu = GetSubMenu(GetMenu(), 0);
	ENABLEMENUITEM(0, EN_MENU_OPEN,           ID_APP_OPEN);
	ENABLEMENUITEM(0, EN_MENU_RUN,            ID_APP_RUN);
	ENABLEMENUITEM(0, EN_MENU_AGAIN,          ID_APP_AGAIN);
	ENABLEMENUITEM(0, EN_MENU_ARRANGE,        ID_APP_ARRANGE);
	ENABLEMENUITEM(0, EN_MENU_UNDO,           ID_APP_UNDO);
	ENABLEMENUITEM(0, EN_MENU_SAVE,           ID_APP_SAVE);
	ENABLEMENUITEM(0, EN_MENU_MANUAL,         ID_APP_MANUAL);
	ENABLEMENUITEM(0, EN_MENU_BATCH,          ID_APP_BATCH);
	ENABLEMENUITEM(0, EN_MENU_EXIT,           ID_APP_EXIT);
	//第二列子菜单
	hSubMenu = GetSubMenu(GetMenu(), 1);
	ENABLEMENUITEM(1, EN_MENU_COPY,           ID_APP_COPY);
	ENABLEMENUITEM(1, EN_MENU_PASTE,          ID_APP_PASTE);
	ENABLEMENUITEM(1, EN_MENU_KEY1,           ID_ACCEL1001);
	ENABLEMENUITEM(1, EN_MENU_KEY2,           ID_ACCEL1002);
	ENABLEMENUITEM(1, EN_MENU_KEY3,           ID_ACCEL1003);
	ENABLEMENUITEM(1, EN_MENU_KEY4,           ID_ACCEL1004);
	ENABLEMENUITEM(1, EN_MENU_CTRLKEY1,       ID_ACCEL1005);
	ENABLEMENUITEM(1, EN_MENU_CTRLKEY2,       ID_ACCEL1006);
	ENABLEMENUITEM(1, EN_MENU_CTRLKEY3,       ID_ACCEL1007);
	ENABLEMENUITEM(1, EN_MENU_CTRLKEY4,       ID_ACCEL1008);
	//第三列子菜单
	hSubMenu = GetSubMenu(GetMenu(), 2);
	ENABLEMENUITEM(2, EN_MENU_PATTERN,        ID_APP_PATTERN);
	ENABLEMENUITEM(2, EN_MENU_TRACE,          ID_APP_TRACE);
	ENABLEMENUITEM(2, EN_MENU_LIMITARYTRACE,  ID_APP_LIMITARYTRACE);
	ENABLEMENUITEM(2, EN_MENU_NEXT,           ID_APP_NEXT);
	ENABLEMENUITEM(2, EN_MENU_LAST,           ID_APP_LAST);
	ENABLEMENUITEM(2, EN_MENU_SOURCE,         ID_APP_SOURCE);
	ENABLEMENUITEM(2, EN_MENU_LIMITARYSOURCE, ID_APP_LIMITARYSOURCE);
	//第四列子菜单
	hSubMenu = GetSubMenu(GetMenu(), 3);
	ENABLEMENUITEM(3, EN_MENU_EXPLAINATION,   ID_APP_EXPLAINATION);
#ifdef _DEBUG
	ENABLEMENUITEM(3, EN_MENU_REGISTER,       ID_APP_REGISTER);
#endif

#undef ENABLEMENUITEM
}

void CMainWin::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_INIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
