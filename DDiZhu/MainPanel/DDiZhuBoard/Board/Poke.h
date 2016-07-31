#ifndef __Poke_h__
#define __Poke_h__

#include "Card.h"

class CPoke {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CPoke();
	~CPoke();

  public:
	bool			 Initial();
	void			 ShuffleCards();
	CCard			*GetCard();
	int				 GetSize() const;
	bool			 IsEmpty() const;

  public:
	virtual bool	 SaveCards(TCHAR wcsFileName[_MAX_PATH]);
	virtual bool	 LoadCards(TCHAR wcsFileName[_MAX_PATH]);

  public:
	CZPool			*m_pCardPool;
	VECCARD			 m_vecCard;
};

inline int CPoke::GetSize() const
{
	return m_vecCard.size();
}

inline bool CPoke::IsEmpty() const
{
	return m_vecCard.size() == 0;
}

#endif
