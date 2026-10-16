#include <windows.h>
#include "line.h"

void Line::draw(HDC hdc){
    MoveToEx(hdc, start.x, start.y, NULL);          // Set starting point
    LineTo(hdc, end.x, end.y);                  // Draw line to
}

void Line::preview_draw(HDC hdc){
    MoveToEx(hdc, start.x, start.y, NULL);          // Set starting point

    HBRUSH hBlueBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    
    LineTo(hdc, end.x, end.y);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);

                      // Draw line to
}

void Line::set_second(Vector2 value){
    end= value;
}