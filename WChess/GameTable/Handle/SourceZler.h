#ifndef __SourceZler_h__
#define __SourceZler_h__

#include "Handle\TraceZler.h"

struct TSeed {
	int				 board[65];
	int				 nTraceTimes;
};

class CSourceZler : public CTraceZler {
  public:
	CSourceZler(CWindow *_window);
	virtual ~CSourceZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 OnMenuSource(bool bLimitary = false);
	void			 OnMenuExit();

  public:
	void			 GetSourceRound();
	void			 GottenTraceRound(int nTraceRound);
	void			 ProcessSource(bool bRootSeed);
	void			 OnCrackEnd(WPARAM wParam, LPARAM lParam);
	void			 OnCrackTout();
	void			 StoreSeeds();
	bool			 IsSeedRepeat(TSeed &tSeed);
	void			 OutputResult();
	void			 OnTraceEnd();
	void			 KillAllTimer();
	void			 InitSourceFile(int nRound);
	void			 SaveToFile();

  private:
	int				 m_nSourceStage; //大倒推阶段 0:计算基准回合数 1:大倒推
	int				 m_nSourceRound;
	std::vector<TSeed>	 m_vecSeed;
	TSeed			 m_tBestSeed;
	int				 m_nFileIndex;
	TCHAR			 m_wcsTimeFile[_MAX_PATH]; //\Source\Date_Time.txt
};

#endif
