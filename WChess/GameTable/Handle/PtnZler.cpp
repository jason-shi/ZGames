#include "StdAfx.h"
#include "Pattern.h"
#include "PtnZler.h"
#include "GameTable.h"

static int			 ptnboard[65];

extern void			 SaveToFen(TCHAR *fen);
extern bool			 SaveToPgn(const TCHAR* filename);

CPtnZler::CPtnZler(CWindow *_window) : CZlerBase()
{
	window = _window;
	g_pCmdManage->AddCmdZler(this);
}

CPtnZler::~CPtnZler()
{
	g_pCmdManage->DelCmdZler(this);
}

bool CPtnZler::ProcessCmd(int nType)
{
	switch (nType)
	{
	case ID_APP_PATTERN:
		OnMenuPattern();
		break;
	case ID_APP_EXIT:
		OnMenuExit();
		break;
	default:
		return false;
	}
	return true;
}

void CPtnZler::OnMenuPattern()
{
	if (!IsBoardLegal())
	{
		window->MessageBox(L"盘面不合法", L"定形局", MB_ICONWARNING | MB_OK);
		return;
	}
	if (window->MessageBox(L"确定以当前盘面进行创作？", L"定形局", MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
		return;

	g_pGameTable->SetZlerBase(this);
	g_pGameTable->SetAction(0);

	//进入Pattern目录
	TCHAR wcsPatternPath[_MAX_PATH];
	wsprintf(wcsPatternPath, L"%s\\Pattern", ModulePath);
	if (_waccess(wcsPatternPath, 0) == -1)
		_wmkdir(wcsPatternPath);
	_wchdir(wcsPatternPath);
	//创建文件夹
	time_t t = time(NULL);
	wcsftime(m_wcsTimeFolder, sizeof(m_wcsTimeFolder) / sizeof(TCHAR), L"%Y%m%d_%H%M%S", localtime(&t)); //Eg: 20130814_210517
	_wmkdir(m_wcsTimeFolder);
	//设置起始文件号
	m_nFileNumber = 1;
	if (!SavePtnTitle(wcsPatternPath))
		return;

	InitSwapRemain();
	ProcessPattern(true);
}

void CPtnZler::OnMenuExit()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	//征询是否全部退出
	if (window->MessageBox(L"是否全部退出？", L"定形局", MB_OKCANCEL) == IDCANCEL)
	{
		memcpy(board, ptnboard, sizeof(board));
		ProcessPattern(true);
	}
	else
	{
		//全部退出
		SendViewInd_InitState();
		SendMenuInd_InitState();
		g_pGameTable->SetZlerBase(NULL);
	}
}

void CPtnZler::ProcessPattern(bool bWeaken)
{
	ChangeOffensive(bWeaken);
	memcpy(ptnboard, board, sizeof(board));

	//开始破解
	SendViewInd_PtnState();
	SendMenuInd_PtnState();

	//初始化破解线程，并启动定时器
	TCrackParam tCrackParam;
	tCrackParam.board = board;
	tCrackParam.bPesudo = false;
	tCrackParam.nMaxDepth = PTN_MAXDEPTH;
	tCrackParam.nFurther = 0;
	tCrackParam.src = 0;
	tCrackParam.dest = 0;
	WChess_InitThreadRFCrack(&tCrackParam);

	window->SetTimer(TM_UPDATE_VIEW, 1000);
	window->SetTimer(TM_PATTERN_GUARD, 600 * 1000); //600秒
}

void CPtnZler::OnCrackEnd(WPARAM wParam, LPARAM lParam)
{
	KillAllTimer();

	TCrackResult tCrackResult = WChess_GetCrackResult();
	if (wParam == BLKLOSE)
	{
		//记录结果并保存文件
		if (tCrackResult.nRound >= PTN_MINROUND)
		{
			SaveToLog(wParam, tCrackResult.nRound);
			SaveToFile();
		}
		//弱化攻势
		ProcessPattern(true);
	}
	else
	{
		//弱化守势
		ProcessPattern(false);
	}
}

void CPtnZler::OnCrackTout()
{
	KillAllTimer();
	WChess_ExitThreadCrack();
	//还原盘面
	memcpy(board, ptnboard, sizeof(board));
	//保存结果
	SaveToLog(TIMEOUT, 0);
	SaveToFile();
	//按红强进行调整子力
	ProcessPattern(true);
}

void CPtnZler::KillAllTimer()
{
	window->KillTimer(TM_UPDATE_VIEW);
	window->KillTimer(TM_PATTERN_GUARD);
}

bool CPtnZler::SavePtnTitle(TCHAR *wcsPatternPath)
{
	//打开日志文件
	TCHAR wcsLogFile[_MAX_PATH];
	wsprintf(wcsLogFile, L"%s\\Log.txt", wcsPatternPath);
	FILE *fp = _wfopen(wcsLogFile, L"a+, ccs=UTF-8");
	if (fp == NULL)
	{
		window->MessageBox(L"打开日志文件失败", L"定形局", MB_ICONWARNING | MB_OK);
		return false;
	}
	//写入分隔符和标题
	fputws(L"\n-- - ---- -----  -- - -- - -  -- -- --  - - -- - --  ----- ---- - --\n", fp);
	TCHAR wcsFolderInfo[256];
	wsprintf(wcsFolderInfo, L"Folder:  %s\n", m_wcsTimeFolder);
	fputws(wcsFolderInfo, fp);
	//写入起始局面
	TCHAR wcsBuff[1024];
	SaveToFen(wcsBuff);
	fputws(wcsBuff, fp);
	fputws(L"r - - 0 1\n", fp);
	fclose(fp);
	return true;
}

void CPtnZler::SaveToLog(int nResult, int nRound)
{
	//打开文件
	TCHAR wcsLogFile[_MAX_PATH];
	wsprintf(wcsLogFile, L"%s\\Pattern\\Log.txt", ModulePath);
	FILE *LogFile;
	if ((LogFile = _wfopen(wcsLogFile, L"a+, ccs=UTF-8")) == NULL)
		return;

	//写入结果
	TCHAR wcsResult[128], szIdent[7];
	switch (nResult)
	{
	case BLKLOSE:
		wsprintf(szIdent, L"%s", nRound >= 30 ? L"      " : nRound >= 25 ? L"   " : L"");
		wsprintf(wcsResult, L"%6d.pgn         %s%3d\n", m_nFileNumber, szIdent, nRound);
		break;
	case TIMEOUT:
		wsprintf(wcsResult, L"%6d.pgn         超时\n", m_nFileNumber);
		break;
	}
	fputws(wcsResult, LogFile);

	//关闭文件
	fclose(LogFile);
}

void CPtnZler::SaveToFile()
{
	static TCHAR wcsFileName[_MAX_PATH];
	//写入序号文件
	wsprintf(wcsFileName, L"%s\\%d.pgn", m_wcsTimeFolder, m_nFileNumber++);
	SaveToPgn(wcsFileName);
}

void CPtnZler::SendViewInd_InitState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CPtnZler::SendViewInd_PtnState()
{
	memset(&g_tViewInd, 0, sizeof(g_tViewInd));
	g_tViewInd.bSyncBoard = true;
	//标签显示
	g_tViewInd.tLabelInd.nLabelBitmap = SH_THINKDEPTH | SH_ASTODESIZE | SH_SPENDTIME;

	window->SendMessage(WM_VIEW_IND, false, (LPARAM)&g_tViewInd);
}

void CPtnZler::SendMenuInd_InitState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_OPEN | EN_MENU_RUN | EN_MENU_ARRANGE | EN_MENU_SAVE | EN_MENU_BATCH | EN_MENU_EXIT;
	g_tMenuInd.nMenuBitmap[1] = EN_MENU_COPY | EN_MENU_PASTE | EN_MENU_SHORTKEY;
	g_tMenuInd.nMenuBitmap[2] = EN_MENU_BUILD;
	g_tMenuInd.nMenuBitmap[3] = EN_MENU_REGISTER | EN_MENU_EXPLAINATION;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}

void CPtnZler::SendMenuInd_PtnState()
{
	memset(&g_tMenuInd, 0, sizeof(g_tMenuInd));
	g_tMenuInd.nMenuBitmap[0] = EN_MENU_EXIT;

	window->SendMessage(WM_MENU_IND, false, (LPARAM)&g_tMenuInd);
}
