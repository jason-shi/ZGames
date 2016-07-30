#ifndef __LightImage_h__
#define __LightImage_h__

class CLightImage : public CImage {
  public:
	CLightImage();
	CLightImage(int nWidth, int nHeight);
	virtual ~CLightImage();

  public:
	void			 FastLineH(int x1, int x2, int y, COLORREF color);
	void			 FastLineL(int x1, int y1, int x2, int y2, COLORREF color);
	void			 FastLineV(int x, int y1, int y2, COLORREF color);
	void			 FastLineB(int x1, int y1, int x2, int y2, COLORREF color);
};

#endif
