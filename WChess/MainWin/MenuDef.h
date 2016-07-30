#ifndef __MenuDef_h__
#define __MenuDef_h__

//菜单启用位图
//第一列
#define EN_MENU_OPEN              0x1
#define EN_MENU_RUN               0x2
#define EN_MENU_AGAIN             0x4
#define EN_MENU_ARRANGE           0x8
#define EN_MENU_UNDO              0x10
#define EN_MENU_SAVE              0x20
#define EN_MENU_MANUAL            0x40
#define EN_MENU_BATCH             0x80
#define EN_MENU_EXIT			  0x100
//第二列
#define EN_MENU_COPY              0x1
#define EN_MENU_PASTE             0x2
#define EN_MENU_KEY1              0x4
#define EN_MENU_KEY2              0x8
#define EN_MENU_KEY3              0x10
#define EN_MENU_KEY4              0x20
#define EN_MENU_CTRLKEY1          0x40
#define EN_MENU_CTRLKEY2          0x80
#define EN_MENU_CTRLKEY3          0x100
#define EN_MENU_CTRLKEY4          0x200
//第三列
#define EN_MENU_PATTERN           0x1
#define EN_MENU_TRACE             0x2
#define EN_MENU_LIMITARYTRACE     0x4
#define EN_MENU_NEXT              0x8
#define EN_MENU_LAST              0x10
#define EN_MENU_SOURCE            0x20
#define EN_MENU_LIMITARYSOURCE    0x40
//第四列
#define EN_MENU_EXPLAINATION      0x1
#define EN_MENU_REGISTER          0x2

//初始化态
#define EN_MENU_INIT			 (EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT)
//快捷盘面存取
#define EN_MENU_SHORTKEY		 (EN_MENU_KEY1 | EN_MENU_KEY2 | EN_MENU_KEY3 | EN_MENU_KEY4 \
									| EN_MENU_CTRLKEY1 | EN_MENU_CTRLKEY2 | EN_MENU_CTRLKEY3 | EN_MENU_CTRLKEY4)
//创作棋局
#define EN_MENU_BUILD			 (EN_MENU_PATTERN | EN_MENU_TRACE | EN_MENU_LIMITARYTRACE | EN_MENU_SOURCE | EN_MENU_LIMITARYSOURCE)
//Menu指示
struct TMenuInd {
	int				 nIncrBitmap[4];
	int				 nMenuBitmap[4];
};


//标签显示位图
#define SH_THINKDEPTH			 0x1
#define SH_ASTODESIZE			 0x2
#define SH_SPENDTIME			 0x4
#define SH_LOSEDEGREE			 0x8
#define SH_ROUND				 0x10

//标签指示
struct TLabelInd {
	int				 nLabelBitmap;
	int				 nThinkDepth;
	int				 nAstodeSize;
	int				 nSpendTime;
	int				 nLoseDegree;
	int				 nRound;
	COLORREF		 rgbColor;
};

struct TViewInd {
	bool			 bShowBox;
	bool			 bShowManual;
	bool			 bShowMoveTag;
	bool			 bUpdateMoveTag;
	int				 nMoveTagA;
	int				 nMoveTagB;
	bool			 bSyncBoard;
	TLabelInd		 tLabelInd;
};

extern TMenuInd		 g_tMenuInd;
extern TViewInd		 g_tViewInd;

#endif
