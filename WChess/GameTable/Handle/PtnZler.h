#ifndef __PatternZler_h__
#define __PatternZler_h__

#include "ZlerBase.h"

class CPtnZler : public CZlerBase {
  public:
	CPtnZler(CWindow *_window);
	virtual ~CPtnZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuPattern();
	void			 OnMenuExit();

  public:
	void			 ProcessPattern(bool bWeaken);
	void			 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	void			 OnCrackTout();
	void			 KillAllTimer();
	bool			 SavePtnTitle(TCHAR *wcsPatternPath);
	void			 SaveToLog(int nResult, int nRound);
	void			 SaveToFile();
	void			 SendViewInd_InitState();
	void			 SendViewInd_PtnState();
	void			 SendMenuInd_InitState();
	void			 SendMenuInd_PtnState();

  private:
	CWindow			*window;
	TCHAR			 m_wcsTimeFolder[_MAX_PATH]; //\Pattern\Date_Time
	int				 m_nFileNumber;
};

#endif
