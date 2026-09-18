#include <windows.h>
#include "dot.h"

void Dot::draw(HDC hdc){
    SetPixel(hdc, position.x, position.y, RGB(255, 255, 255));
}