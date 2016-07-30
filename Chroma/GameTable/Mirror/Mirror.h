/*****************************************************************************
 *                                  镜子介绍
 *
 * Laser           镭射发光器
 * Wall            墙体
 * Target          靶标
 * Conduit         狭缝
 * Reflector       垂直反射镜
 * Bender          45度角反射镜
 * Splitter        分光镜
 * Fliter          滤色镜
 * Prism           三棱镜
 * Doppler         多普勒镜
 * Tangler         量子镜
 * Quad-Reflector  双垂直反射镜
 * Quad-Bender     双45度角反射镜
 * LogicGate       逻辑门镜
 * Starburst       星爆镜
 * Complementor    补色镜
 * Multi-Filter    多通滤色镜
 * Twister         龙卷风镜
 * Occupied-Square 通透镜
 * Teleporter      穿梭镜
 *
 *****************************************************************************/
#ifndef __Mirror_h__
#define __Mirror_h__

//镜子的类型和方向
#define MIRROR_TYPE(n)				((n) >> 3)
#define MIRROR_DIR(n)				((n) & 0x7)

#define MAKE_MIRROR(type, dir)		(((type) << 3) + (dir))

//镜子的大小和源位置
#define FMUL24(n)					(((n) << 4) + ((n) << 3)) //快乘24

#define MIRROR_SIZE					CSize(24, 24)
#define MIRROR_SRC(type, dir)		CPoint(FMUL24(dir), FMUL24(type))

#define MP_LBUTTONUP				0
#define MP_RBUTTONUP				1

class CMirror;
typedef boost::function<void(CMirror *, UINT nFlags, CPoint point)>		 FN_MirrorProc;

class CMirror : public CButton {
  public:
	CMirror(int nGrid, byte type_dir, bool bFixed = false);
	virtual ~CMirror();

  public:
	void			 SetGrid(int nGrid);
	int				 GetGrid() const;
	void			 SetTypeDir(byte type_dir);
	int				 GetTypeDir() const;
	void			 SetFixed(bool bFixed);
	bool			 IsFixed() const;
	bool			 IsTarget() const;
	void			 SetMirrorProc(int nType, FN_MirrorProc MirrorProc);
	FN_MirrorProc	 GetMirrorProc(int nType) const;

  public:
	bool			 CanRotate() const;
	void			 Rotate(bool bClockwise);
	void			 MoveTo(CPoint point);

  protected:
	//bool			 IsEnable(int nBtnIndex) const;
	int				 GetDragThrs() const;
	bool			 CheckDragMode(UINT nFlags);

  protected:
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);
	void			 OnLButtonUp(UINT nFlags, CPoint point);
	void			 OnRButtonUp(UINT nFlags, CPoint point);

  protected:
	byte			 type_dir;
	int				 m_nGrid;
	bool			 m_bFixed;
	FN_MirrorProc	 OnMirrorProc[3];
};

inline void CMirror::SetGrid(int nGrid)
{
	m_nGrid = nGrid;
}

inline int CMirror::GetGrid() const
{
	return m_nGrid;
}

inline void CMirror::SetTypeDir(byte type_dir)
{
	this->type_dir = type_dir;
}

inline int CMirror::GetTypeDir() const
{
	return type_dir;
}

inline void CMirror::SetFixed(bool bFixed)
{
	m_bFixed = bFixed;
}

inline bool CMirror::IsFixed() const
{
	return m_bFixed;
}

inline bool CMirror::IsTarget() const
{
	return MIRROR_TYPE(type_dir) == MT_TARGET
		|| MIRROR_TYPE(type_dir) == MT_HIT_TARGET;
}

inline void CMirror::SetMirrorProc(int nType, FN_MirrorProc MirrorProc)
{
	OnMirrorProc[nType] = MirrorProc;
}

inline int CMirror::GetDragThrs() const
{
	return 3; //拖动门限
}

inline bool CMirror::CheckDragMode(UINT nFlags)
{
	return !IsFixed() && CButton::CheckDragMode(nFlags);
}

inline bool CMirror::OnMouseEnter(UINT nFlags, CPoint point)
{
	return true;
}

inline bool CMirror::OnMouseLeave(UINT nFlags, CPoint point)
{
	return true;
}

inline void CMirror::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (OnMirrorProc[MP_LBUTTONUP])
		OnMirrorProc[MP_LBUTTONUP](this, nFlags, point);
}

inline void CMirror::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (OnMirrorProc[MP_RBUTTONUP])
		OnMirrorProc[MP_RBUTTONUP](this, nFlags, point);
}

inline FN_MirrorProc CMirror::GetMirrorProc(int nType) const
{
	return OnMirrorProc[nType];
}

#endif
