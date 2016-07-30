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

	//�͹������ʱ����Ҫ���¹���
	if (pMirPanel->IsLightRelevant())
		g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
}

void CMirAction::PickMirror(CMirror *pMirror, CPoint point)
{
	//��¼���ӵĹ�������㣬���ͣ�����
	m_pMirPanel = (CMirPanel *)(pMirror->m_pHome);
	m_nPickGrid = m_pMirPanel->GetGrid(point);
	m_byTypeDir = pMirror->GetTypeDir();

	//�����϶��ľ��ӣ�����ӵ�����
	m_pMoveMirror->SetTypeDir(m_byTypeDir);
	m_pMoveMirror->SetSrcPos(MIRROR_SRC(MIRROR_TYPE(m_byTypeDir), MIRROR_DIR(m_byTypeDir)));
	m_pMoveMirror->SetMouseProc(MP_MOUSEDRAG, pMirror->GetMouseProc(MP_MOUSEDRAG));
	m_pMoveMirror->SetMouseProc(MP_MOUSEDROP, pMirror->GetMouseProc(MP_MOUSEDROP));
	m_pMoveMirror->MoveTo(point);

	g_pZ2coo->AddObject(m_pMoveMirror);
	g_pZ2coo->SetActived(m_pMoveMirror);

	//�ӹ���ɾ������
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

	//�������λ�õĶ���
	CObject *pObject = g_pZ2coo->FindExcept(point, pMirror);
	CMirPanel *pMirPanel = dynamic_cast<CMirPanel *>(pObject);
	if (pMirPanel)
	{
		int nGrid = pMirPanel->GetGrid(point);
		if (nGrid != INVALID_GRID)
		{
			pMirPanel->AddMirror(nGrid, m_byTypeDir);

			//Դ����Ŀ�������һ���͹������ʱ������Ҫ���¹���
			if (pMirPanel->IsLightRelevant() || m_pMirPanel->IsLightRelevant())
				g_pGameTable->SendMessage(WM_UPDATE_LIGHT);
			m_bDragMirror = false;
			return;
		}
	}

	//�Ż�ԭ����λ�ã����߲��仯
	m_pMirPanel->AddMirror(m_nPickGrid, m_byTypeDir);
	m_bDragMirror = false;
}

void CMirAction::ChoiceMirror(CMirPanel *pMirPanel, CPoint point, CMirror *pAnchorMirror)
{
	if (!g_bEditState)
		return;

	//����ģ̬ѡ��򲢵���λ��
	CMirChoice tMirChoice(point);
	if (!tMirChoice.Initial())
		return;
	CRect rect;
	g_pGameTable->GetClientRect(&rect);
	tMirChoice.AdjustAnchor(point, rect);

	//����ģ̬ѡ���
	byte type_dir = tMirChoice.DoModal();

	//���ê�㴦�о��ӣ���ɾ��
	if (pAnchorMirror)
		pMirPanel->DelMirror(pAnchorMirror->GetGrid());

	//�����ѡ���Ӳ�Ϊ�գ������
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
