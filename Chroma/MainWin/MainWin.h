#ifndef	__MainWin_h__
#define	__MainWin_h__

#define ClientWidth				634
#define ClientHeight			478

class CMainWin : public CWindow {
  public:
	CMainWin() {}
	virtual LRESULT	 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
	virtual BOOL	 PreCreateWindow(CREATESTRUCT &cs);
	virtual BOOL	 OnCreate(CREATESTRUCT *cs);
	virtual BOOL	 OnIdle(long count);
	virtual bool	 OnFirstAction();
	virtual void	 OnClose();
	virtual void	 OnPaint();
	virtual void	 OnAnimeEnd(CAnime *pAnime);

  protected:
	CDrawImage		 ViewImage; //´°¿Ú»­²¼
	CImage			 BackImage; //´°¿Ú±³¾°
};

inline BOOL CMainWin::OnIdle(long count)
{
	return count >= 100;
}

inline void CMainWin::OnAnimeEnd(CAnime *pAnime)
{
	FN_AnimeEnd EndProc = pAnime->GetEndProc();
	if (EndProc)
		EndProc(pAnime);
}

#endif
