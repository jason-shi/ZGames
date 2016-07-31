#include "StdAfx.h"
#include "Record.h"

CRecord::CRecord(TCHAR wcsFileName[_MAX_PATH])
{
	wcscpy(m_wcsFileName, wcsFileName);
}

CRecord::~CRecord()
{
}

void CRecord::Clear()
{
	FILE *fp = _wfopen(m_wcsFileName, L"w+, ccs=UTF-8");
	fclose(fp);
}

void CRecord::WritePut(TPutRecord *pPutRecord)
{
	CRecordFile tFile(m_wcsFileName);

	TCHAR wcsBuff[512];

	wsprintf(wcsBuff, L"+%02X,%02X,%02X,%02X\n",
					  pPutRecord->byChair,
					  pPutRecord->byRspType,
					  pPutRecord->byGrade,
					  pPutRecord->byCount);
	fputws(wcsBuff, tFile.fp);
}

void CRecord::WriteGvp()
{
	CRecordFile tFile(m_wcsFileName);

	fputws(L"*\n", tFile.fp);
}

void CRecord::WriteUndo()
{
	CRecordFile tFile(m_wcsFileName);

	fputws(L"-\n", tFile.fp);
}
