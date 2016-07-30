#include "StdAfx.h"
#include "Gate.h"
#include "Mirror\MirBoard.h"
#include "Mirror\MirBox.h"

static TCHAR		 wcsBuff[1024];

CGate::CGate(int nGate) : m_nGate(nGate), CPanel()
{
	int nX = m_nGate % 25;
	int nY = m_nGate / 25;

	Set(NULL, CPoint(nX * 20, nY * 20 + 22), CPoint(0, 0), CSize(20, 20), 0);
	SetCease();
}

CGate::~CGate()
{
}

bool CGate::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_nStatus = GT_CONCEAL;

	//关数的个位和十位
	int nShowGate = m_nGate + 1;
	int nDight1 = nShowGate % 10;
	int nDight2 = nShowGate / 10;

	if (nShowGate < 10)
	{
		SpriteNumber1.Set(&ImageGate, CPoint(6, 0), CPoint(nDight1 * 9, 0), CSize(9, 12), 0);
		AddObject(&SpriteNumber1);
	}
	else
	{
		SpriteNumber2.Set(&ImageGate, CPoint(2, 0), CPoint(nDight2 * 9, 0), CSize(9, 12), 0);
		SpriteNumber1.Set(&ImageGate, CPoint(9, 0), CPoint(nDight1 * 9, 0), CSize(9, 12), 0);
		AddObject(&SpriteNumber2);
		AddObject(&SpriteNumber1);
	}
	return true;
}

void CGate::ShowNumber(bool bCurrent)
{
	//当前关偏移到白色字体
	int nOffset = bCurrent ? GT_CURRENT : m_nStatus;

	//关数的个位和十位
	int nShowGate = m_nGate + 1;
	int nDight1 = nShowGate % 10;
	int nDight2 = nShowGate / 10;

	if (nShowGate < 10)
	{
		SpriteNumber1.SetSrcPos(nDight1 * 9, nOffset * 12);
	}
	else
	{
		SpriteNumber1.SetSrcPos(nDight1 * 9, nOffset * 12);
		SpriteNumber2.SetSrcPos(nDight2 * 9, nOffset * 12);
	}
}

void CGate::LoadGate()
{
	g_pMirBoard->ClearMirror();
	g_pMirBox->ClearMirror();

	FILE *fp = _wfopen(L"Config.dat", L"r, ccs=UTF-8");
	while (fgetws(wcsBuff, 1024, fp))
	{
		if ((wcsBuff[0] - L'0') * 10 + wcsBuff[1] - L'0' == m_nGate)
		{
			SetStatus(wcsBuff[2] - L'0');

			TCHAR *pBuff = wcsBuff + 3;
			g_pMirBoard->ReadMirrorData(pBuff);
			g_pMirBox->ReadMirrorData(pBuff);
			break;
		}
	}
	fclose(fp);
}

void CGate::SaveGate()
{
	_wremove(L"Config2.dat");
	_wrename(L"Config.dat", L"Config2.dat");

	FILE *fp = _wfopen(L"Config.dat", L"w+, ccs=UTF-8");
	FILE *fp2 = _wfopen(L"Config2.dat", L"r, ccs=UTF-8");

	while (fgetws(wcsBuff, 1024, fp2))
	{
		if ((wcsBuff[0] - L'0') * 10 + wcsBuff[1] - L'0' != m_nGate)
		{
			//复制非本关数据
			fputws(wcsBuff, fp);
		}
	}
	//在文件尾加上本关数据
	fputws(WriteGateData(), fp);
	fputws(L"\n", fp);

	fclose(fp);
	fclose(fp2);
	_wremove(L"Config2.dat");
}

TCHAR *CGate::WriteGateData()
{
	wcsBuff[0] = m_nGate / 10 + L'0';
	wcsBuff[1] = m_nGate % 10 + L'0';
	wcsBuff[2] = m_nStatus + L'0';

	TCHAR *pBuff = wcsBuff + 3;
	g_pMirBoard->WriteMirrorData(pBuff);
	g_pMirBox->WriteMirrorData(pBuff);

	return wcsBuff;
}

void CGate::OnLeaveGate()
{
	SaveGate();
	ShowNumber(false);
}

void CGate::OnEnterGate()
{
	LoadGate();
	ShowNumber(true);
}

CGate *CreateGate(int nGate)
{
	CGate *pGate = new CGate(nGate);
	if (!pGate || !pGate->Initial())
		return NULL;

	return pGate;
}
