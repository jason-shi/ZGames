#ifndef __MoonRaise_h__
#define __MoonRaise_h__

class CMoonRaise : public CButton {
  public:
	CMoonRaise(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CMoonRaise();

  public:
	void			 OnTimer();

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);
};

#endif
