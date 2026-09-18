#ifndef _LINE_H_
#define _LINE_H_

#include "shape.h"
#include "vec2.h"
#include <windows.h>

class Line : public Shape{
    public:
        Line(Vector2 start, Vector2 end) : start(start), end(end){};
        ~Line() = default;
        void draw(HDC hdc) override;
        void preview_draw(HDC hdc) override;
    private:
        Vector2 start;
        Vector2 end;
};

#endif
