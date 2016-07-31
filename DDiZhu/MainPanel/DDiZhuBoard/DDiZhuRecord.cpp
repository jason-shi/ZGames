#include "StdAfx.h"
#include "DDiZhuRecord.h"
#include "Board\Card.h"

CDDiZhuRecord::CDDiZhuRecord(TCHAR wcsFileName[_MAX_PATH]) : CRecord(wcsFileName)
{
}

CDDiZhuRecord::~CDDiZhuRecord()
{
}

void CDDiZhuRecord::WriteDiZhu(byte byChair)
{
	CRecordFile tFile(m_wcsFileName);

	TCHAR wcsBuff[512];

	wsprintf(wcsBuff, L"B%d\n", byChair);
	fputws(wcsBuff, tFile.fp);
}
