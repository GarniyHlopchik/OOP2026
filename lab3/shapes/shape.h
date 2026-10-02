#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <windows.h>
#include "vec2.h"

class Shape{
    public:
        Shape() = default;
        virtual ~Shape() = default;
        virtual void draw(HDC hdc) = 0;
        virtual void preview_draw(HDC hdc) = 0;
        virtual void set_second(Vector2 value) = 0;
};

#endif

