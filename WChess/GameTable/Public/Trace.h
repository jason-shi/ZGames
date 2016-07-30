#ifndef __TraceList_h__
#define __TraceList_h__

#define MAX_TRACE	 900

typedef void (*_TRACEFUNC)(int x, int y);

struct TTrace {
	int				 src;
	int				 dest;
	int				 actor;
	int				 death;
	int				 index;
	int				 flag;
};

extern int			 trcboard[65];

extern int			 g_nPRest[13]; //Æå×ÓÊ£Óà¸öÊý

extern TTrace		 TrList1[MAX_TRACE];
extern TTrace		 TrList2[MAX_TRACE * 4];

extern int			 TrCount1;
extern int			 TrCount2;

extern bool			 g_bLimitary;

void				 InitTraceList();

void				 AddTraceBlk(int kind, int x1, int y1, int x2, int y2, bool bBian = false, bool bPawnEat = false);
void				 AddTraceRed(int kind, int x1, int y1, int x2, int y2, bool bBian = false, bool bPawnEat = false);

void				 GenTrList1();
void				 GenTrList2();

bool				 FetchTrList1(int &nIndex);
bool				 FetchTrList2(int &nIndex);

void				 UpdateBoard(TTrace *pTrace);
void				 RestoreBoard(TTrace *pTrace);

#endif
