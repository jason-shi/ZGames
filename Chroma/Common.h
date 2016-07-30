#ifndef __CCommon_h__
#define __CCommon_h__

typedef unsigned char			byte;

#define TM_CONNECT				1001 //连接服务器定时器

//注：以下镜子类型的定义和镜子图片耦合
#define MT_NOTHING				0 //注：强制为0
#define MT_WALL					1
#define MT_LASER_R				2
#define MT_LASER_G				3
#define MT_LASER_B				4
#define MT_LASER_W				5
#define MT_REFLECTOR			6
#define MT_BENDER				7
#define MT_DOPPLER				8
#define MT_FILTER_R				9
#define MT_FILTER_G				10
#define MT_FILTER_B				11
#define MT_QUAD_REFL			12
#define MT_QUAD_BEND			13
#define MT_CONDUIT				14
#define MT_MUL_FILTER			15
#define MT_PRISM				16
#define MT_TANGLER				17
#define MT_SPLITTER				18
#define MT_LOGIC_GATE           19
#define MT_COMPLEMENTOR         20
#define MT_STARBURST			21
#define MT_TWISTER				22
#define MT_TELEPORTER			23
#define MT_OCCUPIED				24
#define MT_TARGET				25
#define MT_HIT_TARGET			26
#define MT_MIRROR_COUNT			27

//镜子的方向，入射角
#define DIR(n)					((n) & 0x7)
#define ANGLE_IN				DIR(in - dir + 8)

//由镜型+镜向组成镜子，由光向+光色组成光线
#define MAKE_MIRROR(type, dir)	(((type) << 3) + (dir))
#define MAKE_BEAM(io, color)	(((io) << 3) + (color))

//由镜子+光线组成索引
#define MAKE_INDEX(type, dir, io, color)		((MAKE_MIRROR(type, dir) << 6) + MAKE_BEAM(io, color))


#define WM_ZAPP					(WM_USER + 0x00FF)

#define WM_START_SERVICE		(WM_ZAPP + 1)
#define WM_STOP_SERVICE			(WM_ZAPP + 2)
#define WM_SOCKET				(WM_ZAPP + 3)
#define WM_ASYN_SOCKET			(WM_ZAPP + 4)
#define WM_FIRST_ACTION			(WM_ZAPP + 5)
#define WM_UPDATE_LIGHT			(WM_ZAPP + 6)

extern bool			 g_bEditState;

extern CImage		 ImageBack;
extern CImage		 ImageMirror;
extern CImage		 ImageChoice;
extern CImage		 ImageGate;

extern bool			 LoadImage();

#endif
