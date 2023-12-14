/********************************************************************/
/*                           anim_proc                              */
/*                           =========                              */
/*    ������� ��� ��������  ������������� ������ ������� ��������   */
/*                                                                  */
/********************************************************************/
#include "tdef.cpp"

#include "resource.h"

#define ALL_STEP       200        // �����  ���������  ����������
#define X_SPACE_ANIM   100.0      // ������ ������� ������ �� X
#define Y_SPACE_ANIM    70.0      // ������ ������� ������ �� Y
#define DX X_SPACE_ANIM/ALL_STEP  // ��� �������� �� X
#define DY Y_SPACE_ANIM/ALL_STEP  // ��� �������� �� Y

#define  K_SCALE  0.1           // ����������� ������������������

#define  K_SC     X_SPACE_ANIM * K_SCALE  // ���������� �����.


#define  K_GAB    0.1           // ����������  ����������� �������
								//  ���������� ����������� �� ���� ������� ������

#define  PI       3.14159265
HDC hdc;

HBRUSH 	hbr[] = { CreateSolidBrush(RGB(255, 0, 0)),
				  CreateSolidBrush(RGB(0, 255, 0)),
				  CreateSolidBrush(RGB(0, 0, 255)),
				  CreateSolidBrush(RGB(255, 255, 255)),
				  CreateSolidBrush(RGB(255, 205, 72)),
				  CreateSolidBrush(RGB(127, 199, 255)),
	CreateSolidBrush(RGB(193, 68, 14)),
	CreateSolidBrush(RGB(173, 173, 173)),
	CreateSolidBrush(RGB(227, 187, 118))
},
hbr_prv;


int pr_start = 0, all_br = sizeof(hbr) / sizeof(hbr[0]);

CMatr  mpr;

/********************************************************************/
/*                        anim_main                                 */
/*                        ==========                                */
/*      �����������  ���������  ���������  �����  ��������         */
/********************************************************************/

CVect  sun[ALL_PT] =
{ {  0.5,   0.5  },
	{ -0.5,   0.5 },
	{ -0.5,  -0.5 },
	{  0.5,  -0.5 }
};
CVect  mercury[ALL_PT] =
{ {  0.1,   0.1  },
	{ -0.1,   0.1 },
	{ -0.1,  -0.1 },
	{  0.1,  -0.1 }
};
CVect venera[ALL_PT] =
{ {  0.25,   0.25  },
	{ -0.25,   0.25 },
	{ -0.25,  -0.25},
	{  0.25,  -0.25 }
};
CVect mars[ALL_PT] =
{ {  0.15,   0.15  },
	{ -0.15,   0.15 },
	{ -0.15,  -0.15 },
	{  0.15,  -0.15 }
};
CVect  earth[ALL_PT] =
{ {  0.3,   0.3  },
	{ -0.3,   0.3 },
	{ -0.3,  -0.3 },
	{  0.3,  -0.3 }
};



GO_SHAPE Sun(sun, 4, 0, 0, 0, K_SC, K_SC,
	X_SPACE_ANIM / 2, Y_SPACE_ANIM / 2, SUN, X_SPACE_ANIM),
	Mercury(mercury, 4, DX, DY, 0, K_SC, K_SC,
		X_SPACE_ANIM, 0, MERK, X_SPACE_ANIM),
	Venera(venera, 4, DX, DY, 0, K_SC, K_SC,
		0, Y_SPACE_ANIM / 2, VEN, X_SPACE_ANIM),
	Mars(mars, 4, DX, DY, 0, K_SC, K_SC,
		X_SPACE_ANIM, 0, MARS, X_SPACE_ANIM),
	Earth(earth, 4, DX, DY, 0, K_SC, K_SC,
		X_SPACE_ANIM, 0, EARTH, X_SPACE_ANIM);


