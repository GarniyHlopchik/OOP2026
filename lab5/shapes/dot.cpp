#include <windows.h>
#include "dot.h"

void Dot::draw(HDC hdc){
    if(is_selected){
        SetPixel(hdc, position.x, position.y, RGB(0, 20, 255));
    }
    else{
        SetPixel(hdc, position.x, position.y, RGB(0, 0, 0));
    }
    
}

void Dot::preview_draw(HDC hdc){
    SetPixel(hdc, position.x, position.y, RGB(0, 0, 0));
}
void Dot::set_second(Vector2 value){}

std::pair<Vector2, Vector2> Dot::get_data(){
    return std::make_pair(position, position);
}