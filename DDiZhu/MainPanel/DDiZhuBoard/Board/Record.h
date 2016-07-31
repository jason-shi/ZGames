#ifndef __Record_h__
#define __Record_h__

struct TPutRecord {
	byte			 byChair;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
};

class CRecordFile {
  public:
	CRecordFile(TCHAR wcsFileName[_MAX_PATH])
	{
		fp = _wfopen(wcsFileName, L"a+, ccs=UTF-8");
	}
	~CRecordFile()
	{
		fclose(fp);
		fp = NULL;
	}

  public:
	FILE			*fp;
};

class CRecord {
  public:
	CRecord(TCHAR wcsFileName[_MAX_PATH]);
	virtual ~CRecord();

  public:
	void			 Clear();

  public:
	void			 WritePut(TPutRecord *pPutRecord);
	void			 WriteGvp();
	void			 WriteUndo();

  public:
	TCHAR			 m_wcsFileName[_MAX_PATH];
};

#endif
