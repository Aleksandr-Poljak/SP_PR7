#pragma once

#include "resource.h"
#include <wtypes.h>

extern HWND hButtonText;
extern HWND hButtonDrawShape;
extern HWND hButtonImage;

extern bool textFlag;

#define IDC_BTN_TEXT	150
#define IDC_BTN_DRAW	151
#define IDC_BTN_IMAGE	152

//Функция обертка для создания шрифта согласно условию
HFONT APIENTRY CreateTaskFont();
