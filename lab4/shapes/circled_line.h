#pragma once

#include "line.h"
#include "elipse.h"
#include "vec2.h"
#include <windows.h>

#define CIRCLE_RADIUS 10

class CircledLine : public Line, public Elipse{
    public:
        CircledLine(Vector2 start, Vector2 end) : Shape(), Line(start,end), Elipse(start,end){} //doesn't matter as circles need config every time;
        void draw(HDC hdc);

        void preview_draw(HDC hdc);

        void set_second(Vector2 value);
};