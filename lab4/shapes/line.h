#ifndef _LINE_H_
#define _LINE_H_

#include "shape.h"
#include "vec2.h"
#include <windows.h>

class Line : public virtual Shape{
    public:
        Line(Vector2 start, Vector2 end) : start(start), end(end){};
        ~Line() = default;
        void draw(HDC hdc) override;
        void preview_draw(HDC hdc) override;
        void set_second(Vector2 value) override;
        Vector2 start;
        Vector2 end;
    private:
        
};

#endif
