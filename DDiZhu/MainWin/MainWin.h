#ifndef	__MainWin_h__
#define	__MainWin_h__

#define WM_FIRST_ACTION		(WM_USER + 100)

class CMainPanel;
class CMainWin : public CWindow {
  public:
	CMainWin() {}
	virtual LRESULT		 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
	virtual BOOL		 PreCreateWindow(CREATESTRUCT &cs);
	virtual BOOL		 OnCreate(CREATESTRUCT *cs);
	virtual BOOL		 OnIdle(long count);
	virtual bool		 OnFirstAction();
	virtual void		 OnClose();
	virtual void		 OnPaint();
	virtual void		 OnCommand(UINT notifyCode, UINT id, HWND ctrl);
	virtual void		 OnAnimeEnd(CAnime *pAnime);

  protected:
	CDrawImage			 ViewImage;				 //���ڻ���
	CImage				 BackImage;				 //���ڱ���
	CMainPanel			*m_pMainPanel;           //��Ϸ�����
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
