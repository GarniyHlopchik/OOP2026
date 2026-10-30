#pragma once

#include "line.h"
#include "rect.h"
#include "vec2.h"
#include <windows.h>

#define BACKPLANE_OFFSET_X 0.3
#define BACKPLANE_OFFSET_Y 0.4

class Cube : public Rect, public Line{
    public:
        Cube(Vector2 start, Vector2 end): Shape(), Rect(start,end), Line(start,end){}
        void draw(HDC hdc);

        void preview_draw(HDC hdc);

        void set_second(Vector2 value);

        std::pair<Vector2, Vector2> get_data() override;
        PCWSTR name() const override { return L"Куб"; }
};