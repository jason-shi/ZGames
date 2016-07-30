#ifndef __Pattern_h__
#define __Pattern_h__

#define PTN_MINROUND		 5
#define PTN_MAXDEPTH		 80

struct TRemainPiece {
	int				 kind;
	int				 force;
};

void				 InitSwapRemain();

void				 ChangeOffensive(bool bWeaken);		 //改变攻势
void				 RobustSwap();						 //鲁棒交换

#endif
