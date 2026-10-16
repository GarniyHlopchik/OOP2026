#include "circled_line.h"

void CircledLine::draw(HDC hdc){
    Line::draw(hdc);
    Elipse::corner1 = Vector2{Line::start.x, Line::start.y};
    Elipse::corner2 = Vector2{Line::start.x+CIRCLE_RADIUS, Line::start.y+CIRCLE_RADIUS};
    Elipse::draw(hdc);
    Elipse::corner1 = Vector2{Line::end.x, Line::end.y};
    Elipse::corner2 = Vector2{Line::end.x+CIRCLE_RADIUS, Line::end.y+CIRCLE_RADIUS};
    Elipse::draw(hdc);

}

void CircledLine::preview_draw(HDC hdc){
    HBRUSH hBlueBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);
    HPEN hBlackPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);
    
    //draw
    Line::preview_draw(hdc);
    Elipse::corner1 = Vector2{Line::start.x, Line::start.y};
    Elipse::corner2 = Vector2{Line::start.x+CIRCLE_RADIUS, Line::start.y+CIRCLE_RADIUS};
    Elipse::preview_draw(hdc);
    Elipse::corner1 = Vector2{Line::end.x, Line::end.y};
    Elipse::corner2 = Vector2{Line::end.x+CIRCLE_RADIUS, Line::end.y + CIRCLE_RADIUS};
    Elipse::preview_draw(hdc);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc,hOldPen);
    DeleteObject(hBlackPen);
    DeleteObject(hBlueBrush);
}

void CircledLine::set_second(Vector2 value){
    Line::set_second(value);
}