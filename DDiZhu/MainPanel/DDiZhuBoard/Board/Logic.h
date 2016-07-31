#ifndef __Logic_h__
#define __Logic_h__

class CLogic {
  public:
	CLogic();
	~CLogic();

  public:
	static void		 TransToBitmap(word bitmap[4], VECBYTE &vecCardValue);
	static void		 TransToValue(VECBYTE &vecCardValue, word bitmap[4]);

  public:
	static byte		 GetType(word bitmap[4]);
	static byte		 GetGrade(byte byRspType, word bitmap[4]);
	static byte		 GetCount(byte byRspType, word bitmap[4]);
	static word		 GetAttach(byte byRspType, word bitmap[4]);
	static bool		 CheckLegal(byte byType, byte byRspType, byte byGrade, byte byCount, word bitmap[4]);

  public:
	static void		 GetValues(VECBYTE &vecCardValue, byte byRspType, byte byGrade, byte byCount, word wAttach);
};

#endif
