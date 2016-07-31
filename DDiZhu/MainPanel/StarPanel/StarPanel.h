#ifndef __StarPanel_h__
#define __StarPanel_h__

class CSunRaise;
class CMoonRaise;
class CStarPanel : public CPanel {
  public:
	CStarPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CStarPanel();

  public:
	bool			 Initial();
	void			 OnTimer();

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);

  protected:
	CSunRaise		*m_pSunRaise;
	CMoonRaise		*m_pMoonRaise;
};

#endif
