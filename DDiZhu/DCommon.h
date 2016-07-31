#ifndef __RCommon_h__
#define __RCommon_h__

#include "Common.h"
#include "Table.h"

#define TM_REPLAY				100
#define TM_AUTO_TEST			101
#define TM_COMP_TEST			102

#define STAGE_START				0
#define STAGE_DIZHU				1
#define STAGE_PUTCARD			2

#define RECORD_DIZHU			1
#define RECORD_PUT				2
#define RECORD_GVP				3
#define RECORD_UNDO				4

#define PlayerHeight			37

#define HEX_2_DEC(hex) \
		((hex) >= '0' && (hex) <= '9' ? (hex) - '0' : \
		(hex) >= 'A' && (hex) <= 'F' ? (hex) - 'A' + 10 : (hex) - 'a' + 10)

typedef std::vector<byte>		VECBYTE;

struct TPutReplay {
	byte			 byChair;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
};

extern CImage		 ImageBack;
extern CImage		 ImagePoke;
extern CImage		 ImageDDiZhu;

extern bool			 LoadImage();

#endif
