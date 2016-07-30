#include "StdAfx.h"
#include "Gate.h"
#include "GatePanel.h"
#include "GameTable.h"
#include <boost\bind.hpp>

using namespace std;

static TCHAR		 wcsBuff[1024];

CGatePanel::CGatePanel() : CPanel()
{
	for (int i = 0; i < 50; i++)
		m_pGate[i] = NULL;

	Set(NULL, CPoint(10, 386), CPoint(0, 0), CSize(500, 72), 1);
}

CGatePanel::~CGatePanel()
{
	for (int i = 0; i < 50; i++)
	{
		delete m_pGate[i];
		m_pGate[i] = NULL;
	}

	delete m_pLblHint;
	m_pLblHint = NULL;

	delete m_pLblSuccess;
	m_pLblSuccess = NULL;
}

bool CGatePanel::Initial()
{
	if (!CPanel::Initial())
		return false;

	for (int nGate = 0; nGate < 50; nGate++)
	{
		m_pGate[nGate] = CreateGate(nGate);
		m_pGate[nGate]->SetMouseProc(MP_LCLICK, boost::bind(&CGatePanel::OnLClick, this, _1));

		AddObject((CObject *)m_pGate[nGate]);
	}

	//创建标签
	m_pLblHint = new CTextBlk(false, 16, CPoint(10, 0), CSize(300, 16), 1);
	if (!m_pLblHint || !m_pLblHint->Initial())
		return false;
	m_pLblHint->ClearImage();
	AddObject(m_pLblHint);

	m_pLblSuccess = new CTextBlk(false, 16, CPoint(340, 0), CSize(60, 16), 1);
	if (!m_pLblSuccess || !m_pLblSuccess->Initial())
		return false;
	m_pLblSuccess->ClearImage();
	AddObject(m_pLblSuccess);

	return true;
}

void CGatePanel::OnStartGame()
{
	LoadGateStatus();

	m_nCurGate = 0;
	m_pGate[m_nCurGate]->OnEnterGate();
}

void CGatePanel::OnStopGame()
{
	m_pGate[m_nCurGate]->OnLeaveGate();
}

void CGatePanel::OnSwitchGate(int nGate)
{
	if (!m_pGate[nGate]->CanAccess())
		return;

	m_pGate[m_nCurGate]->OnLeaveGate();

	m_nCurGate = nGate;
	m_pGate[m_nCurGate]->OnEnterGate();
}

void CGatePanel::LoadGateStatus()
{
	FILE *fp = _wfopen(L"Config.dat", L"r, ccs=UTF-8");
	while (fgetws(wcsBuff, 1024, fp))
	{
		byte nGate = (wcsBuff[0] - L'0') * 10 + wcsBuff[1] - L'0';
		byte nStatus = wcsBuff[2] - L'0';
		m_pGate[nGate]->SetStatus(nStatus);
		//用以更新关卡数字
		m_pGate[nGate]->ShowNumber(false);
	}
	fclose(fp);
}

void CGatePanel::SaveGateStatus()
{
	_wremove(L"Config2.dat");
	_wrename(L"Config.dat", L"Config2.dat");

	FILE *fp = _wfopen(L"Config.dat", L"w+, ccs=UTF-8");
	FILE *fp2 = _wfopen(L"Config2.dat", L"r, ccs=UTF-8");

	while (fgetws(wcsBuff, 1024, fp2))
	{
		byte nGate = (wcsBuff[0] - L'0') * 10 + wcsBuff[1] - L'0';
		wcsBuff[2] = m_pGate[nGate]->GetStatus() + L'0';

		//更新状态后回写
		fputws(wcsBuff, fp);
	}

	fclose(fp);
	fclose(fp2);
	_wremove(L"Config2.dat");
}

void CGatePanel::RevealGate()
{
	bool bRevealGate = false;

	//更新可以访问的关卡
	int nMaxDelta = m_nCurGate / 10 + 1;
	for (int nDelta = 1; nDelta <= nMaxDelta; nDelta++)
	{
		if (m_nCurGate + nDelta < 50)
		{
			if (!m_pGate[m_nCurGate + nDelta]->CanAccess())
			{
				m_pGate[m_nCurGate + nDelta]->SetStatus(GT_ACCESS);
				m_pGate[m_nCurGate + nDelta]->ShowNumber(false);

				bRevealGate = true;
			}
		}
	}

	//保存所有关卡的状态
	if (bRevealGate)
		SaveGateStatus();
}

void CGatePanel::ProcessResult(bool bSuccess)
{
	if (bSuccess)
	{
		m_pGate[m_nCurGate]->SetStatus(GT_SOLVED);
		RevealGate();
	}
	SetSuccess(bSuccess);
}

void CGatePanel::SetHint(const TCHAR* pHint)
{
	static HFONT hFont = CreateFont(14, 14, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, L"ArialBD.ttf");

	m_pLblHint->OutText(pHint, wcslen(pHint), hFont, true, BGRA(0, 0, 0, 255));
}

void CGatePanel::SetSuccess(bool bSuccess)
{
	static HFONT hFont = CreateFont(14, 14, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, L"ArialBD.ttf");
	static wstring s_szLast = L"";

	wstring szSuccess = L"";

	//已解决过
	if (m_pGate[m_nCurGate]->IsSolved())
		szSuccess = bSuccess ? L"You win!" : L"(won)";

	if (szSuccess.compare(s_szLast) != 0)
	{
		m_pLblSuccess->OutText(szSuccess.c_str(), wcslen(szSuccess.c_str()), hFont, true, BGRA(0, 0, 0, 255));
		s_szLast = szSuccess;
	}
}

void CGatePanel::OnLClick(CObject *pGate)
{
	int nGate = ((CGate *)pGate)->GetGate();
	OnSwitchGate(nGate);

	g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
}
