#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <windows.h>
#include "vec2.h"
#include <utility>


class Shape{
    public:
        Shape() = default;
        virtual ~Shape() = default;
        virtual void draw(HDC hdc) = 0;
        virtual void preview_draw(HDC hdc) = 0;
        virtual void set_second(Vector2 value) = 0;
        virtual std::pair<Vector2, Vector2> get_data() = 0;
        virtual PCWSTR name() const = 0;
        bool is_selected = false;
};

#endif

