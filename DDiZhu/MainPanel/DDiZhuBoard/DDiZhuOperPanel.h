#ifndef __DDiZhuOperPanel_h__
#define __DDiZhuOperPanel_h__

#include "Board\OperPanel.h"

class CDDiZhuBoard;
class CDDiZhuOperPanel : public COperPanel {
  public:
	CDDiZhuOperPanel(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CDDiZhuOperPanel();

  public:
	bool			 Initial(CBoard *pBoard);

  public:
	void			 OnSendCardsOver(byte byChair);
	void			 OnDiZhu(byte byChair);
};

#endif
