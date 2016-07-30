#ifndef __ArrangeZler_h__
#define __ArrangeZler_h__

class CArrangeZler : public ICmdZler {
  public:
	CArrangeZler(CWindow *_window);
	virtual ~CArrangeZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuArrange();

  public:
	void			 SendViewInd_ArrangeState();
	void			 SendMenuInd_ArrangeState();

  private:
	CWindow			*window;
};

#endif
