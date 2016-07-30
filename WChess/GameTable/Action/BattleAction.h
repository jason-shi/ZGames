#ifndef __BattleAction_h__
#define __BattleAction_h__

#include "Action.h"

class WChessPiece;
class CBattleAction : public CAction {
  public:
	CBattleAction();
	virtual ~CBattleAction();

  public:
	void			 BlackMove(int nSrcGrid, int nDestGrid, int nKind, int nBian = 0);
	void			 ChoicePiece(WChessPiece *pPiece);
	void			 ChoiceBoard(int nGrid);
};

#define g_pBattleAction			CBattleAction::GetInstancePtr()

#endif
