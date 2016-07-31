#ifndef __DDiZhuBoard_h__
#define __DDiZhuBoard_h__

#include "Board\Card.h"
#include "Board\Board.h"

class CDDiZhuPlayer;
class CDiZhuPanel;
class CDDiZhuOperPanel;
class CDDiZhuBoard : public CBoard {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CDDiZhuBoard(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CDDiZhuBoard();

  protected:
	CPlayer			*CreatePlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair);
	CRecord			*CreateRecord();
	COperPanel		*CreateOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);

  public:
	CDDiZhuPlayer		*GetPlayer(byte byChair) const;

  public:
	bool			 Initial();
	bool			 OnStartGame();
	void			 SendCards();
	void			 SendDiCards();
	void			 OnSendCardsOver();
	byte			 GetStage() const;

  protected:
	void			 CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

  public:
	void			 OnDiZhu(CObject *pObject, UINT nFlags, CPoint point);
  public:
	void			 OnDiZhu(int nObjId);
	void			 OnPut(TPutReplay &tPutReplay);

  public:
	void			 OnTimer(WPARAM wParam);
	void			 SaveRecord(byte byType);

  protected:
	CDiZhuPanel		*m_pDiZhuPanel;

  protected:
	VECCARD			 m_vecDiCard;
};

inline CDDiZhuPlayer *CDDiZhuBoard::GetPlayer(byte byChair) const
{
	return (CDDiZhuPlayer *)m_pPlayer[byChair];
}

inline byte CDDiZhuBoard::GetStage() const
{
	return m_byStage;
}

#endif
