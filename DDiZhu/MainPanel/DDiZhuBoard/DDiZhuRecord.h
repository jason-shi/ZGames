#ifndef __DDiZhuRecord_h__
#define __DDiZhuRecord_h__

#include "Board\Record.h"

class CCard;
class CDDiZhuRecord : public CRecord {
	typedef std::vector<CCard *>	VECCARD;
  public:
	CDDiZhuRecord(TCHAR wcsFileName[_MAX_PATH]);
	~CDDiZhuRecord();

  public:
	void			 WriteDiZhu(byte byChair);
};

#endif
