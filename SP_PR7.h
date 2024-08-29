#pragma once

#include "resource.h"
#include <wtypes.h>

extern HWND hButtonText;
extern HWND hButtonDrawShape;
extern HWND hButtonImage;

extern bool textFlag;
extern bool shapeFlag;

#define IDC_BTN_TEXT	150
#define IDC_BTN_DRAW	151
#define IDC_BTN_IMAGE	152
#define _USE_MATH_DEFINES 

//Функция обертка для создания шрифта согласно условию
HFONT APIENTRY CreateTaskFont();


// Функция для создания звезды с полной заливкой
void DrawStarFullShading(HDC hdc, int xCenter, int yCenter, int radius);

// Функция для создания звезды с частичной заливкой только вершин.
void DrawStarTopsShading(HDC hdc, int xCenter, int yCenter, int radius);