#include "StdAfx.h"
#include "GameTable.h"
#include "Mirror\MirBoard.h"
#include "Mirror\MirBox.h"
#include "Mirror\MirChoice.h"
#include "Mirror\MirAction.h"
#include "Gate\GatePanel.h"

using namespace std;

CGameTable::CGameTable() : CMainWin()
{
}

CGameTable::~CGameTable()
{
}

bool CGameTable::Initial()
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

	if (!LoadImage())
		return false;

	if (!g_pMirBoard->Initial())
		return false;
	g_pZ2coo->AddObject(g_pMirBoard);

	if (!g_pMirBox->Initial())
		return false;
	g_pZ2coo->AddObject(g_pMirBox);

	if (!g_pGatePanel->Initial())
		return false;
	g_pZ2coo->AddObject(g_pGatePanel);

	m_pMirAction = new CMirAction();
	if (!m_pMirAction)
		return false;

	return true;
}

void CGameTable::Release()
{
	delete m_pMirAction;
	m_pMirAction = NULL;

	g_pZ2coo->Release();
	g_pZ2mov->Release();
	g_pZTimer->Release();
}

LRESULT	CGameTable::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:
		OnTimer(wParam);
		break;

	case WM_UPDATE_LIGHT:
		OnUpdateLight();
		break;

	default:
		return CMainWin::WindowProc(uMsg, wParam, lParam);
	}
	return 0L;
}

void CGameTable::OnCommand(UINT notifyCode, UINT id, HWND ctrl)
{
	//控件消息不处理
	if (ctrl != NULL)
		return;

	//退出处理
	switch (id)
	{
	case ID_APP_COPY:
		OnAppCopy();
		break;

	case ID_APP_PASTE:
		OnAppPaste();
		break;

	case ID_APP_EDIT:
		OnAppEdit();
		break;

#ifdef _DEBUG
	case ID_APP_REGISTER:
		OnAppRegister();
		break;
#endif

	case ID_APP_EXIT:
		SendMessage(WM_CLOSE);
		break;
	}
}

bool CGameTable::OnFirstAction()
{
	if (!CMainWin::OnFirstAction())
		return false;

	//创建表格
	CreateTables();

	//启动直接进入游戏主界面
	OnStartGame();
	return true;
}

void CGameTable::OnClose()
{
	OnStopGame();

	return CMainWin::OnClose();
}

void CGameTable::OnTimer(WPARAM wParam)
{
}

void CGameTable::OnAppEdit()
{
	g_bEditState = !g_bEditState;

	//进入编辑状态不固定，离开编辑状态固定
	g_pMirBoard->SetFixed(!g_bEditState);
}

