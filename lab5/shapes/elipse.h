#ifndef _ELIPSE_H_
#define _ELIPSE_H_

#include "shape.h"
#include "vec2.h"
#include <windows.h>



class Elipse : public virtual Shape{
    public:
        Elipse(Vector2 corner1, Vector2 corner2) : corner1(corner1), corner2(corner2){};
        ~Elipse() = default;
        void draw(HDC hdc) override;
        void preview_draw(HDC hdc) override;
        void set_second(Vector2 value) override;
        std::pair<Vector2, Vector2> get_data() override;
        Vector2 corner1;
        Vector2 corner2;
        PCWSTR name() const override { return L"Еліпс"; }
    private:
        
};

#endif

