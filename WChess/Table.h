#ifndef __CTable_h__
#define __CTable_h__

extern int			 TblPieceBitmap[64]; //���ӺϷ�λͼ
extern byte			 TblCharToPiece[256]; //�ַ�ת��Ϊ��������
extern char			 TblPieceToChar[15]; //��������ת��Ϊ�ַ�
extern int			 TblExclusion[32]; //��Ӧλ���ų���

void				 CreateAllTable();

#endif
