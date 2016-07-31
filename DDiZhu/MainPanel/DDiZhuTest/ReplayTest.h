#ifndef __ReplayTest_h__
#define __ReplayTest_h__

class CDDiZhuBoard;
class CDDiZhuReplay;
class CReplayTest {
  public:
	CReplayTest(CDDiZhuBoard *pDDiZhuBoard);
	virtual ~CReplayTest();

  public:
	bool			 Initial();
	CDDiZhuReplay	*CreateReplay();

  public:
	bool			 OpenReplay(TCHAR wcsFileName[_MAX_PATH]);
	void			 CloseReplay();
	void			 ReadReplay(byte byType);

  public:
  	void			 DoDiZhu();
  	void			 DoPutCard();
	void			 DoGvp();
	void			 DoUndo();

  public:
	void			 OnTimer(int nTimerId);

  protected:
	CDDiZhuBoard		*m_pDDiZhuBoard;
	CDDiZhuReplay	*m_pDDiZhuReplay;
};

#endif
