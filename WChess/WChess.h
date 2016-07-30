#ifndef __WChess_h__
#define __WChess_h__

#define Board(x, y)				board[(x) + ((y) << 3)]

struct TBlkMove {
	int				 nSrcGrid;
	int				 nDestGrid;
	int				 nBian;
};

extern int			 board[65]; //����һ����Ч��
extern TBlkMove		 g_tBlkMove;

bool				 IsBoardLegal();
bool				 IsBlkLegal();
bool				 IsBothLegal();

void				 InitBoard();

bool				 InCheckRed();
bool				 InCheckBlk();

#endif
