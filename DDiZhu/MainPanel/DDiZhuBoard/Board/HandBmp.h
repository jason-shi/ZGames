#ifndef __HandBmp_h__
#define __HandBmp_h__

class CCard;
class CHandBmp {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CHandBmp();
	~CHandBmp();

  public:
	void			 Clear();
	void			 OnInit(VECCARD &vecCard);
	void			 OnPut(VECBYTE &vecCardValue);
	void			 UnPut(VECBYTE &vecCardValue);

  protected:
	void			 DelCard(byte byCardValue);
	void			 AddCard(byte byCardValue);

  public:
	word			 bitmap[4];
};

#endif