void CGameTable::OnAppCopy()
{
	if (!OpenClipboard(NULL))
	{
		MessageBox(L"无法打开剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!EmptyClipboard())
	{
		MessageBox(L"无法清除剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	HGLOBAL hData = GlobalAlloc(LMEM_FIXED|LMEM_ZEROINIT, 1024);
	{
		TCHAR *pBuff = (TCHAR *)GlobalLock(hData);
		//获取ZChroma串
		g_pMirBoard->WriteMirrorData(pBuff);
		g_pMirBox->WriteMirrorData(pBuff);
		GlobalUnlock(hData);
	}
	if (SetClipboardData(CF_UNICODETEXT, hData) == NULL)
	{
		CloseClipboard();
		MessageBox(L"无法将局面复制到剪贴板", L"复制", MB_ICONWARNING | MB_OK);
		return;
	}
	CloseClipboard();
	MessageBox(L"局面已经复制到剪贴板", L"复制", MB_ICONINFORMATION | MB_OK);
}

void CGameTable::OnAppPaste()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		MessageBox(L"剪贴板中的数据格式不对", L"粘贴", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!OpenClipboard(NULL))
	{
		MessageBox(L"无法打开剪贴板", L"粘贴", MB_ICONWARNING | MB_OK);
		return;
	}
	HGLOBAL hMem = GetClipboardData(CF_UNICODETEXT);
	if (hMem)
	{
		g_pMirBoard->ClearMirror();
		g_pMirBox->ClearMirror();

		TCHAR *pBuff = (TCHAR *)GlobalLock(hMem);
		if (!g_pMirBoard->ReadMirrorData(pBuff)
			|| !g_pMirBox->ReadMirrorData(pBuff))
		{
			//失败时解锁并关闭剪贴板
			GlobalUnlock(hMem);
			CloseClipboard();
			MessageBox(L"ZChroma格式错误", L"加载", MB_ICONWARNING | MB_OK);
			return;
		}
		GlobalUnlock(hMem);
	}
	CloseClipboard();

	SendMessage(WM_UPDATE_LIGHT);
}

void CGameTable::OnAppRegister()
{
	//读取序列号
	TCHAR wcsBuff[1024];
	FILE *fp = _wfopen(L"绝密\\Chroma序列号.txt", L"r, ccs=UTF-8");
	fgetws(wcsBuff, 1024, fp);
	fclose(fp);

	//追加到序列号存储文件
	FILE *fp2 = _wfopen(L"绝密\\Chroma序列号集.txt", L"a+, ccs=UTF-8");
	fputws(wcsBuff, fp2);
	fputws(L"\n", fp2);
	fclose(fp2);

	char szBuff[1024];
	WideCharToMultiByte(CP_ACP, 0, wcsBuff, -1, szBuff, 1024, NULL, NULL);

	//由硬盘序列号生成注册码
	string Ms(szBuff);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(Ms);

	FILE *fp3 = _wfopen(L"绝密\\Chroma注册码.dat", L"wb+");
	fwrite(Cs.c_str(), 1, Cs.size(), fp3);
	fclose(fp3);
}

void CGameTable::TrapRegister()
{
#if 0
	static bool s_bRegistered = false;
	if (!s_bRegistered)
	{
		//检查是否已经注册
		if (!CheckRegister())
		{
			PostMessage(WM_CLOSE);
			return;
		}
		s_bRegistered = true;
	}
#endif
}

bool CGameTable::CheckRegister()
{
	//获取硬盘序列号
	CHDSerial tHDSerial;
	string HDSerial = tHDSerial.GetHDSerial();

	//序列号转换成Unicode码
	TCHAR wcsHDSerial[256];
	MultiByteToWideChar(CP_ACP, 0, HDSerial.c_str(), -1, wcsHDSerial, sizeof(wcsHDSerial) / sizeof(WCHAR));

	TCHAR wcsErrMsg[1024] = { 0 };
	wsprintf(wcsErrMsg, L"%s请注册，注册后即可永久使用！\n", wcsErrMsg);
	wsprintf(wcsErrMsg, L"%s请将\"%s\"发送至：QQ 173578371\n", wcsErrMsg, wcsHDSerial);
	wsprintf(wcsErrMsg, L"%s\n", wcsErrMsg);
	wsprintf(wcsErrMsg, L"%s        感谢您的注册！\n", wcsErrMsg);

	//读取注册码
	FILE *fp = _wfopen(L"Chroma注册码.dat", L"rb");
	if (fp == NULL)
	{
		MessageBox(wcsErrMsg, L"十元注册", MB_ICONINFORMATION | MB_OK);
		return false;
	}
	char szBuff[1024];
	int nLength = fread(szBuff, 1, 1024, fp);
	fclose(fp);

	//由硬盘序列号生成注册码
	string Cs_Reg(szBuff, nLength);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(HDSerial);

	if (Cs.compare(Cs_Reg) != 0)
	{
		MessageBox(wcsErrMsg, L"十元注册", MB_ICONINFORMATION | MB_OK);
		return false;
	}
	return true;
}

void CGameTable::OnStartGame()
{
	g_pGatePanel->OnStartGame();

	SendMessage(WM_UPDATE_LIGHT);
}

void CGameTable::OnStopGame()
{
	g_pGatePanel->OnStopGame();

	g_pMirBoard->ClearMirror();
	g_pMirBox->ClearMirror();
}

void CGameTable::OnUpdateLight()
{
	g_pMirBoard->UpdateLight();

	bool bSuccess = g_pMirBoard->IsSuccess();
#if 0 //取消注册
	if (bSuccess)
	{
#ifndef _DEBUG
		//一旦过关，则进入注册陷阱
		//TrapRegister();
#endif
	}
#endif
	g_pGatePanel->ProcessResult(bSuccess);
}
