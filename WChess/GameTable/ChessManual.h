#ifndef __ChessManual_h__
#define __ChessManual_h__

#define MaxManualLine		 15

class WChessManual : public CPanel {
  public:
	SINGLETON(WChessManual);
	~WChessManual();

  private:
	WChessManual();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	void			 Update();

  private:
	CTextBlk		*m_pLabel[MaxManualLine * 2];
};

#define g_pChessManual		WChessManual::GetInstancePtr()

#endif
