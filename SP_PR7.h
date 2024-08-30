#pragma once

#include "resource.h"
#include <wtypes.h>

extern HWND hButtonText;
extern HWND hButtonDrawShape;
extern HWND hButtonImage;
extern HWND hButtonTextMetafile;

extern HDC hDCMetafile;
extern HMETAFILE hmf;
extern LPCWSTR metafileName;

extern HDC hdc;
extern HBITMAP hBitmap;
extern HDC memBit;
extern BITMAP bm;



extern bool textFlag;
extern bool shapeFlag;
extern bool textFlagMetafile;

#define IDC_BTN_TEXT	150
#define IDC_BTN_DRAW	151
#define IDC_BTN_IMAGE	152
#define IDC_BTN_TEXT_IN_METAFILE	153
#define _USE_MATH_DEFINES 

//Функция обертка для создания шрифта согласно условию
HFONT APIENTRY CreateTaskFont();

// Функция для создания звезды с полной закраской
void DrawStarFullShading(HDC hdc, int xCenter, int yCenter, int radius);

// Функция для создания звезды с частичной закраской только вершин.
void DrawStarTopsShading(HDC hdc, int xCenter, int yCenter, int radius);