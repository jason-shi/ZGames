#ifndef __GatePanel_h__
#define __GatePanel_h__

class CGate;
class CGatePanel : public CPanel {
  public:
	SINGLETON(CGatePanel);
	virtual ~CGatePanel();

  private:
	CGatePanel();

  public:
	bool			 Initial();
	void			 OnStartGame();
	void			 OnStopGame();
	void			 OnSwitchGate(int nGate);

  public:
	void			 LoadGateStatus();
	void			 SaveGateStatus();
	void			 RevealGate();
	void			 ProcessResult(bool bSuccess);

  public:
	void			 SetHint(const TCHAR* pHint);
	void			 SetSuccess(bool bSuccess);

  protected:
	void			 OnLClick(CObject *pGate);

  protected:
	CGate			*m_pGate[50];
	int				 m_nCurGate;

  protected:
	CTextBlk		*m_pLblHint;
	CTextBlk		*m_pLblSuccess;
};

#define g_pGatePanel			CGatePanel::GetInstancePtr()

#endif
