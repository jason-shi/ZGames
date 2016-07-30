#include "StdAfx.h"
#include "Mirror.h"
#include "MirBox.h"
#include <boost\bind.hpp>

CMirBox::CMirBox() : CMirPanel()
{
	memset(m_nMirror, 0, sizeof(m_nMirror));
	memset(m_pMirror, NULL, sizeof(m_pMirror));

	Set(NULL, CPoint(449, 9), CPoint(0, 0), CSize(155, 103), 1);
	SetCease();
}

CMirBox::~CMirBox()
{
}

bool CMirBox::Initial()
{
	if (!CPanel::Initial())
		return false;

	return true;
}

int CMirBox::GetGrid(CPoint point) const
{
	int ax = point.x - m_xDrawPos.x;
	int ay = point.y - m_xDrawPos.y;

	//格子中间为无效区域
	if (ax % 26 >= 24 || ay % 26 >= 24)
		return INVALID_GRID;

	int nX = ax / 26;
	int nY = ay / 26;

	return nX >= 6 || nY >= 4 ? INVALID_GRID : nX + (nY * 6);
}

bool CMirBox::CanRotate(int nGrid) const
{
	return m_pMirror[nGrid]
		&& m_pMirror[nGrid]->CanRotate();
}

void CMirBox::_AddMirror(int nGrid, byte type_dir)
{
	m_pMirror[nGrid] = new CMirror(nGrid, type_dir);
	m_pMirror[nGrid]->SetDrawPos(nGrid % 6 * 26, nGrid / 6 * 26);
	m_pMirror[nGrid]->SetHome(this);
	m_pMirror[nGrid]->SetMouseProc(MP_LCLICK, boost::bind(&CMirBox::OnLClick, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEPICK, boost::bind(&CMirBox::OnMousePick, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEDRAG, boost::bind(&CMirBox::OnMouseDrag, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEDROP, boost::bind(&CMirBox::OnMouseDrop, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMirrorProc(MP_LBUTTONUP, boost::bind(&CMirBox::OnLButtonUp, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMirrorProc(MP_RBUTTONUP, boost::bind(&CMirBox::OnRButtonUp, this, _1, _2, _3));

	AddObject(m_pMirror[nGrid]);
}

void CMirBox::_DelMirror(int nGrid)
{
	DelObject(m_pMirror[nGrid]);

	delete m_pMirror[nGrid];
	m_pMirror[nGrid] = NULL;
}

void CMirBox::AddMirror(int nGrid, byte type_dir)
{
	DelMirror(nGrid);

	//删除后再添加镜子
	m_nMirror[nGrid] = type_dir;
	if (m_nMirror[nGrid] == 0)
		return;

	_AddMirror(nGrid, type_dir);
}

void CMirBox::DelMirror(int nGrid)
{
	if (m_nMirror[nGrid] == 0)
		return;
	m_nMirror[nGrid] = 0;

	_DelMirror(nGrid);
}

void CMirBox::ClearMirror()
{
	for (int nGrid = 0; nGrid < 24; nGrid++)
		DelMirror(nGrid);
}

void CMirBox::RotateMirror(int nGrid, bool bClockwise)
{
	m_pMirror[nGrid]->Rotate(bClockwise);

	//旋转后的type_dir
	m_nMirror[nGrid] = m_pMirror[nGrid]->GetTypeDir();
}

void CMirBox::WriteMirrorData(TCHAR *&pBuff)
{
	//从镜盒的左上开始保存
	for (int y = 0; y < 4; y++)
	{
		int skip = 0;
		for (int x = 0; x < 6; x++)
		{
			int nGrid = x + y * 6;

			int type = MIRROR_TYPE(m_nMirror[nGrid]);
			if (type == 0)
				skip++;
			else
			{
				if (skip != 0)
				{
					*pBuff++ = skip + L'0'; //'1'到'6'
					skip = 0;
				}
				if (TblMirToAlpha[type])
				{
					*pBuff++ = TblMirToAlpha[type]; //'A'到'Z','g'到'z'
					*pBuff++ = MIRROR_DIR(m_nMirror[nGrid]) + L'0'; //L'0'到'8'
				}
			}
		}
		//每行结束
		if (skip != 0)
			*pBuff++ = skip + L'0'; //'1'到'6'
		if (y != 3)
			*pBuff++ = L'/';
	}
	*pBuff++ = L'\0';
}

bool CMirBox::ReadMirrorData(TCHAR *&pBuff)
{
	//从镜盒的左上开始读取
	int x = 0, y = 0;
	while (TCHAR ch = *pBuff++)
	{
		if (x > 6 || y > 4)
			return false;
		if (ch == L'/') //新的一行
			x = 0, y++;
		else if (TblAlphaToMir[(byte)ch] != 0) //镜型
		{
			if (*pBuff < L'0' || *pBuff > L'7') //镜向
				return false;

			int nGrid = x++ + y * 6;
			byte type_dir = MAKE_MIRROR(TblAlphaToMir[(byte)ch], *pBuff++ - L'0');

			AddMirror(nGrid, type_dir);
		}
		else if (ch >= L'1' && ch <= L'6') //跳过
			x += ch - L'0';
		else
			return false;
	}
	return true;
}
