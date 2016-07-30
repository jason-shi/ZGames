#ifndef __ArrangeAction_h__
#define __ArrangeAction_h__

#include "Action.h"

class WChessPiece;
class CArrangeAction : public CAction {
  public:
	CArrangeAction();
	virtual ~CArrangeAction();

  protected:
	void			 ArrangePiece(int nSrcGrid, int nDestGrid, int nKind);

  public:
	void			 ChoicePiece(WChessPiece *pPiece);
	void			 ChoiceBoard(int nGrid);
	void			 ClearPiece(WChessPiece *pPiece);
	void			 ToggleFixed(WChessPiece *pPiece);
};

#define g_pArrangeAction			CArrangeAction::GetInstancePtr()

#endif
