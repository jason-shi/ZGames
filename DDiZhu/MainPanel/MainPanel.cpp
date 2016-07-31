#include "StdAfx.h"
#include "MainPanel.h"
#include "DDiZhuBoard\DDiZhuBoard.h"
#include "DDiZhuTest\AutoTest.h"
#include "DDiZhuTest\CompTest.h"
#include "DDiZhuTest\ReplayTest.h"
#include <boost\bind.hpp>

CMainPanel::CMainPanel(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSize, nDepth)
{
	m_pDDiZhuBoard = NULL;
	m_pAutoTest = NULL;
	m_pCompTest = NULL;
	m_pReplayTest = NULL;
}

CMainPanel::~CMainPanel()
{
	delete m_pDDiZhuBoard;
	m_pDDiZhuBoard = NULL;

	delete m_pAutoTest;
	m_pAutoTest = NULL;

	delete m_pCompTest;
	m_pCompTest = NULL;

	delete m_pReplayTest;
	m_pReplayTest = NULL;
}

bool CMainPanel::Initial()
{
	if (!CPanel::Initial())
		return false;

	if (!LoadImage())
		return false;

	//创建牌盘
	m_pDDiZhuBoard = new CDDiZhuBoard(NULL, CPoint(0, 0), CPoint(0, 0), CSize(840, 148), 0);
	if (!m_pDDiZhuBoard || !m_pDDiZhuBoard->Initial())
		return false;
	g_pZ2coo->AddObject(m_pDDiZhuBoard);

	m_pAutoTest = new CAutoTest(m_pDDiZhuBoard);
	if (!m_pAutoTest || !m_pAutoTest->Initial())
		return false;

	m_pCompTest = new CCompTest(m_pDDiZhuBoard);
	if (!m_pCompTest || !m_pCompTest->Initial())
		return false;

	m_pReplayTest = new CReplayTest(m_pDDiZhuBoard);
	if (!m_pReplayTest || !m_pReplayTest->Initial())
		return false;

	return true;
}

void CMainPanel::OnStartGame()
{
	m_pDDiZhuBoard->OnStartGame();
	m_pDDiZhuBoard->ShufflePoke();
}

void CMainPanel::OnKeySave()
{
}

void CMainPanel::OnRecoverPoke()
{
	m_pDDiZhuBoard->OnStartGame();
	m_pDDiZhuBoard->RecoverPoke();
}

void CMainPanel::OnKeyUp()
{
	m_pCompTest->OnTestBegin();
}

void CMainPanel::OnKeyDown()
{
	static OPENFILENAME ofn;
	static TCHAR wcsFileName[MAX_PATH], wcsTitleName[MAX_PATH];
	static TCHAR wcsFilter[] = TEXT(".dat\0*.dat\0");
	//打开.dat文件
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.Flags = OFN_READONLY;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = wcsFilter;
	ofn.lpstrFile = wcsFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = wcsTitleName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = TEXT("dat");
	if (!GetOpenFileName(&ofn))
		return;
	if (_waccess(wcsFileName, 0) == -1)
	{
		g_pZ2coo->GetWindow()->MessageBox(L"文件不存在", L"打开", MB_ICONWARNING | MB_OK);
		return;
	}
	if (!m_pReplayTest->OpenReplay(wcsFileName))
		return;

	g_pZTimer->AddTimer(TM_REPLAY, 100, boost::bind(&CReplayTest::OnTimer, m_pReplayTest, _1), NULL);
}

void CMainPanel::OnKeyLeft()
{
	g_pZTimer->AddTimer(TM_AUTO_TEST, 100, boost::bind(&CAutoTest::OnTimer, m_pAutoTest, _1), NULL);
}

void CMainPanel::OnKeyRight()
{
	m_pAutoTest->OnTestBegin();
}
