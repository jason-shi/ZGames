#ifndef __DDiZhuReplay_h__
#define __DDiZhuReplay_h__

class CDDiZhuReplay {
  public:
	CDDiZhuReplay();
	~CDDiZhuReplay();

  public:
	bool			 Open(TCHAR wcsFileName[_MAX_PATH]);
	void			 Close();

  public:
	bool			 ReadType(byte& byType);

  public:
	void			 ReadDiZhu(byte& byChair);
	void			 ReadPut(TPutReplay *pPutReplay);
	void			 ReadGvp();
	void			 ReadUndo();

  protected:
	FILE			*m_fp;
};

#endif
