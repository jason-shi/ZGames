/*****************************************************************************
 *                                  光线算法                                
 * 76543210 八个位
 * 210 3个位代表光的颜色红绿蓝，如110则表示黄色，001则表示蓝色
 * 543 3个位代表光的方向：12点钟方向为0；3点钟方向为2；6点钟方向为4；类推
 * 采用光线跟踪算法。
 * 每道光线单独处理，直到光线终止或者同性光线。
 * 任何一种镜子在某个方向，接受8*8=64种可能的光线，并且输出为[0-n]的光线。
 * 为每个镜子构建数组 T[n][8][64]，即每个镜子需要 n * 512 个元素
 *
 * 例：垂直反射镜输出光线数为1，其对应的数组为 Tchui[8][64]
 *     接收的入射光的标志为 nFlag，镜子的方向为 nDir
 *     可以查表 Tchui[nDir][nFlag] 即为出射光的标志
 *     根据出射光的标志可以查表获得格子的相对偏移，例如+16（右上）
 *     垂直反光镜所在的位置nPos + 16即可得到出射光的位置
 *
 *****************************************************************************/
#ifndef __Light_h__
#define __Light_h__

class CLightImage;
class CLight : public CSprite {
  public:
	CLight();
	virtual ~CLight();

  public:
	bool			 Initial();
	void			 DrawLight(int nBmpLight[256]);
	void			 DrawLightSpan(int dir, int x1, int y1, int x2, int y2, int color);

  protected:
	CLightImage		*m_pImageLight;
};

#endif
