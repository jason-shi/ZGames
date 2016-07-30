#ifndef __CrackZler_h__
#define __CrackZler_h__

#include "ZlerBase.h"

class CCrackZler : public CZlerBase {
  public:
	CCrackZler(CWindow *_window);
	virtual ~CCrackZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuRun();
	void			 OnMenuAgain();
	void			 OnMenuUndo();
	void			 OnMenuExit();

  public:
	void			 PseudoCrack();
	void			 ProcessCrack(bool bPesudo, bool bAgain);
	void			 ProcessUndo(bool bRedUndo);
	void			 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	void			 KillAllTimer();
	void			 SendViewInd_InitState();
	void			 SendViewInd_CrackState(bool bPseudo);
	void			 SendViewInd_BlackLose(int nSpendTime, int nRound);
	void			 SendViewInd_RedLose(int nSpendTime);
	void			 SendViewInd_AgainState();
	void			 SendViewInd_UndoState();
	void			 SendMenuInd_InitState();
	void			 SendMenuInd_CrackState();
	void			 SendMenuInd_UndoState();

  private:
	CWindow			*window;
};

#endif
