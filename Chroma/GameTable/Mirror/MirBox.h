#ifndef __MirBox_h__
#define __MirBox_h__

#include "MirPanel.h"

class CMirror;
class CMirBox : public CMirPanel {
  public:
	SINGLETON(CMirBox);
	virtual ~CMirBox();

  private:
	CMirBox();

  public:
	bool			 Initial();
	int				 GetGrid(CPoint point) const;
	bool			 CanRotate(int nGrid) const;
	bool			 IsLightRelevant() const;

  private:
	void			 _AddMirror(int nGrid, byte type_dir);
	void			 _DelMirror(int nGrid);

  public:
	void			 AddMirror(int nGrid, byte type_dir);
	void			 DelMirror(int nGrid);
	void			 ClearMirror();
	void			 RotateMirror(int nGrid, bool bClockwise);

  public:
	void			 WriteMirrorData(TCHAR *&pBuff);
	bool			 ReadMirrorData(TCHAR *&pBuff);

  protected:
	int				 m_nMirror[6 * 4];
	CMirror			*m_pMirror[6 * 4];
};

inline bool CMirBox::IsLightRelevant() const
{
	return false;
}

#define g_pMirBox			CMirBox::GetInstancePtr()

#endif
