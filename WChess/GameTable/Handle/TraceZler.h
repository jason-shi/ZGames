#ifndef __TraceZler_h__
#define __TraceZler_h__

#include "ZlerBase.h"

class CTraceZler : public CZlerBase {
  public:
	CTraceZler();
	CTraceZler(CWindow *_window);
	virtual ~CTraceZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuTrace(bool bLimitary = false);
	void			 OnMenuNext();
	void			 OnMenuLast();
	void			 OnMenuExit();

  public:
	void			 GetTraceRound();
	void			 ProcessTrace(int nTraceRound);
	void			 ProcessTrList1();
	void			 ProcessTrList2();
	void			 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	void			 OnCrackTout();
	void			 ReorderResult();
	virtual void	 OutputResult();
	virtual void	 OnTraceEnd();
	void			 KillAllTimer();
	void			 SaveToFile();

	void			 SendViewInd_InitState();
	void			 SendViewInd_CrackState();
	void			 SendViewInd_BrowseState();
	void			 SendMenuInd_InitState();
	void			 SendMenuInd_CrackState();
	void			 SendMenuInd_BrowseState();

  protected:
	CWindow			*window;
	int				 m_nTraceStage; //小倒推阶段 0:计算基准回合数 1:黑小倒推 2:红小倒推
	int				 m_nTraceTimes;
	int				 m_nTraceRound;
	int				 m_nIdxBlk;
	int				 m_nIdxRed;
	int				 m_nIdxVisist; //浏览索引
	bool			 m_bToutFlag;
};

#endif
