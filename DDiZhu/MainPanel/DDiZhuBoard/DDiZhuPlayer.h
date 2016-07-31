#ifndef __DDiZhuPlayer_h__
#define __DDiZhuPlayer_h__

#include "Board\Player.h"

class CCard;
class CDDiZhuPlayer : public CPlayer {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CDDiZhuPlayer(CImage *pImage, CPoint xDrawPos, CSize xSize, byte byChair);
	virtual ~CDDiZhuPlayer();

  public:
	bool			 Initial();

  public:
	void			 OnDiZhu(VECCARD& vecDiCard);
};

#endif
