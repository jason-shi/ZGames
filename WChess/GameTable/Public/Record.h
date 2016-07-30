#ifndef __Record_h__
#define __Record_h__

struct TRecord {
	int				 src;
	int				 dest;
	int				 bian;
	int				 death;
	int				 nRest;
};

extern std::vector<TRecord>		 vecRecord;

void				 SaveRecord(int src, int dest, int bian, int nRest);
bool				 IsRoundSkip(int nRound);

#endif
