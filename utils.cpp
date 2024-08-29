#include "SP_PR7.h"
#include <wtypes.h>
#include <cmath>
#include <windows.h>

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

// Функции для рисования звезды и окружности

void DrawStarFullShading(HDC hdc, int xCenter, int yCenter, int radius)
{
    // Рисует звезду с полной заливкой и описанную окружность 

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH hBrush =  CreateHatchBrush(HS_FDIAGONAL, RGB(255, 255, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    

    const int numPoints = 5; // Количество вершин звезды
    POINT starPoints[numPoints * 2]; // Вершины звезды

    // Углы для расчета координат вершин
    double angleStep = 2 * M_PI / numPoints;
    double startAngle = -M_PI / 2; // Начальный угол для верхней вершины звезды
    double innerRadius = radius * 0.4; // Радиус для внутренних вершин

    // Рассчитываем координаты вершин звезды
    for (int i = 0; i < numPoints * 2; i++)
    {
        double angle = startAngle + i * angleStep / 2;
        int r = (i % 2 == 0) ? radius : innerRadius;
        starPoints[i].x = static_cast<LONG>(xCenter + r * cos(angle));
        starPoints[i].y = static_cast<LONG>(yCenter + r * sin(angle));
    }

    Ellipse(hdc, xCenter - radius, yCenter - radius, xCenter + radius, yCenter + radius);
    Polygon(hdc, starPoints, numPoints * 2);

    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Polygon(hdc, starPoints, numPoints * 2);

    
    DeleteObject(hPen);
    DeleteObject(hBrush);
    DeleteObject(hOldBrush);
    DeleteObject(hOldPen);
}

/// <summary>
/// Структура для хранения точек треугльника.
/// </summary>
struct Triangle
{
    Triangle() : ptTop{ 0, 0 }, ptLeftBt{ 0, 0 }, ptRightBt{ 0, 0 } {}

    POINT ptTop; // Точка вершины
    POINT ptLeftBt; // Левая точка основания
    POINT ptRightBt; // Правая точка основания
};

void DrawStarTopsShading(HDC hdc, int xCenter, int yCenter, int radius)
{
    // Рисует звезду с полной заливкой и описанную окружность 

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH hBrushTops = CreateHatchBrush(HS_FDIAGONAL, RGB(255, 255, 0));
    HBRUSH hBrushEl = (HBRUSH)GetStockObject(NULL_BRUSH);

    const int numPoints = 5; // Количество вершин звезды
    POINT starPoints[numPoints * 2]; // Вершины звезды
        
    // Углы для расчета координат вершин
    double angleStep = 2 * M_PI / numPoints;
    double startAngle = -M_PI / 2; // Начальный угол для верхней вершины звезды
    double innerRadius = radius * 0.4; // Радиус для внутренних вершин

    Triangle trStart; // Первый треугольник из первой вершины.
    Triangle trTemp; // Временный треугольник для заполнения  массива

    Triangle triangles[numPoints+1]; // Массив треугольников из точек вершин звезды.
    triangles[0] = trStart;
    int trianglesIndex = 1;
    int countAngles = 0;  
    // Вычислние вершин звезды и формирование из них треугольники
    for (int i = 0; i < numPoints * 2; i++)
    {
        double angle = startAngle + i * angleStep / 2;
        int r = (i % 2 == 0) ? radius : innerRadius;
        starPoints[i].x = static_cast<LONG>(xCenter + r * cos(angle));
        starPoints[i].y = static_cast<LONG>(yCenter + r * sin(angle));

        // Вычисление точки вершины первого треугольника из точки первой вершины звезды
        if (i == 0)
        {
            triangles[0].ptTop.x = starPoints[i].x;
            triangles[0].ptTop.y = starPoints[i].y;
        }

        switch (countAngles)
        {
            // Формирование треугольников из вершин звезды
        case 1:
            trTemp.ptRightBt.x = starPoints[i].x;
            trTemp.ptRightBt.y = starPoints[i].y;
            break;
        case 2:
            trTemp.ptTop.x = starPoints[i].x;
            trTemp.ptTop.y = starPoints[i].y;
            break;
        case 3:
            trTemp.ptLeftBt.x = starPoints[i].x;
            trTemp.ptLeftBt.y = starPoints[i].y;

            // Добавление треуг. в массив и начало вычисления нового
            triangles[trianglesIndex] = trTemp;
            trianglesIndex++;

            trTemp = Triangle();
            trTemp.ptRightBt.x = starPoints[i].x;
            trTemp.ptRightBt.y = starPoints[i].y;
            countAngles = 1;
            break;
        }

        countAngles++; // Счетчик вычеслинных точек треуг.
    }
    // Добавление точек первому треугольнику
    triangles[0].ptLeftBt.x = triangles[1].ptRightBt.x;
    triangles[0].ptLeftBt.y = triangles[1].ptRightBt.y;
    triangles[0].ptRightBt.x = triangles[numPoints - 1].ptLeftBt.x;
    triangles[0].ptRightBt.y = triangles[numPoints - 1].ptLeftBt.y;

    // Рисование звезды и описанной окружности
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HPEN hOldBrushEl = (HPEN)SelectObject(hdc, hBrushEl);
    Ellipse(hdc, xCenter - radius, yCenter - radius, xCenter + radius, yCenter + radius);
    Polygon(hdc, starPoints, numPoints * 2);

    // Соединение внутренних вершин звезды и образование соприкасаемых треугольников из вершин.
    for (int j = 0; j <= numPoints; j++)
    {
        POINT triangleBottomPoints[2];
        triangleBottomPoints[0] = triangles[j].ptLeftBt;
        triangleBottomPoints[1] = triangles[j].ptRightBt;
        Polyline(hdc, triangleBottomPoints, 2);
    };

    SelectObject(hdc, hOldBrushEl); 
    SelectObject(hdc, hPen);

    // Заполнение треугольников (из вершин звезды ) кистью с штриховкой
    HPEN hOldBrushTops = (HPEN)SelectObject(hdc, hBrushTops);
    for (int j = 0; j <= numPoints; j++)
    {
        POINT trianglePoints[3];
        trianglePoints[0] = triangles[j].ptLeftBt;
        trianglePoints[1] = triangles[j].ptRightBt;
        trianglePoints[2] = triangles[j].ptTop;
        Polygon(hdc, trianglePoints, 3);
    };

    SelectObject(hdc, hOldBrushTops);

    // Освобождение ресурсов
    DeleteObject(hPen);
    DeleteObject(hBrushTops);
    DeleteObject(hBrushEl);

}