/********************************************************************/
/*                           anim_proc                              */
/*                           =========                              */
/*    Функции для анимации  произвольного набора плоских объектов   */
/*                                                                  */
/********************************************************************/
#include "tdef.cpp"

#include "resource.h"

#define ALL_STEP       200        // число  временных  интервалов
#define X_SPACE_ANIM   100.0      // Размер области вывода по X
#define Y_SPACE_ANIM    70.0      // Размер области вывода по Y
#define DX X_SPACE_ANIM/ALL_STEP  // шаг анимации по X
#define DY Y_SPACE_ANIM/ALL_STEP  // шаг анимации по Y

#define  K_SCALE  0.1           // коэффициент пропорциональности

#define  K_SC     X_SPACE_ANIM * K_SCALE  // масштабный коэфф.


#define  K_GAB    0.1           // Габаритный  коэффициент отступа
								//  выводимого изображение от края границы вывода
#define  G		 9.8
#define  PI       3.14159265
HDC hdc;
static double k = 0;
HBRUSH 	hbr[] = { CreateSolidBrush(RGB(255, 0, 0)),
				  CreateSolidBrush(RGB(0, 0, 0)),
				  CreateSolidBrush(RGB(0, 0, 255)),
				  CreateSolidBrush(RGB(255, 255, 255)),
				  CreateSolidBrush(RGB(255, 205, k)),
				  CreateSolidBrush(RGB(127, 199, 255)),
	CreateSolidBrush(RGB(193, 68, 14)),
	CreateSolidBrush(RGB(173, 173, 173)),
	CreateSolidBrush(RGB(227, 187, 118))
},
hbr_prv;


int pr_start = 0, all_br = sizeof(hbr) / sizeof(hbr[0]);

HPEN pen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
double g = G;
static double N = 0;
CMatr  mpr;
double v = 0;
/********************************************************************/
/*                        anim_main                                 */
/*                        ==========                                */
/*      Управляющая  процедура  отрисовки  полёта  квадрата         */
/********************************************************************/

CVect  sun[ALL_PT] =
{ {  0.5,   0.5  },
	{ -0.5,   0.5 },
	{ -0.5,  -0.5 },
	{  0.5,  -0.5 }
};





GO_SHAPE Sun(sun, 4, 0, 0, 0, K_SC, K_SC,
	X_SPACE_ANIM / 2, 0, SUN, X_SPACE_ANIM),
	line (sun, 4, 0, 0, 0, K_SC, K_SC,
		X_SPACE_ANIM / 2, 0, 1, X_SPACE_ANIM);
	

BOOL  anim_main(HWND hwnd)
{
	int    i;
	double dx, dy, ang;

	extern int timer, znak, timer2;

	CVect vt;


	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);

	if (pr_start == 0)
	{
		pr_start = 1;
		EndPaint(hwnd, &ps);
		return TRUE;
		
	}

	// определить размеры клиентской области окна
	if (timer == 0)
	{
		for (size_t i = 0; i < 255; i++)
		{
			hbr[i] = CreateSolidBrush(RGB(255, 205, i));
		}
		RECT rc;
		GetClientRect(hwnd, &rc);

		mpr = make_matr_pr(rc);
	}

	/*---------------------------------------*/
	/*      Текущая перерисовка              */
	/*---------------------------------------*/
	Sun.vt.x = X_SPACE_ANIM/2;
	

	line.vt.x = X_SPACE_ANIM / 2;
	line.vt.y = Y_SPACE_ANIM / 2;



	

	//line.move();

	//
	//SelectObject(hdc, pen);

	//MoveToEx(hdc, line.kvadro_win[2].x, line.kvadro_win[2].y, NULL);
	//LineTo(hdc, line.kvadro_win[3].x, line.kvadro_win[3].y);
	
	Sun.color = k;
	k = k + 2;

	
	v = (timer * g);

	
	Sun.vt.y = (Y_SPACE_ANIM)-((v * timer) + ((pow(timer, 2) * g) / 2)) / 1000;


		if (Sun.vt.y <= Y_SPACE_ANIM/2 ) {
		
			znak = -znak;
			k = 0;
		}
		
		
	
		Sun.move();

		



	/*---------------------------------------*/


	EndPaint(hwnd, &ps);

	return TRUE;
}

/********************************************************************/
/*                        draw_polygon                              */
/*                        ============                              */
/*      Рисование многоугольника  на экране по  вещественным коорд  */
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
/*              Формирование  матрицы  проекции                     */
/********************************************************************/
CMatr  make_matr_pr(RECT& cp)
{
	int a, b, gab_a, gab_b, xc_w, yc_w;
	double x_max, y_max, x_min, y_min, k_scale, k1, k2,
		dx, dy, xc, yc, l_snar, l_pushka;

	CMatr  mt1, ms, mr, mt2;

	CVect  vt;


	/*--------------------------------*/
	/*  Параметры  окна  вывода       */
	/*--------------------------------*/
	a = abs(cp.right - cp.left);
	b = abs(cp.top - cp.bottom);

	gab_a = (int)((double)a * K_GAB);  // отступ по ширине
	gab_b = (int)((double)b * K_GAB);  // отступ по высоте

	a -= gab_a;
	b -= gab_b;

	xc_w = (cp.right + cp.left) / 2;
	yc_w = (cp.top + cp.bottom) / 2;

	/*-----------------------------------------*/
	/*  Определение  масштабного коэффициента  */
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
	// Зеркальное отображение
	MatrScale(mr, 1, -1);

	vt.x = xc_w;
	vt.y = yc_w;
	MatrTransl(mt2, vt);

	return (mt1 * ms * mr * mt2);
}

/********************************************************************/
/*                           trans_coor                             */
/*                           ==========                             */
/*  Изменение массива координат  по  матрице  преобразований        */
/********************************************************************/

void trans_coor(CVect* pvOld, CVect* pvNew, int all_pt, CMatr& mpr)
{
	for (int i = 0; i < all_pt; i++)
		pvNew[i] = pvOld[i] * mpr;
}





