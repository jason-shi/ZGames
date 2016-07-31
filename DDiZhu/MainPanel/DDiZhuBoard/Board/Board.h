#ifndef __Board_h__
#define __Board_h__

struct TAction {
	byte			 byChair;
	byte			 byChair1;
	byte			 byType;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	word			 wAttach;
	byte			 byRound;
	byte			 byIndex;
	VECBYTE			 vecCardValue;
};

class CPoke;
class CPlayer;
class CRecord;
class COperPanel;
class CBoard : public CPanel {
  public:
	CBoard(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBoard();

  protected:
	virtual CPoke		*CreatePoke();
	virtual CPlayer		*CreatePlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair);
	virtual CRecord		*CreateRecord();
	virtual COperPanel	*CreateOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);

  public:
	bool			 Initial();
	void			 SetWindow(CWindow *_window);

  public:
	virtual byte	 GetDiZhu() const;
	virtual byte	 GetChair() const;
	virtual byte	 GetChair1() const;
	virtual byte	 GetNextPlayer(byte byChair) const;

  public:
	virtual bool	 OnStartGame();
	virtual bool	 ShufflePoke();
	virtual bool	 RecoverPoke();
	virtual void	 SendCards();
	virtual void	 SendDiCards();
	virtual void	 OnSendCardsOver();
	virtual bool	 CheckGameOver();

  public:
	void			 ClearAction();
	void			 SaveAction();
	void			 BackAction();

  public:
	void			 SaveRecord(byte byType);

  protected:
	virtual void	 CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

  public:
	virtual bool	 OnPut(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 OnGvp(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 OnUndo(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 OnTrust(CObject *pObject, UINT nFlags, CPoint point);

  public:
	virtual void	 ProcessCrackResult(TCrackResult *pCrackResult);

  protected:
	CWindow			*window;
	CPoke			*m_pPoke;
	CPlayer			*m_pPlayer[3];
	CRecord			*m_pRecord;

  protected:
	COperPanel		*m_pOperPanel;

  protected:
	byte			 m_byStage;
	byte			 m_byDiZhu;
	byte			 m_byChair;
	byte			 m_byChair1;
	byte			 m_byAlive;
	byte			 m_byRound;
	byte			 m_byIndex;
	std::vector<TAction>		m_vecAction;

  protected:
	byte			 m_byType;
	byte			 m_byRspType;
	byte			 m_byGrade;
	byte			 m_byCount;
	word			 m_wAttach;
};

inline void CBoard::SetWindow(CWindow *_window)
{
	window = _window;
}

inline byte CBoard::GetDiZhu() const
{
	return m_byDiZhu;
}

inline byte CBoard::GetChair() const
{
	return m_byChair;
}

inline byte CBoard::GetChair1() const
{
	return m_byChair1;
}

inline byte CBoard::GetNextPlayer(byte byChair) const
{
	if (++byChair == 3)
		byChair = 0;
	return byChair;
}

#endif
