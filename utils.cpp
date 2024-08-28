#include "SP_PR7.h"
#include <wtypes.h>

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