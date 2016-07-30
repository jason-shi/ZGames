#include "StdAfx.h"
#include "Mirror.h"
#include "MirBoard.h"
#include "MirAction.h"
#include "Light\Light.h"
#include "GameTable\GameTable.h"
#include <boost\bind.hpp>

#define Board(x, y)				m_nMirror[x + (y << 4)]

//设置光线
#define MAKE_BEAM(dir, color)	(((dir) << 3) + (color))

CMirBoard::CMirBoard() : CMirPanel()
{
	m_pLight = NULL;

	memset(m_nMirror, 0, sizeof(m_nMirror));
	memset(m_pMirror, NULL, sizeof(m_pMirror));

	Set(NULL, CPoint(49, 19), CPoint(0, 0), CSize(360, 360), 1);
	SetCease();
}

CMirBoard::~CMirBoard()
{
	delete m_pLight;
	m_pLight = NULL;
}

bool CMirBoard::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pLight = new CLight();
	if (!m_pLight || !m_pLight->Initial())
		return false;
	AddObject(m_pLight);

	return true;
}

void CMirBoard::UpdateLight()
{
	TraceFromLaser();
	TraceViaMirror();
	ProcessTarget();

	//绘制光线
	m_pLight->DrawLight(m_bmpLight);
}

int CMirBoard::GetGrid(CPoint point) const
{
	int nX = (point.x - m_xDrawPos.x) / 24;
	int nY = (point.y - m_xDrawPos.y) / 24;
	return nX >= 15 || nY >= 15 ? INVALID_GRID : nX + (nY << 4);
}

bool CMirBoard::CanRotate(int nGrid) const
{
	return m_pMirror[nGrid]
		&& m_pMirror[nGrid]->CanRotate();
}

