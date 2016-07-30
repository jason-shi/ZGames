#ifndef __BatchZler_h__
#define __BatchZler_h__

#include "ZlerBase.h"

class CBatchZler : public CZlerBase {
  public:
	CBatchZler(CWindow *_window);
	virtual ~CBatchZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuBatch();
	void			 OnMenuExit();

  public:
	void			 FetchElement();
	void			 CrackElement(const TCHAR* wcsFileName);
	void			 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	void			 OnCrackTout();
	void			 KillAllTimer();
	void			 SaveToLog(int nResult);
	void			 OnBatchOver();
	void			 SendViewInd_InitState();
	void			 SendViewInd_BatchState();
	void			 SendMenuInd_InitState();
	void			 SendMenuInd_CrackState();

  private:
	CWindow			*window;
	long			 m_hFile;
	_wfinddata_t	 m_tFindData;
};

#endif
