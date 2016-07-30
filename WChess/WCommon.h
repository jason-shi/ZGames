#ifndef __CCommon_h__
#define __CCommon_h__

#define PSEUDO					true
#define AGAIN					true

#define TM_UPDATE_VIEW			1001 //破解更新定时器
#define TM_BATCH_GUARD			1002 //破解保护定时器
#define TM_PATTERN_GUARD		1003 //定形局保护定时器
#define TM_TRACE_GUARD			1004 //小倒推保护定时器
#define TM_SOURCE_GUARD			1005 //大倒推保护定时器

#define WM_ZAPP					(WM_USER + 1024)
#define WM_FIRST_ACTION			(WM_ZAPP + 1)
#define WM_CRACK_END			(WM_ZAPP + 2)
#define WM_PSEUDO_CRACK			(WM_ZAPP + 3)
#define WM_MOVE_PIECE			(WM_ZAPP + 4)
#define WM_VIEW_IND				(WM_ZAPP + 5)
#define WM_MENU_IND				(WM_ZAPP + 6)

extern TCHAR		 ModulePath[_MAX_PATH];
extern CImage		 ImagePiece;

bool				 LoadImage();

#endif
