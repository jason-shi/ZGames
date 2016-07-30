#ifndef __CTable_h__
#define __CTable_h__

extern byte			 TblMirror[256 * 64];
extern byte			 TblNextGrid[256 * 8];

extern bool			 TblSpecMirror[MT_MIRROR_COUNT];
extern bool			 TblQuantumEnd[MT_MIRROR_COUNT];

extern byte			 TblSplitter[4 * 64];
extern byte			 TblTangler[8 * 64];
extern byte			 TblPrismG[8 * 64];
extern byte			 TblPrismB[8 * 64];
extern byte			 TblStarburst1[64];
extern byte			 TblStarburst2[64];
extern byte			 TblStarburst3[64];
extern int			 TblLightMask[64];

extern char			 TblMirToAlpha[256];
extern byte			 TblAlphaToMir[256];

extern void			 CreateTables();

#endif
