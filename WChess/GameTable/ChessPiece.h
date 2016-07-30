#ifndef __ChessPiece_h__
#define __ChessPiece_h__

class WChessPiece : public CButton {
  public:
	WChessPiece(CPoint xDrawPos, int nGrid, int nKind);
	virtual ~WChessPiece();

  public:
	void			 SetGrid(int nGrid);
	int				 GetGrid() const;
	void			 SetKind(int nKind);
	int				 GetKind() const;

  protected:
	bool			 IsExist() const;

  public:
	void			 OnSelect();
	void			 OnUnSelect();

  protected:
	int				 m_nGrid;
	int				 m_nKind;

  protected:
	static CFlickAnime		 AnimeFlick;
};

inline void WChessPiece::SetGrid(int nGrid)
{
	m_nGrid = nGrid;
}

inline int WChessPiece::GetGrid() const
{
	return m_nGrid;
}

inline void WChessPiece::SetKind(int nKind)
{
	m_nKind = nKind;
}

inline int WChessPiece::GetKind() const
{
	return m_nKind;
}

inline bool WChessPiece::IsExist() const
{
	//显示或者在播放闪烁动画
	return m_bShow || AnimeFlick.GetAttach() == this;
}

#endif
