#ifndef __Action_h__
#define __Action_h__

#define NO_ACTION				0
#define BATTLE_ACTION			1
#define ARRANGE_ACTION			2

class WChessPiece;
class CAction {
  public:
	CAction();
	virtual ~CAction();

  public:
	virtual void	 ChoicePiece(WChessPiece *pPiece);
	virtual void	 ChoiceBoard(int nGrid);
	virtual void	 ClearPiece(WChessPiece *pPiece);
	virtual void	 ToggleFixed(WChessPiece *pPiece);

  protected:
	void			 Select(WChessPiece *pPiece);
	void			 UnSelect();

  protected:
	WChessPiece		*m_pSelected;
};

inline void CAction::ChoicePiece(WChessPiece *pPiece)
{
}

inline void CAction::ChoiceBoard(int nGrid)
{
}

inline void CAction::ClearPiece(WChessPiece *pPiece)
{
}

inline void CAction::ToggleFixed(WChessPiece *pPiece)
{
}

#endif
