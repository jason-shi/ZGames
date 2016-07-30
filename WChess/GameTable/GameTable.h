#ifndef __GameTable_h__
#define __GameTable_h__

#include "MainWin\MainWin.h"

class CAction;
class CZlerBase;
class CAccessZler;
class CCrackZler;
class CArrangeZler;
class CBatchZler;
class CPtnZler;
class CManualZler;
class CTraceZler;
class CSourceZler;
class CRegZler;
class CGameTable : public CMainWin {
  public:
	SINGLETON(CGameTable);
	~CGameTable();

  private:
	CGameTable();

  public:
	bool			 Initial();
	bool			 Release();
	LRESULT			 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void			 OnCommand(UINT notifyCode, UINT id, HWND ctrl);
	bool			 OnFirstAction();
	void			 OnClose();
	void			 OnTimer(WPARAM wParam);

  public:
	void			 RecvViewInd(WPARAM wParam, LPARAM lParam);
	void			 ShowChessBox(bool bShow = true);
	void			 ShowChessManual(bool bShow = true);

  public:
	void			 SetZlerBase(CZlerBase *pZler);
	CZlerBase		*GetZlerBase() const;
	void			 SetAction(int nAction);
	CAction			*GetAction() const;

  public:
	void			 UpdateThinkDepth(int nDepth);
	void			 UpdateAstodeSize(int nSize);
	void			 UpdateSpendTime(int nTime);
	void			 UpdateLoseDegree(int nDegree);
	void			 UpdateRound(int nRound, COLORREF rgbColor);
	void			 ShowLabel(int nViewBitmap);

  private:
	CTextBlk		*m_pLblThinkDepth;		 //思考深度
	CTextBlk		*m_pLblAstodeSize;		 //数据库使用量
	CTextBlk		*m_pLblSpendTime;		 //用时
	CTextBlk		*m_pLblLoseDegree;		 //红必负阶值
	CTextBlk		*m_pLblRound;			 //回合数

  protected:
	CZlerBase		*m_pZlerBase;
	CAction			*m_pAction;
};

inline void CGameTable::SetZlerBase(CZlerBase *pZler)
{
	m_pZlerBase = pZler;
}

inline CZlerBase *CGameTable::GetZlerBase() const
{
	return m_pZlerBase;
}

inline CAction *CGameTable::GetAction() const
{
	return m_pAction;
}

#define g_pGameTable		CGameTable::GetInstancePtr()

#endif
