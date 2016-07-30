#ifndef __AccessZler_h__
#define __AccessZler_h__

class CAccessZler : public ICmdZler {
  public:
	CAccessZler(CWindow *_window);
	virtual ~CAccessZler();

  public:
	bool			 ProcessCmd(int nType);
	void			 SaveBoardToKey(int key); //�����ݱ���
	void			 LoadBoardFromKey(int key); //�����ݼ���
	void			 OnMenuOpen();
	void			 OnMenuSave();
	void			 OnMenuCopy();
	void			 OnMenuPaste();
	void			 OnLoadSuccess();
	void			 SendViewInd_LoadState();
	void			 SendMenuInd_InitState();

  private:
	CWindow			*window;
	TCHAR			 KeyFen[4][128]; //�����
	bool			 m_bAccessBusy; //����æ
};

#endif
