#ifndef __RegZler_h__
#define __RegZler_h__

class CRegZler : public ICmdZler {
  public:
	CRegZler(CWindow *_window);
	virtual ~CRegZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuRegister();

  public:
	static void		 TrapRegister();
	static bool		 CheckRegister();

  private:
	CWindow			*window;
};

#endif
