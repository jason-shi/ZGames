#ifndef __MirPanel_h__
#define __MirPanel_h__

#define INVALID_GRID				-1

class CMirPanel : public CPanel {
  public:
	CMirPanel();
	virtual ~CMirPanel();

  public:
	virtual int		 GetGrid(CPoint point) const = 0;
	virtual bool	 CanRotate(int nGrid) const = 0;
	virtual bool	 IsLightRelevant() const = 0;

  public:
	virtual void	 AddMirror(int nGrid, byte type_dir) = 0;
	virtual void	 DelMirror(int nGrid) = 0;
	virtual void	 RotateMirror(int nGrid, bool bClockwise) = 0;

  public:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnLClick(CObject *pMirror, UINT nFlags, CPoint point);
	void			 OnLButtonUp(CObject *pMirror, UINT nFlags, CPoint point);
	void			 OnRButtonUp(CObject *pMirror, UINT nFlags, CPoint point);
	void			 OnMousePick(CObject *pMirror, UINT nFlags, CPoint point);
	void			 OnMouseDrag(CObject *pMirror, UINT nFlags, CPoint point);
	void			 OnMouseDrop(CObject *pMirror, UINT nFlags, CPoint point);
};

#endif
