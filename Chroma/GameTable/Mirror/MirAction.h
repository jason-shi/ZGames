#ifndef __Action_h__
#define __Action_h__

class CMirror;
class CMirPanel;
class CMirAction {
  public:
	CMirAction();
	virtual ~CMirAction();

  public:
	void			 RotateMirror(CMirror *pMirror, bool bClosewise);
	void			 PickMirror(CMirror *pMirror, CPoint point);
	void			 DragMirror(CMirror *pMirror, CPoint point);
	void			 DropMirror(CMirror *pMirror, CPoint point);
	void			 ChoiceMirror(CMirPanel *pMirPanel, CPoint point, CMirror *pAnchorMirror);

  public:
	bool			 IsDragMirror() const;

  protected:
	CMirror			*m_pMoveMirror;
	CMirPanel		*m_pMirPanel;
	int				 m_nPickGrid;
	byte			 m_byTypeDir;

  protected:
	bool			 m_bDragMirror;
};

inline bool CMirAction::IsDragMirror() const
{
	return m_bDragMirror;
}

#endif
