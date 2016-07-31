#ifndef __AutoTest_h__
#define __AutoTest_h__

class CDDiZhuBoard;
class CAutoTest {
  public:
	CAutoTest(CDDiZhuBoard *pDDiZhuBoard);
	virtual ~CAutoTest();

  public:
	bool			 Initial();

  public:
	void			 OnTestBegin();
	void			 OnTestEnd();

  public:
  	void			 DoDiZhu();
  	void			 DoPutCard();

  public:
	void			 OnTimer(int nTimerId);

  protected:
	CDDiZhuBoard		*m_pDDiZhuBoard;
};

#endif
