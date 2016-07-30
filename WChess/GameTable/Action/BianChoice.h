#ifndef __BianChoice_h__
#define __BianChoice_h__

class CBianChoice : public CDialog {
  public:
	CBianChoice(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos);
	virtual ~CBianChoice();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	INT_PTR			 DialogResult();

  public:
	void			 OnLClick(UINT nFlags, CPoint point);

  protected:
	int				 m_nBian;
};

#endif
