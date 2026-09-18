#include <windows.h>
#include "elipse.h"

void Elipse::draw(HDC hdc){
    HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 122, 255));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    Ellipse(hdc,corner1.x,corner1.y,corner2.x,corner2.y);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);
}

void Elipse::preview_draw(HDC hdc){
    HBRUSH hBlueBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    Ellipse(hdc,corner1.x,corner1.y,corner2.x,corner2.y);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);
}