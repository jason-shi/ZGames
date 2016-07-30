#ifndef __GameTable_h__
#define __GameTable_h__

#include "MainWin\MainWin.h"

class CMirChoice;
class CGatePanel;
class CMirAction;
class CGameTable : public CMainWin {
  public:
	SINGLETON(CGameTable);
	~CGameTable();
  private:
	CGameTable();

  public:
	bool			 Initial();
	void			 Release();
	LRESULT			 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void			 OnCommand(UINT notifyCode, UINT id, HWND ctrl);
	bool			 OnFirstAction();
	void			 OnClose();
	void			 OnTimer(WPARAM wParam);

  protected:
	void			 OnAppEdit();
	void			 OnAppCopy();
	void			 OnAppPaste();
	void			 OnAppRegister();

  protected:
	void			 TrapRegister();
	bool			 CheckRegister();

  protected:
	void			 OnStartGame();
	void			 OnStopGame();
	void			 OnUpdateLight();

  public:
	CMirAction		*GetMirAction() const;

  protected:
	CMirAction		*m_pMirAction;
};

inline CMirAction *CGameTable::GetMirAction() const
{
	return m_pMirAction;
}

#define g_pGameTable		CGameTable::GetInstancePtr()

#endif
