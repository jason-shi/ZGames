#ifndef __MirChoice_h__
#define __MirChoice_h__

class CMirChoice : public CDialog {
  public:
	CMirChoice(CPoint xDrawPos);
	virtual ~CMirChoice();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	INT_PTR			 DialogResult();

  public:
	void			 OnLClick(UINT nFlags, CPoint point);

  protected:
	int				 type_dir;
};

#endif
