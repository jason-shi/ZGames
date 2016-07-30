#include "StdAfx.h"
#include "Record.h"

using namespace std;

vector<TRecord>		 vecRecord;


void SaveRecord(int src, int dest, int bian, int nRest)
{
	TRecord tRecord;
	memset(&tRecord, 0, sizeof(tRecord));

	tRecord.src = src;
	tRecord.dest = dest;
	tRecord.bian = bian;
	tRecord.death = board[dest];
	tRecord.nRest = nRest;

	vecRecord.push_back(tRecord);
}

bool IsRoundSkip(int nRound)
{
	if (vecRecord.size() == 0)
		return false;
	if (nRound == vecRecord[vecRecord.size() - 2].nRest - 1)
		return false;

	return true;
}
