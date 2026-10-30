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
        void preview_draw(HDC hdc) override;
        void set_second(Vector2 value) override;
        std::pair<Vector2, Vector2> get_data() override;
        PCWSTR name() const override { return L"Точка"; }
    private:
        Vector2 position;
};

#endif

