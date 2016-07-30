#ifndef __Gate_h__
#define __Gate_h__

#define GT_CONCEAL			0
#define GT_ACCESS			1
#define GT_SOLVED			2
#define GT_CURRENT			3 //临时状态

class CGate : public CPanel {
  public:
	CGate(int nGate);
	virtual ~CGate();

  public:
	bool			 Initial();
	void			 ShowNumber(bool bCurrent);

  public:
	int				 GetGate() const;
	void			 SetStatus(int nStatus);
	int				 GetStatus() const;
	bool			 CanAccess() const;
	bool			 IsSolved() const;

  public:
	void			 LoadGate();
	void			 SaveGate();
	TCHAR			*WriteGateData(); //长度 关数 状态 [属地 位置 镜子]

  public:
	void			 OnLeaveGate();
	void			 OnEnterGate();

  protected:
	int				 m_nGate;
	int				 m_nStatus;

  protected:
	CSprite			 SpriteNumber1;
	CSprite			 SpriteNumber2;
};

CGate				*CreateGate(int nGate);

inline int CGate::GetGate() const
{
	return m_nGate;
}

inline void CGate::SetStatus(int nStatus)
{
	m_nStatus = nStatus;
}

inline int CGate::GetStatus() const
{
	return m_nStatus;
}

inline bool CGate::CanAccess() const
{
	return g_bEditState || m_nStatus != GT_CONCEAL;
}

inline bool CGate::IsSolved() const
{
	return m_nStatus == GT_SOLVED;
}

#endif
