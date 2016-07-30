#ifndef __SplashWin_h__
#define __SplashWin_h__

#include "Window.h"

class CSplashWin : public CWindow {
  public:
	CSplashWin() {}
	virtual BOOL		 PreCreateWindow(CREATESTRUCT &cs);

  public:
	LRESULT				 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
	BOOL				 OnCreate(CREATESTRUCT *cs);
	bool				 OnFirstAction();
	void				 OnClose();
	void				 OnDestroy();
	void				 OnPaint();

  protected:
	CDrawImage			 ViewImage; //´°¿Ú»­²¼
	CImage				 BackImage; //´°¿Ú±³¾°
};

extern CSplashWin		 SplashWin;

#endif
