#include "StdAfx.h"
#include <commdlg.h>
#include "Access.h"
#include "AccessZler.h"
#include "GameTable.h"

CAccessZler::CAccessZler(CWindow *_window) : ICmdZler()
{
	for (int i = 0; i < 4; i++)
	{
		//设置初始盘面
		SetVirginFen(KeyFen[i]); 
	}
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CAccessZler::~CAccessZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CAccessZler::ProcessCmd(int nType)
{ 
	switch (nType)
	{
	case ID_ACCEL1001:
		SaveBoardToKey(1);
		break;
	case ID_ACCEL1002:
		SaveBoardToKey(2);
		break;
	case ID_ACCEL1003:
		SaveBoardToKey(3);
		break;
	case ID_ACCEL1004:
		SaveBoardToKey(4);
		break;
	case ID_ACCEL1005:
		LoadBoardFromKey(1);
		break;
	case ID_ACCEL1006:
		LoadBoardFromKey(2);
		break;
	case ID_ACCEL1007:
		LoadBoardFromKey(3);
		break;
	case ID_ACCEL1008:
		LoadBoardFromKey(4);
		break;
	case ID_APP_OPEN:
		OnMenuOpen();
		break;
	case ID_APP_SAVE:
		OnMenuSave();
		break;
	case ID_APP_COPY:
		OnMenuCopy();
		break;
	case ID_APP_PASTE:
		OnMenuPaste();
		break;
	default:
		return false;
	}
	return true;
}

void CAccessZler::SaveBoardToKey(int key)
{
	TCHAR wcsBuff[128];
	wsprintf(wcsBuff, L"保存到 %d 吗？\n使用Ctrl+%d可以还原到当前盘面。", key, key);
	if (window->MessageBox(wcsBuff, L"快捷存储", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		//将盘面存储到快捷盘
		SaveToFen(KeyFen[key - 1]);
	}
}

void CAccessZler::LoadBoardFromKey(int key)
{
	TCHAR wcsBuff[128];
	wsprintf(wcsBuff, L"还原到盘面 %d 吗?", key);
	if (window->MessageBox(wcsBuff, L"快捷还原", MB_ICONQUESTION | MB_OKCANCEL) == IDOK)
	{
		//从快捷盘加载
		LoadByFen(KeyFen[key - 1]);
		OnLoadSuccess();
	}
}

void CAccessZler::OnMenuOpen()
{
	static OPENFILENAME ofn;
	static TCHAR wcsFileName[MAX_PATH], wcsTitleName[MAX_PATH];
	static TCHAR wcsFilter[] = TEXT(".pgn\0*.pgn\0");
	//打开.pgn文件
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.Flags = OFN_READONLY;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = wcsFilter;
	ofn.lpstrFile = wcsFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = wcsTitleName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = TEXT("pgn");
	if (!GetOpenFileName(&ofn))
		return;
	if (_waccess(wcsFileName, 0) == -1)
	{
		window->MessageBox(L"文件不存在", L"打开", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!LoadByPgn(wcsFileName))
	{
		window->MessageBox(L"FEN格式错误", L"加载", MB_ICONWARNING | MB_OK);
		return;
	}
	OnLoadSuccess();
}

void CAccessZler::OnMenuSave()
{
	static OPENFILENAME ofn;
	static TCHAR wcsFileName[MAX_PATH], wcsTitleName[MAX_PATH];
	static TCHAR wcsFilter[] = TEXT(".pgn\0*.pgn\0");

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.Flags = OFN_CREATEPROMPT;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = wcsFilter;
	ofn.lpstrFile = wcsFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = wcsTitleName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = TEXT("pgn");
	if (!GetSaveFileName(&ofn))
		return;
	if (_waccess(wcsFileName, 0) != -1 && _waccess(wcsFileName, 2) == -1)
	{
		window->MessageBox(L"文件没有保存权限", L"保存", MB_ICONWARNING | MB_OK);
		return;
	}
	SaveToPgn(wcsFileName);
}

void CAccessZler::OnMenuCopy()
{
	TCHAR wcsBuff[256];
	if (!OpenClipboard(NULL))
	{
		window->MessageBox(L"无法打开剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!EmptyClipboard())
	{
		window->MessageBox(L"无法清除剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	HGLOBAL hData = GlobalAlloc(LMEM_MOVEABLE|LMEM_ZEROINIT, 256);
	{
		TCHAR *fen = (TCHAR *)GlobalLock(hData);
		//获取FEN串并补充
		SaveToFen(wcsBuff);
		wsprintf(fen, L"%s%s", wcsBuff, L"w - - 0 1");
		GlobalUnlock(hData);
	}
	if (SetClipboardData(CF_UNICODETEXT, hData) == NULL)
	{
		CloseClipboard();
		window->MessageBox(L"无法将局面复制到剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	window->MessageBox(L"局面已经复制到剪贴板", L"复制", MB_ICONINFORMATION | MB_OK);
	CloseClipboard();
}

void CAccessZler::OnMenuPaste()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		window->MessageBox(L"剪贴板中的数据格式不对", L"粘贴", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!OpenClipboard(NULL))
	{
		window->MessageBox(L"无法打开剪贴板", L"粘贴", MB_ICONWARNING | MB_OK);
		return;
	}
	HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
	if (hMem)
	{
		if (!LoadByFen((TCHAR *)GlobalLock(hMem)))
		{
			//失败时解锁并关闭剪贴板
			GlobalUnlock(hMem);
			CloseClipboard();
			window->MessageBox(L"FEN格式错误", L"加载", MB_ICONWARNING | MB_OK);
			return;
		}
		OnLoadSuccess();
		GlobalUnlock(hMem);
	}
	CloseClipboard();
}

void CAccessZler::OnLoadSuccess()
{
	//置相关状态
	g_pGameTable->SetAction(0);

	SendViewInd_LoadState();
	SendMenuInd_InitState();
}

void CAccessZler::SendViewInd_LoadState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CAccessZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_INIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
