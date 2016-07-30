#include "StdAfx.h"
#include "Table.h"

#define FOREACH_DIR_BEGIN(turns) \
	for (int dir = 0; dir < (turns); dir++) \
	{ \
		for (int in = 0; in < 8; in++) \
		{

#define FOREACH_DIR_END \
		} \
	}

byte				 TblMirror[256 * 64]; //TblMirror[����+����][���+��ɫ] = ����+��ɫ
byte				 TblNextGrid[256 * 8]; //TblNextGrid[���][����] = ��һ�����

bool				 TblSpecMirror[MT_MIRROR_COUNT]; //��Ҫ���⴦��ľ���
bool				 TblQuantumEnd[MT_MIRROR_COUNT]; //ʹ����������ֹ�ľ���

byte				 TblSplitter[4 * 64]; //TblSplitter[����][���+��ɫ]
byte				 TblTangler[8 * 64]; //���Ӿ�����
byte				 TblPrismG[8 * 64]; //��ɫ����
byte				 TblPrismB[8 * 64]; //��ɫ����
byte				 TblStarburst1[64]; //�Ǳ����ӷ���1
byte				 TblStarburst2[64]; //�Ǳ����ӷ���2
byte				 TblStarburst3[64]; //�Ǳ����ӷ���3
int					 TblLightMask[64]; //TblLightMask[����+��ɫ] = 24λ����(rgb rgb ... rgb)

char				 TblMirToAlpha[256]; //��������ת������ĸ
byte				 TblAlphaToMir[256]; //��ĸת���ɾ�������

void				 CreateTables(); //�㷨�����뷽��=>�����=>�����=>�������


void CreateNothing()
{
	for (int in = 0; in < 8; in++)
	{
		for (int color = 1; color < 8; color++)
		{
			TblMirror[MAKE_INDEX(MT_NOTHING,    0, in, color)] = MAKE_BEAM(in, color);
			TblMirror[MAKE_INDEX(MT_TELEPORTER, 0, in, color)] = MAKE_BEAM(in, color);
			TblMirror[MAKE_INDEX(MT_OCCUPIED,   0, in, color)] = MAKE_BEAM(in, color);

			for (int dir = 0; dir < 8; dir++)
			{
				TblMirror[MAKE_INDEX(MT_TARGET,     dir, in, color)] = MAKE_BEAM(in, color);
				TblMirror[MAKE_INDEX(MT_HIT_TARGET, dir, in, color)] = MAKE_BEAM(in, color);
			}
		}
	}
}

void _CreateRefraction(int nType, int turns, int TOut[8])
{
	FOREACH_DIR_BEGIN(turns)
		int angle_out = TOut[ANGLE_IN];
		if (angle_out != -1)
		{
			int out = DIR(dir + angle_out + 4);
			//90�ȷ��䣬��ֱ����
			for (int color = 1; color < 8; color++)
				TblMirror[MAKE_INDEX(nType, dir, in, color)] = MAKE_BEAM(out, color);
		}
	FOREACH_DIR_END
}

void CreateReflector()
{
	int TOut[8] = { 0, 7, -1, -1, -1, -1, -1, 1 };
	_CreateRefraction(MT_REFLECTOR, 8, TOut);
}

void CreateBender()
{
	int TOut[8] = { 1, 0, 7, -1, -1, -1, -1, 2 };
	_CreateRefraction(MT_BENDER, 8, TOut);
}

void CreateQuadReflector()
{
	int TOut[8] = { 0, 7, -1, 5, 4, 3, -1, 1 };
	_CreateRefraction(MT_QUAD_REFL, 8, TOut);
}

void CreateQuadBender()
{
	int TOut[8] = { 1, 0, 7, 6, 5, 4, 3, 2 };
	_CreateRefraction(MT_QUAD_BEND, 4, TOut);
}

void CreateSplitter()
{
	int TOut[8] = { 4, 3, -1, 1, 0, 7, -1, 5 };

	FOREACH_DIR_BEGIN(4)
		int angle_out = TOut[ANGLE_IN];
		if (angle_out != -1)
		{
			int out = DIR(dir + angle_out);
			for (int color = 1; color < 8; color++)
			{
				//͸��ʹ�ֱ�ֹ�
				TblMirror[MAKE_INDEX(MT_SPLITTER, dir, in, color)] = MAKE_BEAM(in, color);
				TblSplitter[(dir << 6) + MAKE_BEAM(in, color)] = MAKE_BEAM(out, color);
			}
		}
	FOREACH_DIR_END
}

