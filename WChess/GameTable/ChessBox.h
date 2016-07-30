#ifndef __ChessBox_h__
#define __ChessBox_h__

#ifndef INVALID_GRID
#define INVALID_GRID			-1
#endif

class WChessPiece;
class WChessBox : public CPanel {
  public:
	SINGLETON(WChessBox);
	~WChessBox();

  private:
	WChessBox();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	void			 AddPiece(int nGrid, int nKind);
	void			 DelPiece(int nGrid);

  public:
	void			 SetAvailPiece();

  protected:
	void			 OnLClick(CObject *pPiece);

  protected:
	int				 m_nRemain[13];
	WChessPiece		*m_pPiece[13]; //“¿¿µ”⁄m_nRemain
};

#define g_pChessBox			WChessBox::GetInstancePtr()

#endif
