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
	//�ؼ���Ϣ������
	if (ctrl != NULL)
		return;

	//�˳�����
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

	//�������
	CreateTables();

	//����ֱ�ӽ�����Ϸ������
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

	//����༭״̬���̶����뿪�༭״̬�̶�
	g_pMirBoard->SetFixed(!g_bEditState);
}

void CGameTable::OnAppCopy()
{
	if (!OpenClipboard(NULL))
	{
		MessageBox(L"�޷��򿪼�����", L"����", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!EmptyClipboard())
	{
		MessageBox(L"�޷����������", L"����", MB_ICONWARNING | MB_OK);
		return;
	}
	HGLOBAL hData = GlobalAlloc(LMEM_FIXED|LMEM_ZEROINIT, 1024);
	{
		TCHAR *pBuff = (TCHAR *)GlobalLock(hData);
		//��ȡZChroma��
		g_pMirBoard->WriteMirrorData(pBuff);
		g_pMirBox->WriteMirrorData(pBuff);
		GlobalUnlock(hData);
	}
	if (SetClipboardData(CF_UNICODETEXT, hData) == NULL)
	{
		CloseClipboard();
		MessageBox(L"�޷������渴�Ƶ�������", L"����", MB_ICONWARNING | MB_OK);
		return;
	}
	CloseClipboard();
	MessageBox(L"�����Ѿ����Ƶ�������", L"����", MB_ICONINFORMATION | MB_OK);
}

void CGameTable::OnAppPaste()
{
	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		MessageBox(L"�������е����ݸ�ʽ����", L"ճ��", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!OpenClipboard(NULL))
	{
		MessageBox(L"�޷��򿪼�����", L"ճ��", MB_ICONWARNING | MB_OK);
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
			//ʧ��ʱ�������رռ�����
			GlobalUnlock(hMem);
			CloseClipboard();
			MessageBox(L"ZChroma��ʽ����", L"����", MB_ICONWARNING | MB_OK);
			return;
		}
		GlobalUnlock(hMem);
	}
	CloseClipboard();

	SendMessage(WM_UPDATE_LIGHT);
}

void CGameTable::OnAppRegister()
{
	//��ȡ���к�
	TCHAR wcsBuff[1024];
	FILE *fp = _wfopen(L"����\\Chroma���к�.txt", L"r, ccs=UTF-8");
	fgetws(wcsBuff, 1024, fp);
	fclose(fp);

	//׷�ӵ����кŴ洢�ļ�
	FILE *fp2 = _wfopen(L"����\\Chroma���кż�.txt", L"a+, ccs=UTF-8");
	fputws(wcsBuff, fp2);
	fputws(L"\n", fp2);
	fclose(fp2);

	char szBuff[1024];
	WideCharToMultiByte(CP_ACP, 0, wcsBuff, -1, szBuff, 1024, NULL, NULL);

	//��Ӳ�����к�����ע����
	string Ms(szBuff);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(Ms);

	FILE *fp3 = _wfopen(L"����\\Chromaע����.dat", L"wb+");
	fwrite(Cs.c_str(), 1, Cs.size(), fp3);
	fclose(fp3);
}

void CGameTable::TrapRegister()
{
#if 0
	static bool s_bRegistered = false;
	if (!s_bRegistered)
	{
		//����Ƿ��Ѿ�ע��
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
	//��ȡӲ�����к�
	CHDSerial tHDSerial;
	string HDSerial = tHDSerial.GetHDSerial();

	//���к�ת����Unicode��
	TCHAR wcsHDSerial[256];
	MultiByteToWideChar(CP_ACP, 0, HDSerial.c_str(), -1, wcsHDSerial, sizeof(wcsHDSerial) / sizeof(WCHAR));

	TCHAR wcsErrMsg[1024] = { 0 };
	wsprintf(wcsErrMsg, L"%s��ע�ᣬע��󼴿�����ʹ�ã�\n", wcsErrMsg);
	wsprintf(wcsErrMsg, L"%s�뽫\"%s\"��������QQ 173578371\n", wcsErrMsg, wcsHDSerial);
	wsprintf(wcsErrMsg, L"%s\n", wcsErrMsg);
	wsprintf(wcsErrMsg, L"%s        ��л����ע�ᣡ\n", wcsErrMsg);

	//��ȡע����
	FILE *fp = _wfopen(L"Chromaע����.dat", L"rb");
	if (fp == NULL)
	{
		MessageBox(wcsErrMsg, L"ʮԪע��", MB_ICONINFORMATION | MB_OK);
		return false;
	}
	char szBuff[1024];
	int nLength = fread(szBuff, 1, 1024, fp);
	fclose(fp);

	//��Ӳ�����к�����ע����
	string Cs_Reg(szBuff, nLength);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(HDSerial);

	if (Cs.compare(Cs_Reg) != 0)
	{
		MessageBox(wcsErrMsg, L"ʮԪע��", MB_ICONINFORMATION | MB_OK);
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
#if 0 //ȡ��ע��
	if (bSuccess)
	{
#ifndef _DEBUG
		//һ�����أ������ע������
		//TrapRegister();
#endif
	}
#endif
	g_pGatePanel->ProcessResult(bSuccess);
}
