#include "StdAfx.h"
#include "RegZler.h"
#include "GameTable.h"

using namespace std;

CRegZler::CRegZler(CWindow *_window) : ICmdZler()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CRegZler::~CRegZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CRegZler::ProcessCmd(int nType)
{
	switch (nType)
	{
#ifdef _DEBUG
	case ID_APP_REGISTER:
		OnMenuRegister();
		break;
#endif

	default:
		return false;
	}
	return true;
}

void CRegZler::OnMenuRegister()
{
	_wchdir(ModulePath);

	//读取序列号
	TCHAR wcsBuff[1024];
	FILE *fp = _wfopen(L"绝密\\WChess序列号.txt", L"r, ccs=UTF-8");
	fgetws(wcsBuff, 1024, fp);
	fclose(fp);

	//追加到序列号存储文件
	FILE *fp2 = _wfopen(L"绝密\\WChess序列号集.txt", L"a+, ccs=UTF-8");
	fputws(wcsBuff, fp2);
	fputws(L"\n", fp2);
	fclose(fp2);

	char szBuff[1024];
	WideCharToMultiByte(CP_ACP, 0, wcsBuff, -1, szBuff, 1024, NULL, NULL);

	//由硬盘序列号生成注册码
	string Ms(szBuff);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(Ms);

	FILE *fp3 = _wfopen(L"绝密\\WChess注册码.dat", L"wb+");
	fwrite(Cs.c_str(), 1, Cs.size(), fp3);
	fclose(fp3);
}

void CRegZler::TrapRegister()
{
	static bool s_bRegistered = false;
	if (!s_bRegistered)
	{
		//检查是否已经注册
		if (!CheckRegister())
		{
			g_pGameTable->PostMessage(WM_CLOSE);
			return;
		}
		s_bRegistered = true;
	}
}

bool CRegZler::CheckRegister()
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
	_wchdir(ModulePath);
	FILE *fp = _wfopen(L"WChess注册码.dat", L"rb");
	if (fp == NULL)
	{
		g_pGameTable->MessageBox(wcsErrMsg, L"百元注册", MB_ICONINFORMATION | MB_OK);
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
		g_pGameTable->MessageBox(wcsErrMsg, L"百元注册", MB_ICONINFORMATION | MB_OK);
		return false;
	}
	return true;
}
