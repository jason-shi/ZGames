#ifndef	__MainWin_h__
#define	__MainWin_h__

#include "Window.h"
#include "DrawImage.h"
#include "dc.h"

class CMainWin : public CWindow {
  public:
	CMainWin() {}
	virtual LRESULT	 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
	virtual BOOL	 PreCreateWindow(CREATESTRUCT &cs);
	virtual BOOL	 OnCreate(CREATESTRUCT *cs);
	virtual bool	 OnFirstAction();
	virtual void	 OnClose();
	virtual void	 OnPaint();
	virtual void	 OnAnimeEnd(CAnime *pAnime);
	BOOL			 OnIdle(long count);

  public:
	void			 EnableMenu(bool bIncr);
	void			 SendMenuInd_InitState();

  protected:
	CDrawImage		 ViewImage;				 //´°¿Ú»­²¼
	CImage			 BackImage;				 //´°¿Ú±³¾°
};

inline void CMainWin::OnAnimeEnd(CAnime *pAnime)
{
	FN_AnimeEnd EndProc = pAnime->GetEndProc();
	if (EndProc)
		EndProc(pAnime);
}

inline BOOL CMainWin::OnIdle(long count)
{
	return count >= 100;
}

#endif
