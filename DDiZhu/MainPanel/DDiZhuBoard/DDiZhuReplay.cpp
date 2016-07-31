#include "StdAfx.h"
#include "DDiZhuReplay.h"
#include "Board\Card.h"

CDDiZhuReplay::CDDiZhuReplay()
{
	m_fp = NULL;
}

CDDiZhuReplay::~CDDiZhuReplay()
{
	Close();
}

bool CDDiZhuReplay::Open(TCHAR wcsFileName[_MAX_PATH])
{
	m_fp = _wfopen(wcsFileName, L"r, ccs=UTF-8");
	return m_fp != NULL;
}

void CDDiZhuReplay::Close()
{
	if (m_fp)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

bool CDDiZhuReplay::ReadType(byte& byType)
{
	TCHAR wcsBuff[128];

	if (!fgetws(wcsBuff, 2, m_fp))
		return false;

	switch (wcsBuff[0])
	{
	case L'B':
		byType = RECORD_DIZHU;
		break;
	case L'+':
		byType = RECORD_PUT;
		break;
	case L'*':
		byType = RECORD_GVP;
		break;
	case L'-':
		byType = RECORD_UNDO;
		break;
	default:
		return false;
	}
	return true;
}

void CDDiZhuReplay::ReadDiZhu(byte& byChair)
{
	TCHAR wcsBuff[128];

	if (fgetws(wcsBuff, 128, m_fp))
	{
		byChair = wcsBuff[0] - L'0';
	}
}

void CDDiZhuReplay::ReadPut(TPutReplay *pPutReplay)
{
	TCHAR wcsBuff[128];

	if (fgetws(wcsBuff, 128, m_fp))
	{
		int i = 0;
		pPutReplay->byChair   = HEX_2_DEC(wcsBuff[i + 0]) * 16 + HEX_2_DEC(wcsBuff[i + 1]);
		pPutReplay->byRspType = HEX_2_DEC(wcsBuff[i + 3]) * 16 + HEX_2_DEC(wcsBuff[i + 4]);
		pPutReplay->byGrade   = HEX_2_DEC(wcsBuff[i + 6]) * 16 + HEX_2_DEC(wcsBuff[i + 7]);
		pPutReplay->byCount   = HEX_2_DEC(wcsBuff[i + 9]) * 16 + HEX_2_DEC(wcsBuff[i + 10]);
	}
}

void CDDiZhuReplay::ReadGvp()
{
	TCHAR wcsBuff[32];

	fgetws(wcsBuff, 32, m_fp); //读取换行符
}

void CDDiZhuReplay::ReadUndo()
{
	TCHAR wcsBuff[32];

	fgetws(wcsBuff, 32, m_fp); //读取换行符
}
