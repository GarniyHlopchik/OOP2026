#include <windows.h>
#include "elipse.h"

void Elipse::draw(HDC hdc){
    HBRUSH hBlueBrush = CreateSolidBrush(RGB(247, 2, 170));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    Vector2 delta = Vector2{corner1.x-corner2.x,corner1.y-corner2.y};
    Ellipse(hdc,corner1.x-delta.x,corner1.y-delta.y,corner1.x+delta.x,corner1.y+delta.y);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);
}

void Elipse::preview_draw(HDC hdc){
    HBRUSH hBlueBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    Vector2 delta = Vector2{corner1.x-corner2.x,corner1.y-corner2.y};
    Ellipse(hdc,corner1.x-delta.x,corner1.y-delta.y,corner1.x+delta.x,corner1.y+delta.y);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);
}

void Elipse::set_second(Vector2 value){
    corner2 = value;
}