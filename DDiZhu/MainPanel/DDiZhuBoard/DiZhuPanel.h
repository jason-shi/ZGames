#ifndef __DiZhuPanel_h__
#define __DiZhuPanel_h__

class CDDiZhuBoard;
class CCard;
class CDiZhuPanel : public CPanel {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CDiZhuPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CDiZhuPanel();

  public:
	bool			 Initial(CDDiZhuBoard *pDDiZhuBoard);
	void			 OnStartGame();

  public:
	void			 OnSendCard(int nIndex, CCard *pCard);
	VECCARD&		 GetDiCards() { return m_vecDiCard; }

  public:
	void			 OnDiZhu(byte byChair);

  protected:
	CButton			*m_pBtnDiZhu[3];
	VECCARD			 m_vecDiCard;
};

inline void CDiZhuPanel::OnDiZhu(byte byChair)
{
	Show(false);
}

#endif
