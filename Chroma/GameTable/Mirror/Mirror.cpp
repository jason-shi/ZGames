#include "StdAfx.h"
#include "Mirror.h"

const int MirrorAttr[28][2] = { //转向，深度
	1, 3, //MT_NOTHING
	1, 1, //MT_WALL
	8, 1, //MT_LASER_R
	8, 1, //MT_LASER_G
	8, 1, //MT_LASER_B
	8, 1, //MT_LASER_W
	8, 3, //MT_REFLECTOR
	8, 3, //MT_BENDER
	8, 1, //MT_DOPPLER
	4, 1, //MT_FILTER_R
	4, 1, //MT_FILTER_G
	4, 1, //MT_FILTER_B
	4, 3, //MT_QUAD_REFL
	4, 3, //MT_QUAD_BEND
	4, 3, //MT_CONDUIT
	4, 1, //MT_MUL_FILTER
	8, 1, //MT_PRISM
	8, 1, //MT_TANGLER
	4, 3, //MT_SPLITTER
	8, 1, //MT_LOGIC_GATE
	2, 1, //MT_COMPLEMENTOR
	1, 1, //MT_STARBURST
	1, 1, //MT_TWISTER
	1, 1, //MT_TELEPORTER
	1, 1, //MT_OCCUPIED
	1, 1, //MT_TARGET
	1, 1, //MT_HIT_TARGET
};

CMirror::CMirror(int nGrid, byte type_dir, bool bFixed)
			: m_nGrid(nGrid), type_dir(type_dir), m_bFixed(bFixed)
{
	int type = MIRROR_TYPE(type_dir);
	int dir = MIRROR_DIR(type_dir);

	CRect xRect(0, 0, 24, 24);
	Set(&ImageMirror, xRect, xRect + MIRROR_SRC(type, dir), MirrorAttr[type][1]);

	SetCease();
	SetDragMode(MK_LBUTTON);

	memset(OnMirrorProc, NULL, sizeof(OnMirrorProc));
}

CMirror::~CMirror()
{
}

bool CMirror::CanRotate() const
{
	return MirrorAttr[MIRROR_TYPE(type_dir)][0] > 1;
}

void CMirror::Rotate(bool bClockwise)
{
	int type = MIRROR_TYPE(type_dir);
	int dir = MIRROR_DIR(type_dir);

	//更新镜子的方向
	int max_turns = MirrorAttr[type][0];
	if (max_turns <= 1)
		return;
	dir = (bClockwise ? dir + 1 : dir + max_turns - 1) % max_turns;

	type_dir = MAKE_MIRROR(type, dir);
	SetSrcPos(MIRROR_SRC(type, dir));
}

void CMirror::MoveTo(CPoint point)
{
	SetDrawPos(point + -m_xOffset + CPoint(-12, -12));
}
