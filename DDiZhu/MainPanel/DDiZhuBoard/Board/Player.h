#ifndef __Player_h__
#define __Player_h__

#include "Card.h"
#include "HandBmp.h"

class CPlayer : public CPanel {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CPlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair);
	virtual ~CPlayer();

  public:
	bool			 Initial();

  public:
	void			 OnStartGame();
	void			 OnSendCard(CCard *pCard);
	void			 OnSendCardsOver();

  public:
	void			 ClearHandBmp();
	void			 ClearCards();
	void			 SortCards();
	void			 ShowCards();

  public:
	bool			 HasCards(byte byGrade, byte byCount);
	void			 UpdateStatus(byte byType, byte byGrade, byte byCount, word wAttach);
	void			 GetResponse(byte &byRspType, byte &byGrade, byte &byCount, word &wAttach);

  public: //玩家操作
	bool			 OnPut(byte byRound, byte byIndex);
	void			 UnPut(byte byRound, byte byIndex);
	bool			 IsAlive() const;
	void			 UpdateAlive();

  public: //托管相关
	void			 GetHandBmp(word bitmap[4]);
	void			 OnPut(byte byRound, byte byIndex, VECBYTE &vecCardValue);

  protected:
	VECCARD			 m_vecCard;

  protected:
	byte			 m_byChair;
	byte			 m_byType;
	byte			 m_byRspType;
	byte			 m_byGrade;
	byte			 m_byCount;
	word			 m_wAttach;
	bool			 m_bAlive;

  protected:
	CHandBmp		 m_tHandBmp;
};

inline void CPlayer::ClearHandBmp()
{
	m_tHandBmp.Clear();
}

inline bool CPlayer::IsAlive() const
{
	return m_bAlive;
}

#endif
