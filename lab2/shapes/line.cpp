#include <windows.h>
#include "line.h"

void Line::draw(HDC hdc){
    MoveToEx(hdc, start.x, start.y, NULL);          // Set starting point
    LineTo(hdc, end.x, end.y);                  // Draw line to
}

void Line::preview_draw(HDC hdc){
    MoveToEx(hdc, start.x, start.y, NULL);          // Set starting point
    LineTo(hdc, end.x, end.y);                  // Draw line to
}