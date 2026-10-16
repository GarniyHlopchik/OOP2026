#include <windows.h>
#include "dot.h"

void Dot::draw(HDC hdc){
    SetPixel(hdc, position.x, position.y, RGB(0, 0, 0));
}

void Dot::preview_draw(HDC hdc){
    SetPixel(hdc, position.x, position.y, RGB(0, 0, 0));
}
void Dot::set_second(Vector2 value){}