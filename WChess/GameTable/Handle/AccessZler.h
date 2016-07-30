#ifndef __AccessZler_h__
#define __AccessZler_h__

class CAccessZler : public ICmdZler {
  public:
	CAccessZler(CWindow *_window);
	virtual ~CAccessZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 SaveBoardToKey(int key); //盘面快捷保存
	void			 LoadBoardFromKey(int key); //盘面快捷加载
	void			 OnMenuOpen();
	void			 OnMenuSave();
	void			 OnMenuCopy();
	void			 OnMenuPaste();
	void			 OnLoadSuccess();
	void			 SendViewInd_LoadState();
	void			 SendMenuInd_InitState();

  private:
	CWindow			*window;
	TCHAR			 KeyFen[4][128]; //快捷盘
	bool			 m_bAccessBusy; //访问忙
};

#endif
