#include "SP_PR7.h"
#include <wtypes.h>
#include <cmath>
#include <windows.h>

HFONT APIENTRY CreateTaskFont()
{
    HFONT hFont = CreateFont(
        -36,                          // ������ ������
        0,                            // ������� ������ ��������
        0,                            // ���� ������� ������ ������������ ��� X
        0,                            // ���� ������� ������ ������������ ��� Y
        FW_BOLD,                      // ������ �����
        TRUE,                         // ������
        FALSE,                        // �������������
        FALSE,                        // ������������
        DEFAULT_CHARSET,              // ����� ��������
        OUT_DEFAULT_PRECIS,           // �������� ������
        CLIP_DEFAULT_PRECIS,          // �������� ���������
        DEFAULT_QUALITY,              // �������� ������
        DEFAULT_PITCH | FF_SWISS,     // ��� ������ � ��������� �������
        TEXT("Microsoft Sans Serif")); // ��� ������
	
    return hFont;
};

// ������� ��� ��������� ������ � ����������

void DrawStarFullShading(HDC hdc, int xCenter, int yCenter, int radius)
{
    // ������ ������ � ������ �������� � ��������� ���������� 

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH hBrush =  CreateHatchBrush(HS_FDIAGONAL, RGB(255, 255, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    

    const int numPoints = 5; // ���������� ������ ������
    POINT starPoints[numPoints * 2]; // ������� ������

    // ���� ��� ������� ��������� ������
    double angleStep = 2 * M_PI / numPoints;
    double startAngle = -M_PI / 2; // ��������� ���� ��� ������� ������� ������
    double innerRadius = radius * 0.4; // ������ ��� ���������� ������

    // ������������ ���������� ������ ������
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
/// ��������� ��� �������� ����� �����������.
/// </summary>
struct Triangle
{
    Triangle() : ptTop{ 0, 0 }, ptLeftBt{ 0, 0 }, ptRightBt{ 0, 0 } {}

    POINT ptTop; // ����� �������
    POINT ptLeftBt; // ����� ����� ���������
    POINT ptRightBt; // ������ ����� ���������
};

void DrawStarTopsShading(HDC hdc, int xCenter, int yCenter, int radius)
{
    // ������ ������ � ������ �������� � ��������� ���������� 

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
    HBRUSH hBrushTops = CreateHatchBrush(HS_FDIAGONAL, RGB(255, 255, 0));
    HBRUSH hBrushEl = (HBRUSH)GetStockObject(NULL_BRUSH);

    const int numPoints = 5; // ���������� ������ ������
    POINT starPoints[numPoints * 2]; // ������� ������
        
    // ���� ��� ������� ��������� ������
    double angleStep = 2 * M_PI / numPoints;
    double startAngle = -M_PI / 2; // ��������� ���� ��� ������� ������� ������
    double innerRadius = radius * 0.4; // ������ ��� ���������� ������

    Triangle trStart; // ������ ����������� �� ������ �������.
    Triangle trTemp; // ��������� ����������� ��� ����������  �������

    Triangle triangles[numPoints+1]; // ������ ������������� �� ����� ������ ������.
    triangles[0] = trStart;
    int trianglesIndex = 1;
    int countAngles = 0;  
    // ��������� ������ ������ � ������������ �� ��� ������������
    for (int i = 0; i < numPoints * 2; i++)
    {
        double angle = startAngle + i * angleStep / 2;
        int r = (i % 2 == 0) ? radius : innerRadius;
        starPoints[i].x = static_cast<LONG>(xCenter + r * cos(angle));
        starPoints[i].y = static_cast<LONG>(yCenter + r * sin(angle));

        // ���������� ����� ������� ������� ������������ �� ����� ������ ������� ������
        if (i == 0)
        {
            triangles[0].ptTop.x = starPoints[i].x;
            triangles[0].ptTop.y = starPoints[i].y;
        }

        switch (countAngles)
        {
            // ������������ ������������� �� ������ ������
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

            // ���������� �����. � ������ � ������ ���������� ������
            triangles[trianglesIndex] = trTemp;
            trianglesIndex++;

            trTemp = Triangle();
            trTemp.ptRightBt.x = starPoints[i].x;
            trTemp.ptRightBt.y = starPoints[i].y;
            countAngles = 1;
            break;
        }

        countAngles++; // ������� ����������� ����� �����.
    }
    // ���������� ����� ������� ������������
    triangles[0].ptLeftBt.x = triangles[1].ptRightBt.x;
    triangles[0].ptLeftBt.y = triangles[1].ptRightBt.y;
    triangles[0].ptRightBt.x = triangles[numPoints - 1].ptLeftBt.x;
    triangles[0].ptRightBt.y = triangles[numPoints - 1].ptLeftBt.y;

    // ��������� ������ � ��������� ����������
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HPEN hOldBrushEl = (HPEN)SelectObject(hdc, hBrushEl);
    Ellipse(hdc, xCenter - radius, yCenter - radius, xCenter + radius, yCenter + radius);
    Polygon(hdc, starPoints, numPoints * 2);

    // ���������� ���������� ������ ������ � ����������� ������������� ������������� �� ������.
    for (int j = 0; j <= numPoints; j++)
    {
        POINT triangleBottomPoints[2];
        triangleBottomPoints[0] = triangles[j].ptLeftBt;
        triangleBottomPoints[1] = triangles[j].ptRightBt;
        Polyline(hdc, triangleBottomPoints, 2);
    };

    SelectObject(hdc, hOldBrushEl); 
    SelectObject(hdc, hPen);

    // ���������� ������������� (�� ������ ������ ) ������ � ����������
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

    // ������������ ��������
    DeleteObject(hPen);
    DeleteObject(hBrushTops);
    DeleteObject(hBrushEl);

}