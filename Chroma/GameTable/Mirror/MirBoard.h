#ifndef __MirBoard_h__
#define __MirBoard_h__

#include "MirPanel.h"

class CMirror;
class CLight;
class CMirBoard : public CMirPanel {
	typedef std::vector<CMirror *>		VECMIRROR;
  public:
	SINGLETON(CMirBoard);
	virtual ~CMirBoard();

  private:
	CMirBoard();

  public:
	bool			 Initial();
	void			 UpdateLight();
	bool			 IsSuccess() const;

  public:
	int				 GetGrid(CPoint point) const;
	bool			 CanRotate(int nGrid) const;
	bool			 IsLightRelevant() const;

  protected:
	void			 _AddMirror(int nGrid, byte type_dir);
	void			 _DelMirror(int nGrid);

  public:
	void			 AddMirror(int nGrid, byte type_dir);
	void			 DelMirror(int nGrid);
	void			 ClearMirror();
	void			 RotateMirror(int nGrid, bool bClockwise);

  public:
	void			 SetFixed(bool bFixed);

  public:
	void			 WriteMirrorData(TCHAR *&pBuff);
	bool			 ReadMirrorData(TCHAR *&pBuff);

  protected:
	void			 TraceFromLaser();
	void			 TraceViaMirror();
	void			 ProcessTarget();
	byte			 ViaSpecMirror(byte nGrid, byte nBeam);
	int				 GetLightColor(int nGrid);

  protected:
	byte			 ConstructDoppler(int nGrid, int nBeam, int nBeamDir); //构造反向多普勒镜
	int				 TraceOneGrid(int &nGrid, int nBeam); //跟踪一个格子的距离
	void			 SymmTraceTangler(int nGrid1, int nBeam1, int nGrid2, int nBeam2); //对称跟踪Tangler镜

  protected:
	int				 m_nMirror[16 * 16]; //实际大小15 * 15
	CMirror			*m_pMirror[16 * 16];

  protected:
	int				 m_bmpLight[256]; //rgb rgb ... rgb
	CLight			*m_pLight;
	bool			 m_bSuccess;

  private:
	VECMIRROR			 m_vecTarget;
	std::vector<byte>	 m_vecBeam;
	std::vector<byte>	 m_vecGrid;

  protected:
	CMirror			*m_pChoiceMirror;
};

inline bool CMirBoard::IsSuccess() const
{
	return m_bSuccess;
}

inline bool CMirBoard::IsLightRelevant() const
{
	return true;
}

#define g_pMirBoard			CMirBoard::GetInstancePtr()

#endif
