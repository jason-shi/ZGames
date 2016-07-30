#ifndef __ChessBoard_h__
#define __ChessBoard_h__

#ifndef INVALID_GRID
#define INVALID_GRID			-1
#endif

class WChessPiece;
class WChessBoard : public CPanel {
  public:
	SINGLETON(WChessBoard);
	~WChessBoard();

  private:
	WChessBoard();

  public:
	bool			 Initial();
	bool			 Release();

  private:
	void			 _AddPiece(int nGrid, int nKind);
	void			 _DelPiece(int nGrid);
	void			 _ChgPiece(int nGrid, int nKind);

  public:
	void			 AddPiece(int nGrid, int nKind);
	void			 DelPiece(int nGrid);
	void			 FastSyncBoard();

  public:
	bool			 IsFixed(int nGrid);
	void			 SetFixed(int nGrid);
	void			 ClearFixed(int nGrid);
	void			 ToggleFixed(int nGrid);
	void			 ClearFixed();

  public:
	void			 ShowMoveTag(int nSrcGrid, int nDestGrid);
	void			 HideMoveTag();

  public:
	void			 MovePiece(int nSrcGrid, int nDestGrid, int nBian);

  protected:
	int				 GetGrid(CPoint point) const;

  protected:
	void			 OnLClick(UINT nFlags, CPoint point);
	void			 OnLClick(CObject *pPiece);
	void			 OnRClick(CObject *pPiece);
	void			 OnMClick(CObject *pPiece);

  protected:
	WChessPiece		*m_pPiece[65];
	bool			 m_bFixed[65];
	CSprite			*m_pFixed[65]; //“¿¿µ”⁄m_bFixed

  protected:
	CZPool			*m_pPiecePool;
	CZPool			*m_pFixedPool;

  protected:
	CSprite			*m_pMoveTagA;
	CSprite			*m_pMoveTagB;
};

#define g_pChessBoard		WChessBoard::GetInstancePtr()

#endif
