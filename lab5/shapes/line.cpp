#include <windows.h>
#include "line.h"

void Line::draw(HDC hdc){
    if(is_selected){
        HBRUSH hNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hNullBrush);
        HPEN hBluePen = CreatePen(PS_SOLID, 1, RGB(0, 20, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBluePen);
        MoveToEx(hdc, start.x, start.y, NULL);          // Set starting point
        LineTo(hdc, end.x, end.y); 
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hNullBrush);
        DeleteObject(hBluePen);
        return;
    }
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

std::pair<Vector2, Vector2> Line::get_data(){
    return std::make_pair(start, end);
}