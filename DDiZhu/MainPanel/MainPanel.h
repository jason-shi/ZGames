#ifndef __MainPanel_h__
#define __MainPanel_h__

class CDDiZhuBoard;
class CAutoTest;
class CCompTest;
class CReplayTest;
class IRCrack;
class CMainPanel : public CPanel {
  public:
	CMainPanel(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth);
	~CMainPanel();

  public:
	bool			 Initial();
	void			 OnStartGame();
	void			 OnKeySave();
	void			 OnRecoverPoke();
	void			 OnKeyUp();
	void			 OnKeyDown();
	void			 OnKeyLeft();
	void			 OnKeyRight();

  protected:
	CDDiZhuBoard		*m_pDDiZhuBoard;
	CAutoTest		*m_pAutoTest;
	CCompTest		*m_pCompTest;
	CReplayTest		*m_pReplayTest;
};

#endif
