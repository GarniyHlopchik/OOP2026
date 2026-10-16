#ifndef _RECT_H_
#define _RECT_H_

#include "shape.h"
#include "vec2.h"

#include <windows.h>

//black contour no fill

class Rect : public virtual Shape{
    public:
        Rect(Vector2 center, Vector2 delta) : center(center), delta(delta){};
        ~Rect() = default;
        void draw(HDC hdc) override;
        void preview_draw(HDC hdc) override;
        void set_second(Vector2 value) override;
    protected:
        Vector2 center;
        Vector2 delta;
    private:
        
};

#endif

