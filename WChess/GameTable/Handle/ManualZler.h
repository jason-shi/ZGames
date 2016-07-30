#ifndef __ManualZler_h__
#define __ManualZler_h__

class CWindow;
class CManualZler : public ICmdZler {
  public:
	CManualZler(CWindow *_window);
	virtual ~CManualZler();

  public:
	bool				 ProcessCmd(int nType);
	void				 OnMenuManual();

  private:
	CWindow				*window;
};

#endif