void CMirBoard::_AddMirror(int nGrid, byte type_dir)
{
	m_pMirror[nGrid] = new CMirror(nGrid, type_dir);
	m_pMirror[nGrid]->SetDrawPos((nGrid & 15) * 24, (nGrid >> 4) * 24);
	m_pMirror[nGrid]->SetHome(this);
	m_pMirror[nGrid]->SetMouseProc(MP_LCLICK, boost::bind(&CMirBoard::OnLClick, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEPICK, boost::bind(&CMirBoard::OnMousePick, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEDRAG, boost::bind(&CMirBoard::OnMouseDrag, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMouseProc(MP_MOUSEDROP, boost::bind(&CMirBoard::OnMouseDrop, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMirrorProc(MP_LBUTTONUP, boost::bind(&CMirBoard::OnLButtonUp, this, _1, _2, _3));
	m_pMirror[nGrid]->SetMirrorProc(MP_RBUTTONUP, boost::bind(&CMirBoard::OnRButtonUp, this, _1, _2, _3));
	AddObject(m_pMirror[nGrid]);

	if (m_pMirror[nGrid]->IsTarget())
	{
		//添加靶标，加速靶标检测
		m_vecTarget.push_back(m_pMirror[nGrid]);
	}
}

void CMirBoard::_DelMirror(int nGrid)
{
	//同时删除靶标
	VECMIRROR::iterator iter;
	for (iter = m_vecTarget.begin(); iter != m_vecTarget.end(); iter++)
	{
		if (*iter == m_pMirror[nGrid])
		{
			m_vecTarget.erase(iter);
			break;
		}
	}

	DelObject(m_pMirror[nGrid]);

	delete m_pMirror[nGrid];
	m_pMirror[nGrid] = NULL;
}

void CMirBoard::AddMirror(int nGrid, byte type_dir)
{
	DelMirror(nGrid);

	//删除后再添加镜子
	m_nMirror[nGrid] = type_dir;
	if (m_nMirror[nGrid] == 0)
		return;

	_AddMirror(nGrid, type_dir);
}

void CMirBoard::DelMirror(int nGrid)
{
	if (m_nMirror[nGrid] == 0)
		return;
	m_nMirror[nGrid] = 0;

	_DelMirror(nGrid);
}

void CMirBoard::ClearMirror()
{
	for (int nGrid = 0; nGrid < 256; nGrid++)
		DelMirror(nGrid);
}

void CMirBoard::RotateMirror(int nGrid, bool bClockwise)
{
	m_pMirror[nGrid]->Rotate(bClockwise);

	//旋转后的type_dir
	m_nMirror[nGrid] = m_pMirror[nGrid]->GetTypeDir();
}

void CMirBoard::SetFixed(bool bFixed)
{
	for (int nGrid = 0; nGrid < 256; nGrid++)
	{
		//固定或不固定镜子
		if (m_pMirror[nGrid])
			m_pMirror[nGrid]->SetFixed(bFixed);
	}
}

void CMirBoard::WriteMirrorData(TCHAR *&pBuff)
{
	bool bMirFixed[16 * 16] = { 0 };

	//将在盘面上固定的镜子保存为数组
	for (int nGrid = 0; nGrid < 256; nGrid++)
	{
		if (m_pMirror[nGrid])
		{
			if (m_pMirror[nGrid]->IsFixed())
				bMirFixed[nGrid] = true;
		}
	}

	//从盘面的左上开始保存
	for (int y = 0; y < 15; y++)
	{
		int skip = 0;
		for (int x = 0; x < 15; x++)
		{
			int nGrid = x + (y << 4);

			int type = MIRROR_TYPE(m_nMirror[nGrid]);
			if (type == 0)
				skip++;
			else
			{
				if (skip != 0)
				{
					*pBuff++ = skip + (skip < 10 ? L'0' : L'a' - 10); //'1'到'9',L'a'到'f'
					skip = 0;
				}
				if (TblMirToAlpha[type])
				{
					*pBuff++ = TblMirToAlpha[type]; //'A'到'Z','g'到'z'
					*pBuff++ = MIRROR_DIR(m_nMirror[nGrid]) + L'0'; //L'0'到'8'
				}
				if (bMirFixed[nGrid])
					*pBuff++ = L'_';
			}
		}
		//每行结束
		if (skip != 0)
			*pBuff++ = skip + (skip < 10 ? L'0' : L'a' - 10); //'1'到'9',L'a'到'f'
		*pBuff++ = L'/';
	}
}

bool CMirBoard::ReadMirrorData(TCHAR *&pBuff)
{
	//从盘面的左上开始读取
	int x = 0, y = 0;
	while (TCHAR ch = *pBuff++)
	{
		if (x > 15)
			return false;

		if (ch == L'/') //新的一行
			x = 0, y++;
		else if (TblAlphaToMir[(byte)ch] != 0) //镜型
		{
			if (*pBuff < L'0' || *pBuff > L'7') //镜向
				return false;
			byte type_dir = MAKE_MIRROR(TblAlphaToMir[(byte)ch], *pBuff++ - L'0');

			bool bFixed = false; //固定
			if (*pBuff == L'_')
			{
				*pBuff++;
				bFixed = true;
			}

			int nGrid = x++ + (y << 4);
			AddMirror(nGrid, type_dir);
			m_pMirror[nGrid]->SetFixed(bFixed);
		}
		else if (ch >= L'1' && ch <= L'9') //跳过
			x += ch - L'0';
		else if (ch >= L'a' && ch <= L'f') //跳过
			x += ch - L'a' + 10;
		else
			return false;

		if (y == 15)
			break;
	}
	return true;
}

void CMirBoard::TraceFromLaser()
{
	memset(m_bmpLight, 0, sizeof(m_bmpLight));

	int color = 0;
	for (int nGrid = 0; nGrid < 256; nGrid++)
	{
		int type = MIRROR_TYPE(m_nMirror[nGrid]);
		switch (type)
		{
		case MT_LASER_R:
			color = 4;
			break;
		case MT_LASER_G:
			color = 2;
			break;
		case MT_LASER_B:
			color = 1;
			break;
		case MT_LASER_W:
			color = 7;
			break;
		default:
			continue; //不是光源
		}
		m_vecGrid.push_back(nGrid);

		int dir = MIRROR_DIR(m_nMirror[nGrid]);
		int out = DIR(dir + 4);
		m_vecBeam.push_back(MAKE_BEAM(out, color));
	}
}

void CMirBoard::TraceViaMirror()
{
	while (!m_vecBeam.empty())
	{
		byte nBeam = m_vecBeam.back();
		byte nGrid = m_vecGrid.back();
		m_vecBeam.pop_back();
		m_vecGrid.pop_back();

		while (nBeam)
		{
			//当前格子的输出光束的方向和颜色
			if ((m_bmpLight[nGrid] & TblLightMask[nBeam]) == TblLightMask[nBeam])
				break;
			m_bmpLight[nGrid] |= TblLightMask[nBeam];

			//追踪不压栈的光束
			if ((nGrid = TblNextGrid[(nGrid << 3) + (nBeam >> 3)]) == 255) //格点+出向
				break;

			if (TblSpecMirror[MIRROR_TYPE(m_nMirror[nGrid])])
			{
				//对特殊镜子的处理
				if ((nGrid = ViaSpecMirror(nGrid, nBeam)) == 255)
					break;
			}
			nBeam = TblMirror[(m_nMirror[nGrid] << 6) + nBeam];
		}
	}
}

void CMirBoard::ProcessTarget()
{
	static int TColor[8] = { 4, 2, 1, 6, 5, 3, 7, 0 }; //“镜向”->颜色

	m_bSuccess = true;
	//没有靶标时为失败
	if (m_vecTarget.empty())
		m_bSuccess = false;

	VECMIRROR::iterator iter;
	for (iter = m_vecTarget.begin(); iter != m_vecTarget.end(); iter++)
	{
		int type_dir = (*iter)->GetTypeDir();
		int dir = MIRROR_DIR(type_dir);

		//设置类型和源位置
		int type;
		if (TColor[dir] == GetLightColor((*iter)->GetGrid()))
			type = MT_HIT_TARGET;
		else
		{
			m_bSuccess = false;
			type = MT_TARGET;
		}
		(*iter)->SetTypeDir(MAKE_MIRROR(type, dir));
		(*iter)->SetSrcPos(MIRROR_SRC(type, dir));
	}
}

byte CMirBoard::ViaSpecMirror(byte nGrid, byte nBeam)
{
	int nBeamIndex;
	switch (MIRROR_TYPE(m_nMirror[nGrid]))
	{
	case MT_TELEPORTER: //穿梭镜
		while ((nGrid = TblNextGrid[(nGrid << 3) + (nBeam >> 3)]) != 255)
		{
			if (MIRROR_TYPE(m_nMirror[nGrid]) == MT_TELEPORTER)
				break;
		}
		break;
	case MT_PRISM: //三棱镜
		nBeamIndex = (MIRROR_DIR(m_nMirror[nGrid]) << 6) + nBeam;
		if (TblPrismG[nBeamIndex] != 0)
		{
			m_vecBeam.push_back(TblPrismG[nBeamIndex]);
			m_vecGrid.push_back(nGrid);
		}
		if (TblPrismB[nBeamIndex] != 0)
		{
			m_vecBeam.push_back(TblPrismB[nBeamIndex]);
			m_vecGrid.push_back(nGrid);
		}
		break;
	case MT_SPLITTER: //分光镜
		m_vecBeam.push_back(TblSplitter[(MIRROR_DIR(m_nMirror[nGrid]) << 6) + nBeam]);
		m_vecGrid.push_back(nGrid);
		break;
	case MT_STARBURST: //星爆镜
		m_vecBeam.push_back(TblStarburst1[nBeam]);
		m_vecGrid.push_back(nGrid);
		m_vecBeam.push_back(TblStarburst2[nBeam]);
		m_vecGrid.push_back(nGrid);
		m_vecBeam.push_back(TblStarburst3[nBeam]);
		m_vecGrid.push_back(nGrid);
		break;
	case MT_TANGLER: //能量镜
		for (int mask = 1; mask <= 4; mask <<= 1)
		{
			//按红绿蓝颜色分量单独处理
			if (nBeam & mask)
			{
				int nBeamRGB = MAKE_BEAM(nBeam >> 3, nBeam & mask);
				SymmTraceTangler(nGrid, TblMirror[(m_nMirror[nGrid] << 6) + nBeamRGB],
								 nGrid, TblTangler[(MIRROR_DIR(m_nMirror[nGrid]) << 6) + nBeamRGB]);
			}
		}
		return 255;
	}
	return nGrid;
}

int CMirBoard::GetLightColor(int nGrid)
{
	int color = 0;
	for (int dir = 0; dir < 8; dir++)
	{
		int nNextGrid = TblNextGrid[(nGrid << 3) + dir];
		if (nNextGrid != 255)
		{
			//入射光颜色
			int dir_in = (dir + 4) & 0x7;
			color |= m_bmpLight[nNextGrid] >> (dir_in * 3);
		}
	}
	return color & 0x7;
}

byte CMirBoard::ConstructDoppler(int nGrid, int nBeam, int nBeamDir)
{
	nGrid = TblNextGrid[(nGrid << 3) + (nBeam >> 3)];

	//如果光线通过多普勒镜，则构造反向的多普勒镜
	if (MIRROR_TYPE(m_nMirror[nGrid]) == MT_DOPPLER)
	{
		int in = nBeam >> 3;
		int dir = MIRROR_DIR(m_nMirror[nGrid]);
		int angle_in = (in - dir + 8) & 0x7;

		if (angle_in == 0)
			return MAKE_MIRROR(MT_DOPPLER, DIR(nBeamDir + 4));
		if (angle_in == 4)
			return MAKE_MIRROR(MT_DOPPLER, DIR(nBeamDir + 0));
	}
	return MAKE_MIRROR(MT_NOTHING, 0);
}

int CMirBoard::TraceOneGrid(int &nGrid, int nBeam)
{
	if (MIRROR_TYPE(m_nMirror[nGrid]) == MT_TELEPORTER)
	{
		while ((nGrid = TblNextGrid[(nGrid << 3) + (nBeam >> 3)]) != 255)
		{
			if (MIRROR_TYPE(m_nMirror[nGrid]) == MT_TELEPORTER)
				break;
		}
		if (nGrid == 255)
			return 0;
	}
	if (MIRROR_TYPE(m_nMirror[nGrid]) == MT_PRISM)
	{
		int nBeamIndex = (MIRROR_DIR(m_nMirror[nGrid]) << 6) + nBeam;
		//最多存在一个红，绿，蓝分量
		if (TblPrismG[nBeamIndex] != 0)
			return TblPrismG[nBeamIndex];
		if (TblPrismB[nBeamIndex] != 0)
			return TblPrismB[nBeamIndex];
	}
	return TblMirror[(m_nMirror[nGrid] << 6) + nBeam];
}

void CMirBoard::SymmTraceTangler(int nGrid1, int nBeam1, int nGrid2, int nBeam2)
{
	int nNextGrid1, nNextGrid2;
	while (nBeam1 && nBeam2) //光束终止
	{
		nNextGrid1 = TblNextGrid[(nGrid1 << 3) + (nBeam1 >> 3)];
		nNextGrid2 = TblNextGrid[(nGrid2 << 3) + (nBeam2 >> 3)];
		if (nNextGrid1 == 255 || nNextGrid2 == 255) //出界
			break;

		byte type1 = MIRROR_TYPE(m_nMirror[nNextGrid1]);
		byte type2 = MIRROR_TYPE(m_nMirror[nNextGrid2]);
		if (TblQuantumEnd[type1] || TblQuantumEnd[type2]) //能量终止
			break;

		//真实颜色的光束
		m_bmpLight[nGrid1] |= TblLightMask[nBeam1];
		m_bmpLight[nGrid2] |= TblLightMask[nBeam2];

		//经过实际的镜子
		int nNextBeam1 = TraceOneGrid(nNextGrid1, nBeam1);
		int nNextBeam2 = TraceOneGrid(nNextGrid2, nBeam2);

		//经过反向的多普勒镜
		byte doppler1 = ConstructDoppler(nGrid2, nBeam2, nNextBeam1 >> 3);
		byte doppler2 = ConstructDoppler(nGrid1, nBeam1, nNextBeam2 >> 3);

		nGrid1 = nNextGrid1;
		nGrid2 = nNextGrid2;
		nBeam1 = TblMirror[(doppler1 << 6) + nNextBeam1];
		nBeam2 = TblMirror[(doppler2 << 6) + nNextBeam2];
	}
	//能量终止，按照普通光束处理
	m_vecGrid.push_back(nGrid1);
	m_vecBeam.push_back(nBeam1);
	m_vecGrid.push_back(nGrid2);
	m_vecBeam.push_back(nBeam2);
}