void _FilterCreator(int nType, int TColor[8])
{
	FOREACH_DIR_BEGIN(4)
		//ֻ�з��߷��� �� 180�ȷ���ͨ��
		if (in != dir && in != DIR(dir + 4))
			continue;

		for (int color = 1; color < 8; color++)
		{
			if (TColor[color] == 0)
				continue;
			//������������ͬ����ɫ�任
			TblMirror[MAKE_INDEX(nType, dir, in, color)] = MAKE_BEAM(in, TColor[color]);
		}
	FOREACH_DIR_END
}

void CreateFilterR()
{
	int TColor[8] = { 0, 0, 0, 0, 4, 4, 4, 4 };
	_FilterCreator(MT_FILTER_R, TColor);
}

void CreateFilterG()
{
	int TColor[8] = { 0, 0, 2, 2, 0, 0, 2, 2 };
	_FilterCreator(MT_FILTER_G, TColor);
}

void CreateFilterB()
{
	int TColor[8] = { 0, 1, 0, 1, 0, 1, 0, 1 };
	_FilterCreator(MT_FILTER_B, TColor);
}

void CreateConduit()
{
	int TColor[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	_FilterCreator(MT_CONDUIT, TColor);
}

void _CreatePrismR()
{
	//��ɫ������͸
	FOREACH_DIR_BEGIN(8)
		if (in == DIR(dir + 3) || in == DIR(dir + 4) || in == DIR(dir + 7) || in == dir)
		{
			for (int color = 1; color < 8; color++)
			{
				if (color & 4)
					TblMirror[MAKE_INDEX(MT_PRISM, dir, in, color)] = MAKE_BEAM(in, color & 4);
			}
		}
	FOREACH_DIR_END
}

void _CreatePrismG()
{
	//��ɫ����135������
	FOREACH_DIR_BEGIN(8)
		int out;
		if (in == dir)
			out = DIR(dir + 1);
		else if (in == DIR(dir + 6))
			out = DIR(dir + 7);
		else if (in == DIR(dir + 5))
			out = DIR(dir + 4);
		else if (in == DIR(dir + 3))
			out = DIR(dir + 2);
		else
			continue;
		for (int color = 1; color < 8; color++)
		{
			if (color & 2)
				TblPrismG[(dir << 6) + MAKE_BEAM(in, color)] = MAKE_BEAM(out, color & 2);
		}
	FOREACH_DIR_END
}

void _CreatePrismB()
{
	//��ɫ����90������
	FOREACH_DIR_BEGIN(8)
		int out;
		if (in == dir)
			out = DIR(dir + 2);
		else if (in == DIR(dir + 6))
			out = DIR(dir + 4);
		else if (in == DIR(dir + 5))
			out = DIR(dir + 7);
		else if (in == DIR(dir + 3))
			out = DIR(dir + 1);
		else
			continue;
		for (int color = 1; color < 8; color++)
		{
			if (color & 1)
				TblPrismB[(dir << 6) + MAKE_BEAM(in, color)] = MAKE_BEAM(out, color & 1);
		}
	FOREACH_DIR_END
}

void CreatePrism()
{
	_CreatePrismR();
	_CreatePrismG();
	_CreatePrismB();
}

void CreateDoppler()
{
	int T_R2G[8] = { 0, 4, 1, 5, 2, 6, 3, 7 }; //��-��ת��
	int T_R2B[8] = { 0, 2, 4, 6, 1, 3, 5, 7 }; //��-��ת��

	FOREACH_DIR_BEGIN(8)
		if (in == DIR(dir + 4)) //���߷���
		{
			for (int color = 1; color < 8; color++)
				TblMirror[MAKE_INDEX(MT_DOPPLER, dir, in, color)] = MAKE_BEAM(in, T_R2G[color]);
		}
		if (in == dir) //+180�ȷ���
		{
			for (int color = 1; color < 8; color++)
				TblMirror[MAKE_INDEX(MT_DOPPLER, dir, in, color)] = MAKE_BEAM(in, T_R2B[color]);
		}
	FOREACH_DIR_END
}

void CreateTangler()
{
	FOREACH_DIR_BEGIN(8)
		if (in != dir) //���߷���
			continue;

		int out1 = DIR(dir + 2);
		int out2 = DIR(dir + 6);
		for (int color = 1; color < 8; color++)
		{
			TblMirror[MAKE_INDEX(MT_TANGLER, dir, in, color)] = MAKE_BEAM(out1, color);
			TblTangler[(dir << 6) + MAKE_BEAM(in, color)] = MAKE_BEAM(out2, color);
		}
	FOREACH_DIR_END
}

void CreateStarburst()
{
	for (int in = 0; in < 8; in++)
	{
		int out = DIR(in + 1);
		int out1 = DIR(in + 3);
		int out2 = DIR(in + 5);
		int out3 = DIR(in + 7);
		for (int color = 1; color < 8; color++)
		{
			TblMirror[MAKE_INDEX(MT_STARBURST, 0, in, color)] = MAKE_BEAM(out, color);
			TblStarburst1[MAKE_BEAM(in, color)] = MAKE_BEAM(out1, color);
			TblStarburst2[MAKE_BEAM(in, color)] = MAKE_BEAM(out2, color);
			TblStarburst3[MAKE_BEAM(in, color)] = MAKE_BEAM(out3, color);
		}
	}
}

void CreateMulFilter()
{
	int TMask[8] = { 4, 7, 1, 2, 4, 7, 1, 2 };

	//��ͬ������в�ͬ����ɫ����
	FOREACH_DIR_BEGIN(4)
		for (int color = 1; color < 8; color++)
		{
			if (color & TMask[ANGLE_IN])
				TblMirror[MAKE_INDEX(MT_MUL_FILTER, dir, in, color)] = MAKE_BEAM(in, color & TMask[ANGLE_IN]);
		}
	FOREACH_DIR_END
}

void CreateTwister()
{
	for (int in = 0; in < 8; in++)
	{
		int out = DIR(in + 6);
		for (int color = 1; color < 8; color++)
			TblMirror[MAKE_INDEX(MT_TWISTER, 0, in, color)] = MAKE_BEAM(out, color);
	}
}

void CreateComplementor()
{
	int TColor[8] = { 0, 6, 5, 7, 3, 7, 7, 7 };

	FOREACH_DIR_BEGIN(2)
		if ((ANGLE_IN & 1) == 0)
		{
			for (int color = 1; color < 8; color++)
				TblMirror[MAKE_INDEX(MT_COMPLEMENTOR, dir, in, color)] = MAKE_BEAM(in, TColor[color]);

		}
	FOREACH_DIR_END
}

void CreateNextGrid()
{
	int TOffset[8] = { 16, 15, -1, -17, -16, -15, 1, 17 };
	for (int nGrid = 0; nGrid < 256; nGrid++)
	{
		for (int dir = 0; dir < 8; dir++)
		{
			int next = nGrid + TOffset[dir];
			if (next < 0 || next > 255 || (next & 15) == 15 || (next >> 4) == 15)
				next = 255; //����
			TblNextGrid[(nGrid << 3) + dir] = next;
		}
	}
}

void CreateSpecMirror()
{
	memset(TblSpecMirror, 0, sizeof(TblSpecMirror));
	TblSpecMirror[MT_TELEPORTER] = true;
	TblSpecMirror[MT_PRISM] = true;
	TblSpecMirror[MT_SPLITTER] = true;
	TblSpecMirror[MT_TANGLER] = true;
	TblSpecMirror[MT_STARBURST] = true;
}

void CreateTanglerEnd()
{
	memset(TblQuantumEnd, 0, sizeof(TblQuantumEnd));
	TblQuantumEnd[MT_SPLITTER] = true;
	TblQuantumEnd[MT_TANGLER] = true;
	TblQuantumEnd[MT_STARBURST] = true;
	TblQuantumEnd[MT_COMPLEMENTOR] = true;
}

void CreateLightMask()
{
	for (int dir = 0; dir < 8; dir++)
	{
		for (int color = 0; color < 8; color++)
			TblLightMask[MAKE_BEAM(dir, color)] = color << (dir * 3);
	}
}

void CreateMirToAlpha()
{
	memset(TblMirToAlpha, 0, sizeof(TblMirToAlpha));
	for (int i = 0; i < 26; i++)
		TblMirToAlpha[i] = i + 'A'; //A...Z ��26��
	for (int i = 26; i < 46; i++)
		TblMirToAlpha[i] = i - 26 + 'g'; //g...z ��20��
}

void CreateAlphaToMir()
{
	memset(TblAlphaToMir, 0, sizeof(TblAlphaToMir));
	for (char ch = 'A'; ch <= 'Z'; ch++)
		TblAlphaToMir[ch] = ch - 'A';
	for (char ch = 'g'; ch <= 'z'; ch++)
		TblAlphaToMir[ch] = ch - 'g' + 26;
}

void CreateTables()
{
	CreateNothing();
	CreateReflector();
	CreateBender();
	CreateSplitter();
	CreateQuadReflector();
	CreateQuadBender();
	CreateFilterR();
	CreateFilterG();
	CreateFilterB();
	CreateConduit();
	CreatePrism();
	CreateDoppler();
	CreateTangler();
	CreateStarburst();
	CreateMulFilter();
	CreateTwister();
	CreateComplementor();

	CreateNextGrid();
	CreateSpecMirror();
	CreateTanglerEnd();

	CreateMirToAlpha();
	CreateAlphaToMir();

	CreateLightMask();
}
