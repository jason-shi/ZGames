#include "StdAfx.h"
#include "Access.h"
#include "Trace.h"
#include "SourceZler.h"
#include "ChessBoard.h"
#include "GameTable.h"

using namespace std;

static int			 srcboard[65];

CSourceZler::CSourceZler(CWindow *_window) : CTraceZler(_window)
{
}

CSourceZler::~CSourceZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CSourceZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_SOURCE:
		OnMenuSource();
		break;
	case ID_APP_LIMITARYSOURCE:
		OnMenuSource(true);
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CSourceZler::OnMenuSource(bool bLimitary)
{
	if (!IsBlkLegal())
	{
		window->MessageBox(L"���治�Ϸ�\n�ڷ����ڽ���״̬", L"����", MB_OK);
		return;
	}
	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	g_pChessBoard->ClearFixed();
	g_bLimitary = bLimitary;

	m_nSourceStage = 0;
	m_nSourceRound = 0;
	m_nTraceTimes = 1;

	m_vecSeed.clear();

	//��¼��ʼ����Ϊ��������
	memcpy(m_tBestSeed.board, board, sizeof(board));
	m_tBestSeed.nTraceTimes = 0;

	memcpy(srcboard, board, sizeof(board));
	GetSourceRound();
}

void CSourceZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();

	//�쳣�˳�ʱ�ͷ��ڴ�
	m_vecSeed.clear();

	//��ԭ����
	memcpy(board, srcboard, sizeof(board));
	SendViewInd_InitState();
	SendMenuInd_InitState();
	g_pGameTable->SetZlerBase(NULL);

	window->MessageBox(L"��������ֹ", L"����", MB_ICONWARNING | MB_OK);
}

void CSourceZler::GetSourceRound()
{
	SendViewInd_CrackState();
	SendMenuInd_CrackState();

	//��ʼ���ƽ��̣߳���������ʱ��
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = 0;
	tCrackParam.nFurther = 0;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_SOURCE_GUARD, 600 * 1000);
}

void CSourceZler::ProcessSource(bool bRootSeed)
{
	if (bRootSeed == false)
	{
		vector<TSeed>::iterator head = m_vecSeed.begin();
		memcpy(board, head->board, sizeof(board));
		m_nTraceTimes = head->nTraceTimes + 1;

		m_vecSeed.erase(head);
	}

	//ͨ��Trace����������
	ProcessTrace(m_nSourceRound + m_nTraceTimes - 1);
}

void CSourceZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	if (m_nSourceStage == 0)
	{
		KillAllTimer();

		TCrackResult tCrackResult = WChess_GetCrackResult();
		if (wParam == REDLOSE)
		{
			//����ʱ�۴�
			OnCrackTout();
		}
		else if (tCrackResult.nRound <= 20)
		{
			SendViewInd_InitState();
			SendMenuInd_InitState();
			g_pGameTable->SetZlerBase(NULL);

			window->MessageBox(L"��ʼ����������20�غ�", L"����", MB_ICONWARNING | MB_OK);
			return;
		}
		else
		{
			//��������ļ�
			InitSourceFile(tCrackResult.nRound);

			m_nSourceStage = 1;
			m_nSourceRound = tCrackResult.nRound;
			ProcessSource(true);
		}
		return;
	}

	CTraceZler::OnCrackEnd(wParam, lParam);
}

void CSourceZler::OnCrackTout()
{
	if (m_nSourceStage == 0)
	{
		KillAllTimer();
		WChess_ExitThreadCrack();

		//��ԭ����
		memcpy(board, srcboard, sizeof(board));
		SendViewInd_InitState();
		SendMenuInd_InitState();
		g_pGameTable->SetZlerBase(NULL);

		window->MessageBox(L"��֧�ֻ��ƴ���", L"����", MB_ICONWARNING | MB_OK);
		return;
	}

	CTraceZler::OnCrackTout();
}

