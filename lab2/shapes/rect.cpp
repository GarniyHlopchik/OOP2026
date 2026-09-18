#include <windows.h>
#include "rect.h"

void Rect::draw(HDC hdc){
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
    Rectangle(hdc, center.x-delta.x,center.y-delta.y,center.x+delta.x,center.y+delta.y);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hNullBrush);

}

void Rect::preview_draw(HDC hdc){
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
    HPEN hBlackPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    Rectangle(hdc, center.x-delta.x,center.y-delta.y,center.x+delta.x,center.y+delta.y);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hNullBrush);
    DeleteObject(hBlackPen);

}