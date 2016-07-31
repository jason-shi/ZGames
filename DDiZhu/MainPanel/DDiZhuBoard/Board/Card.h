#ifndef __Card_h__
#define __Card_h__

#define CardWidth				20
#define CardHeight				32
#define CardLift				5

class CCard;
typedef boost::function<void(CCard *, bool bSelect)>		 FN_CardSelect;

class CCard : public CButton {
  public:
	CCard(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	~CCard();

  public:
	void			 InitWith(byte pips, byte suit);
	bool			 operator==(const CCard& tCard) const;
	byte			 GetValue() const;
	byte			 GetSortIndex() const;

  public:
	void			 SetCardSelect(FN_CardSelect CardSelect);
	void			 OnSelect();
	void			 UnSelect();
	bool			 IsSelected() const;

  public:
	void			 OnPut(byte byRound, byte byIndex);
	bool			 UnPut(byte byRound, byte byIndex);
	byte			 GetRound() const;

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnRClick(UINT nFlags, CPoint point);

  public:
	byte			 pips;
	byte			 suit;

  protected:
	bool			 m_bSelected;
	byte			 m_byRound;
	byte			 m_byIndex;

  protected:
	FN_CardSelect	 OnCardSelect;
};

inline byte CCard::GetValue() const
{
	return pips;
}

inline bool CCard::IsSelected() const
{
	return m_bSelected;
}

inline byte CCard::GetRound() const
{
	return m_byRound;
}

inline void CCard::SetCardSelect(FN_CardSelect CardSelect)
{
	OnCardSelect = CardSelect;
}

#endif
