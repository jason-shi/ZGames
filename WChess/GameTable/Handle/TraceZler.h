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
	int				 m_nTraceStage; //С���ƽ׶� 0:�����׼�غ��� 1:��С���� 2:��С����
	int				 m_nTraceTimes;
	int				 m_nTraceRound;
	int				 m_nIdxBlk;
	int				 m_nIdxRed;
	int				 m_nIdxVisist; //�������
	bool			 m_bToutFlag;
};

#endif
