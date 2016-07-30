#ifndef __ZlerBase_h__
#define __ZlerBase_h__

class CZlerBase : public ICmdZler {
  public:
	CZlerBase();
	virtual ~CZlerBase();

  public:
	virtual void	 OnMenuExit();
	virtual void	 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	virtual void	 OnCrackTout();
	virtual void	 KillAllTimer();
};

#endif
