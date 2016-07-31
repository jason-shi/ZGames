#ifndef __CompTest_h__
#define __CompTest_h__

#define MAX_PROGRAM				2
#define MAX_CHAIR				2

class CDDiZhuBoard;
class CCompTest {
  public:
	CCompTest(CDDiZhuBoard *pDDiZhuBoard);
	virtual ~CCompTest();

  public:
	bool			 Initial();

  public:
	void			 OnTestBegin();
	bool			 OnTestNext();
	void			 OnTestEnd();

  public:
  	void			 DoDiZhu();
  	void			 DoPutCard();

  public:
	void			 OnTimer(int nTimerId);

  protected:
	CDDiZhuBoard		*m_pDDiZhuBoard;

  protected:
    int				 m_nProgram;
    byte			 m_byChair;
    int				 m_nCaseNo;
    int				 m_nResult[MAX_PROGRAM][MAX_CHAIR];
};

#endif
