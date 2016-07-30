#include "StdAfx.h"
#include "Manual.h"
#include "ManualZler.h"

CManualZler::CManualZler(CWindow *_window) : ICmdZler()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CManualZler::~CManualZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CManualZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_MANUAL:
		OnMenuManual();
		break;
	default:
		return false;
	}
	return true;
}

void CManualZler::OnMenuManual()
{
	if (!OpenClipboard(NULL))
	{
		window->MessageBox(L"无法打开剪贴板", L"着法", MB_ICONERROR | MB_OK);
		return;
	}
	if (!EmptyClipboard())
	{
		window->MessageBox(L"无法清除剪贴板", L"着法", MB_ICONERROR | MB_OK);
		return;
	}
	HGLOBAL hData = GlobalAlloc(LMEM_MOVEABLE|LMEM_ZEROINIT, MaxCrackDepth * 25);
	{
		TCHAR wcsManual[MaxCrackDepth * 25];
		wcsManual[0] = L'\0';

		TCHAR wcsLine[25];
		TCHAR *p = (TCHAR *)GlobalLock(hData);
		for (size_t i = 0; i < vecManual.size() / 2; i++)
		{
			wsprintf(wcsLine, L"%3d. %s %s\r\n", i+1, vecManual[2 * i].c_str(), vecManual[2 * i + 1].c_str());
			wcscat(wcsManual, wcsLine);
		}
		if ((vecManual.size() & 1) == 1)
		{
			wsprintf(wcsLine, L"%3d. %s", (vecManual.size() + 1) / 2, vecManual.back().c_str());
			wcscat(wcsManual, wcsLine);
		}
		wcscpy(p, wcsManual);
		GlobalUnlock(hData);
	}
	if (SetClipboardData(CF_UNICODETEXT, hData) == NULL)
	{
		window->MessageBox(L"无法将数据复制到剪贴板", L"着法", MB_ICONERROR | MB_OK);
		CloseClipboard();
		return;
	}
	window->MessageBox(L"着法已经复制到剪贴板", L"着法", MB_ICONINFORMATION | MB_OK);
	CloseClipboard();
}
