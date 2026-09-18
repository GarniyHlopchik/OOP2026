#ifndef _DOT_H_
#define _DOT_H_

#include "shape.h"
#include "vec2.h"
#include <windows.h>

class Dot : public Shape{
    public:
        Dot(Vector2 position) : position(position){};
        ~Dot() override = default;
        void draw(HDC hdc) override;
    private:
        Vector2 position;
};

#endif

