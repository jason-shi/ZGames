#ifndef	__Sample_h__
#define	__Sample_h__

#include "Application.h"

class CSampleApp : public CWinApp {
  public:
	CSampleApp();
	~CSampleApp();

  public:
	BOOL			 InitInstance();
};

extern CSampleApp	 theApp;

#endif
