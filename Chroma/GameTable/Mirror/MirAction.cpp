#include "StdAfx.h"
#include "Mirror\MirAction.h"
#include "Mirror\Mirror.h"
#include "Mirror\MirPanel.h"
#include "Mirror\MirChoice.h"
#include "GameTable.h"

CMirAction::CMirAction()
{
	m_pMoveMirror = new CMirror(0, 0, false);
	m_pMoveMirror->SetDepth(0);

	m_bDragMirror = false;
}

CMirAction::~CMirAction()
{
	delete m_pMoveMirror;
	m_pMoveMirror = NULL;
}

void CMirAction::RotateMirror(CMirror *pMirror, bool bClosewise)
{
	if (pMirror->IsFixed())
		return;

	CMirPanel *pMirPanel = (CMirPanel *)(pMirror->m_pHome);
	pMirPanel->RotateMirror(pMirror->GetGrid(), bClosewise);

	//和光线相关时，需要更新光线
	if (pMirPanel->IsLightRelevant())
		g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
}

void CMirAction::PickMirror(CMirror *pMirror, CPoint point)
{
	//记录镜子的归属，格点，类型，方向
	m_pMirPanel = (CMirPanel *)(pMirror->m_pHome);
	m_nPickGrid = m_pMirPanel->GetGrid(point);
	m_byTypeDir = pMirror->GetTypeDir();

	//设置拖动的镜子，并添加到顶层
	m_pMoveMirror->SetTypeDir(m_byTypeDir);
	m_pMoveMirror->SetSrcPos(MIRROR_SRC(MIRROR_TYPE(m_byTypeDir), MIRROR_DIR(m_byTypeDir)));
	m_pMoveMirror->SetMouseProc(MP_MOUSEDRAG, pMirror->GetMouseProc(MP_MOUSEDRAG));
	m_pMoveMirror->SetMouseProc(MP_MOUSEDROP, pMirror->GetMouseProc(MP_MOUSEDROP));
	m_pMoveMirror->MoveTo(point);

	g_pZ2coo->AddObject(m_pMoveMirror);
	g_pZ2coo->SetActived(m_pMoveMirror);

	//从归属删除镜子
	m_pMirPanel->DelMirror(m_nPickGrid);
	m_bDragMirror = true;
}

void CMirAction::DragMirror(CMirror *pMirror, CPoint point)
{
	pMirror->MoveTo(point);
}

void CMirAction::DropMirror(CMirror *pMirror, CPoint point)
{
	g_pZ2coo->DelObject(pMirror);

	//查找落点位置的对象
	CObject *pObject = g_pZ2coo->FindExcept(point, pMirror);
	CMirPanel *pMirPanel = dynamic_cast<CMirPanel *>(pObject);
	if (pMirPanel)
	{
		int nGrid = pMirPanel->GetGrid(point);
		if (nGrid != INVALID_GRID)
		{
			pMirPanel->AddMirror(nGrid, m_byTypeDir);

			//源或者目的面板有一个和光线相关时，就需要更新光线
			if (pMirPanel->IsLightRelevant() || m_pMirPanel->IsLightRelevant())
				g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
			m_bDragMirror = false;
			return;
		}
	}

	//放回原来的位置，光线不变化
	m_pMirPanel->AddMirror(m_nPickGrid, m_byTypeDir);
	m_bDragMirror = false;
}

void CMirAction::ChoiceMirror(CMirPanel *pMirPanel, CPoint point, CMirror *pAnchorMirror)
{
	if (!g_bEditState)
		return;

	//创建模态选择框并调整位置
	CMirChoice tMirChoice(point);
	if (!tMirChoice.Initial())
		return;
	CRect rect;
	g_pGameTable->GetClientRect(&rect);
	tMirChoice.AdjustAnchor(point, rect);

	//弹出模态选择框
	byte type_dir = tMirChoice.DoModal();

	//如果锚点处有镜子，则删除
	if (pAnchorMirror)
		pMirPanel->DelMirror(pAnchorMirror->GetGrid());

	//如果所选镜子不为空，则添加
	if (type_dir != 0)
	{
		int nGrid = pMirPanel->GetGrid(point);
		if (nGrid == INVALID_GRID)
			return;
		pMirPanel->AddMirror(nGrid, type_dir);
	}

	if (pMirPanel->IsLightRelevant())
		g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
}
