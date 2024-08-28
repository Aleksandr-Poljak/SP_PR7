#include "SP_PR7.h"
#include <wtypes.h>

HFONT APIENTRY CreateTaskFont()
{
    HFONT hFont = CreateFont(
        -36,                          // Высота шрифта
        0,                            // Средняя ширина символов
        0,                            // Угол наклона текста относительно оси X
        0,                            // Угол наклона текста относительно оси Y
        FW_BOLD,                      // Жирный шрифт
        TRUE,                         // Курсив
        FALSE,                        // Подчеркивание
        FALSE,                        // Зачеркивание
        DEFAULT_CHARSET,              // Набор символов
        OUT_DEFAULT_PRECIS,           // Точность вывода
        CLIP_DEFAULT_PRECIS,          // Точность отсечения
        DEFAULT_QUALITY,              // Качество вывода
        DEFAULT_PITCH | FF_SWISS,     // Шаг шрифта и семейство шрифтов
        TEXT("Microsoft Sans Serif")); // Имя шрифта
	
    return hFont;
};