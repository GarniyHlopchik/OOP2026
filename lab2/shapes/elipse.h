#ifndef _ELIPSE_H_
#define _ELIPSE_H_

#include "shape.h"
#include "vec2.h"
#include <windows.h>

//black contour light-blue fill

class Elipse : public Shape{
    public:
        Elipse(Vector2 corner1, Vector2 corner2) : corner1(corner1), corner2(corner2){};
        ~Elipse() = default;
        void draw(HDC hdc) override;
        void preview_draw(HDC hdc) override;
    private:
        Vector2 corner1;
        Vector2 corner2;
};

#endif

