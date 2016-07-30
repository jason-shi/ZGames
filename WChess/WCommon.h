#ifndef __CCommon_h__
#define __CCommon_h__

#define PSEUDO					true
#define AGAIN					true

#define TM_UPDATE_VIEW			1001 //�ƽ���¶�ʱ��
#define TM_BATCH_GUARD			1002 //�ƽⱣ����ʱ��
#define TM_PATTERN_GUARD		1003 //���ξֱ�����ʱ��
#define TM_TRACE_GUARD			1004 //С���Ʊ�����ʱ��
#define TM_SOURCE_GUARD			1005 //���Ʊ�����ʱ��

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
