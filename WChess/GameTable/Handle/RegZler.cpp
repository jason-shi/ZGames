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

	//��ȡ���к�
	TCHAR wcsBuff[1024];
	FILE *fp = _wfopen(L"����\\WChess���к�.txt", L"r, ccs=UTF-8");
	fgetws(wcsBuff, 1024, fp);
	fclose(fp);

	//׷�ӵ����кŴ洢�ļ�
	FILE *fp2 = _wfopen(L"����\\WChess���кż�.txt", L"a+, ccs=UTF-8");
	fputws(wcsBuff, fp2);
	fputws(L"\n", fp2);
	fclose(fp2);

	char szBuff[1024];
	WideCharToMultiByte(CP_ACP, 0, wcsBuff, -1, szBuff, 1024, NULL, NULL);

	//��Ӳ�����к�����ע����
	string Ms(szBuff);

	CRSACrypt tRSACrypt;
	string Cs = tRSACrypt.Encrypt(Ms);

	FILE *fp3 = _wfopen(L"����\\WChessע����.dat", L"wb+");
	fwrite(Cs.c_str(), 1, Cs.size(), fp3);
	fclose(fp3);
}

void CRegZler::TrapRegister()
{
	static bool s_bRegistered = false;
	if (!s_bRegistered)
	{
		//����Ƿ��Ѿ�ע��
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
	_wchdir(ModulePath);
	FILE *fp = _wfopen(L"WChessע����.dat", L"rb");
	if (fp == NULL)
	{
		g_pGameTable->MessageBox(wcsErrMsg, L"��Ԫע��", MB_ICONINFORMATION | MB_OK);
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
		g_pGameTable->MessageBox(wcsErrMsg, L"��Ԫע��", MB_ICONINFORMATION | MB_OK);
		return false;
	}
	return true;
}
