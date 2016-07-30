#ifndef __Access_h__
#define __Access_h__

bool				 LoadByFen(TCHAR *fen);
void				 SaveToFen(TCHAR *fen);
void				 SetVirginFen(TCHAR *fen);

bool				 LoadByPgn(const TCHAR* wcsFileName);
bool				 SaveToPgn(const TCHAR* wcsFileName);

#endif
