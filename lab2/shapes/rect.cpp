#include <windows.h>
#include "rect.h"

void Rect::draw(HDC hdc){
    HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
    Rectangle(hdc, center.x-delta.x,center.y-delta.y,center.x+delta.x,center.y+delta.y);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hNullBrush);

}