void CSourceZler::StoreSeeds()
{
	TSeed tSeed;
	tSeed.nTraceTimes = m_nTraceTimes;
	for (int i = 0; i < TrCount2; i++)
	{
		UpdateBoard(&TrList1[TrList2[i].index]);
		UpdateBoard(&TrList2[i]);

		memcpy(tSeed.board, board, sizeof(board));
		if (IsSeedRepeat(tSeed) == false)
			m_vecSeed.push_back(tSeed);
		//��ԭ����
		RestoreBoard(&TrList2[i]);
		RestoreBoard(&TrList1[TrList2[i].index]);
	}
}

bool CSourceZler::IsSeedRepeat(TSeed &tSeed)
{
	vector<TSeed>::iterator iter;
	for (iter = m_vecSeed.begin(); iter != m_vecSeed.end(); iter++)
	{
		if (memcmp(iter->board, tSeed.board, sizeof(board)) == 0)
			return true;
	}
	return false;
}

void CSourceZler::OutputResult()
{
	StoreSeeds();
	if (TrCount2 != 0)
	{
		if (m_tBestSeed.nTraceTimes < m_nTraceTimes)
		{
			m_nFileIndex = 0;
			memcpy(&m_tBestSeed, &m_vecSeed.back(), sizeof(TSeed));
		}
		if (m_tBestSeed.nTraceTimes == m_nTraceTimes)
		{
			//ֻ������������غ����ľ���
			SaveToFile();
		}
	}
	if (m_vecSeed.empty())
	{
		memcpy(board, m_tBestSeed.board, sizeof(board));

		SendViewInd_InitState();
		SendMenuInd_InitState();
	}
}

void CSourceZler::OnTraceEnd()
{
	if (m_vecSeed.empty())
	{
		if (m_nTraceTimes == 1)
			window->MessageBox(L"����û���·���", L"����", MB_ICONWARNING | MB_OK);

		g_pGameTable->SetZlerBase(NULL);
	}
	else
	{
		ProcessSource(false);
	}
}

void CSourceZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
	window->KillTimer(TM_TRACE_GUARD);
	window->KillTimer(TM_SOURCE_GUARD);
}

void CSourceZler::InitSourceFile(int nRound)
{
	TCHAR szSourceFolder[_MAX_PATH];
	wsprintf(szSourceFolder, L"%s\\Source", ModulePath);
	//����SourceĿ¼
	if (_wchdir(szSourceFolder) != 0)
	{
		_wmkdir(szSourceFolder);
		_wchdir(szSourceFolder);
	}
	//����ļ���
	time_t t = time(NULL);
	wcsftime(m_wcsTimeFile, sizeof(m_wcsTimeFile) / sizeof(TCHAR), L"%Y%m%d_%H%M%S", localtime(&t));
	wsprintf(m_wcsTimeFile, L"%s_R%d.txt", m_wcsTimeFile, nRound); //Eg:20131221_102305_R25.txt
	//��������ļ�
	FILE *fp = _wfopen(m_wcsTimeFile, L"w+, ccs=UTF-8");
	fclose(fp);
}

void CSourceZler::SaveToFile()
{
	FILE *fp = _wfopen(m_wcsTimeFile, L"a+, ccs=UTF-8");
	//д�����ļ�
	wstring wcsContent;
	TCHAR wcsBuff[128];
	for (int i = 0; i < TrCount2; i++)
	{
		wsprintf(wcsBuff, L"%3d_%5d: ", m_nTraceTimes, m_nFileIndex++);
		wcsContent += wcsBuff;

		UpdateBoard(&TrList1[TrList2[i].index]);
		UpdateBoard(&TrList2[i]);
		SaveToFen(wcsBuff);
		wcsContent += wcsBuff;
		wcsContent += L"w - - 0 1\n";
		//��ԭ����
		RestoreBoard(&TrList2[i]);
		RestoreBoard(&TrList1[TrList2[i].index]);
	}
	fputws(wcsContent.c_str(), fp);
	fclose(fp);
}
