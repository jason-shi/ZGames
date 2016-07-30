#ifndef __CTable_h__
#define __CTable_h__

extern int			 TblPieceBitmap[64]; //棋子合法位图
extern byte			 TblCharToPiece[256]; //字符转换为棋子类型
extern char			 TblPieceToChar[15]; //棋子类型转换为字符
extern int			 TblExclusion[32]; //对应位的排除码

void				 CreateAllTable();

#endif
