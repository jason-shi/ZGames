#ifndef __OperPanel_h__
#define __OperPanel_h__

class CBoard;
class COperPanel : public CPanel {
  public:
	COperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~COperPanel();

  public:
	virtual bool	 Initial(CBoard *pBoard);
	virtual void	 OnStartGame();

  public:
	virtual void	 ShowSpendTime(int nMS);

  public:
	virtual void	 OnSendCardsOver(byte byChair);
	virtual void	 OnPut(bool bGameOver, byte byChair);
	virtual void	 OnGvp(byte byChair);
	virtual void	 OnUndo(byte byChair);
	virtual void	 OnCrackResult(byte byScore, byte byChair);

  protected:
	CButton			*m_pBtnPut;
	CButton			*m_pBtnGvp;
	CButton			*m_pBtnUndo;
	CButton			*m_pBtnTrust;
	CSprite			*m_pSprTurn;
	CSprite			*m_pSprScore;
	CTextBlk		*m_pSpendTime;
};

#endif
