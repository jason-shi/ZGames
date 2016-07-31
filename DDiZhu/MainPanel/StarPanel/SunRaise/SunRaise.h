#ifndef __SunRaise_h__
#define __SunRaise_h__

class CSunRaise : public CButton {
  public:
	CSunRaise(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CSunRaise();

  public:
	void			 OnTimer();

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);
};

#endif