BOOL  anim_main(HWND hwnd)
{
	int    i;
	double dx, dy, ang;

	extern int timer;

	CVect vt;


	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	if (pr_start == 0)
	{
		pr_start = 1;
		EndPaint(hwnd, &ps);
		return TRUE;
	}

	// ���������� ������� ���������� ������� ����
	if (timer == 0)
	{
		RECT rc;
		GetClientRect(hwnd, &rc);

		mpr = make_matr_pr(rc);
	}

	/*---------------------------------------*/
	/*      ������� �����������              */
	/*---------------------------------------*/
	
	
	Sun.move();

	double dk = double(timer + 1300) / (ALL_STEP / 6);
	Mercury.vt.x = X_SPACE_ANIM / 2 + 10 * cos(dk);
	Mercury.vt.y = Y_SPACE_ANIM / 2 + 10 * sin(dk);
	Mercury.move();
	TextOut(hdc, Mercury.kvadro_win[0].x, Mercury.kvadro_win[0].y, "��������", 8);
	dk = double(timer + 1000) / (ALL_STEP / 4);
	Venera.vt.x = X_SPACE_ANIM / 2 + 20 * cos(dk);
	Venera.vt.y = Y_SPACE_ANIM / 2 + 20 * sin(dk);
	Venera.move();
	TextOut(hdc, Venera.kvadro_win[0].x, Venera.kvadro_win[0].y, "������", 6);
	dk = double(timer + 3000) / (ALL_STEP / 2);
	Mars.vt.x = X_SPACE_ANIM / 2 + 40 * cos(dk);
	Mars.vt.y = Y_SPACE_ANIM / 2 + 40 * sin(dk);
	Mars.move();
	TextOut(hdc, Mars.kvadro_win[0].x, Mars.kvadro_win[0].y, "����", 4);
	dk = double(timer) / (ALL_STEP);
	Earth.vt.x = X_SPACE_ANIM / 2 + 30 * cos(dk);
	Earth.vt.y = Y_SPACE_ANIM / 2 + 30 * sin(dk);
	Earth.move();
	TextOut(hdc, Earth. kvadro_win[0].x, Earth.kvadro_win[0].y, "�����", 5);




	/*---------------------------------------*/


	EndPaint(hwnd, &ps);

	return TRUE;
}

/********************************************************************/
/*                        draw_polygon                              */
/*                        ============                              */
/*      ��������� ��������������  �� ������ ��  ������������ �����  */
/********************************************************************/
void draw_polygon(HDC hDc, CVect* pVect, int nPoints)
{
	POINT* pPnt = new POINT[nPoints];
	for (int i = 0; i < nPoints; i++) {
		pPnt[i].x = LONG(pVect[i].x);
		pPnt[i].y = LONG(pVect[i].y);
	}
	Polygon(hDc, pPnt, nPoints);
	delete[] pPnt;
}



/********************************************************************/
/*                        make_matr_pr                              */
/*                        ============                              */
/*              ������������  �������  ��������                     */
/********************************************************************/
CMatr  make_matr_pr(RECT& cp)
{
	int a, b, gab_a, gab_b, xc_w, yc_w;
	double x_max, y_max, x_min, y_min, k_scale, k1, k2,
		dx, dy, xc, yc, l_snar, l_pushka;

	CMatr  mt1, ms, mr, mt2;

	CVect  vt;


	/*--------------------------------*/
	/*  ���������  ����  ������       */
	/*--------------------------------*/
	a = abs(cp.right - cp.left);
	b = abs(cp.top - cp.bottom);

	gab_a = (int)((double)a * K_GAB);  // ������ �� ������
	gab_b = (int)((double)b * K_GAB);  // ������ �� ������

	a -= gab_a;
	b -= gab_b;

	xc_w = (cp.right + cp.left) / 2;
	yc_w = (cp.top + cp.bottom) / 2;

	/*-----------------------------------------*/
	/*  �����������  ����������� ������������  */
	/*-----------------------------------------*/
	x_min = 0;
	y_min = 0;

	x_max = X_SPACE_ANIM;
	y_max = Y_SPACE_ANIM;

	dx = fabs(x_max - x_min);
	dy = fabs(y_max - y_min);

	xc = (x_max + x_min) / 2;
	yc = (y_max + y_min) / 2;

	k1 = a / dx;
	k2 = b / dy;

	k_scale = (k1 < k2) ? k1 : k2;


	vt.x = -xc;
	vt.y = -yc;
	MatrTransl(mt1, vt);
	MatrScale(ms, k_scale, k_scale);
	// ���������� �����������
	MatrScale(mr, 1, -1);

	vt.x = xc_w;
	vt.y = yc_w;
	MatrTransl(mt2, vt);

	return (mt1 * ms * mr * mt2);
}

/********************************************************************/
/*                           trans_coor                             */
/*                           ==========                             */
/*  ��������� ������� ���������  ��  �������  ��������������        */
/********************************************************************/

void trans_coor(CVect* pvOld, CVect* pvNew, int all_pt, CMatr& mpr)
{
	for (int i = 0; i < all_pt; i++)
		pvNew[i] = pvOld[i] * mpr;
}





