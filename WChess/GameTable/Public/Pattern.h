#ifndef __Pattern_h__
#define __Pattern_h__

#define PTN_MINROUND		 5
#define PTN_MAXDEPTH		 80

struct TRemainPiece {
	int				 kind;
	int				 force;
};

void				 InitSwapRemain();

void				 ChangeOffensive(bool bWeaken);		 //�ı乥��
void				 RobustSwap();						 //³������

#endif
