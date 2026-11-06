#include <cmath>
#include <algorithm>
#include "cube.h"

void Cube::draw(HDC hdc){
    Rect::is_selected = is_selected;
    Line::is_selected = is_selected;
    // 1. Save original bounding box coordinates (corner and offset)
    Vector2 cache_start = Rect::center; 
    Vector2 cache_end = Rect::delta;    

    int total_width = cache_end.x;
    int total_height = cache_end.y;

    // 2. Calculate the backplane offset in pixels
    int offset_x = (int)(total_width * BACKPLANE_OFFSET_X);
    int offset_y = (int)(total_height * BACKPLANE_OFFSET_Y);

    // 3. Correct rectangle sizing: Fit both rects inside the total dragged bounds
    int cube_width = total_width - offset_x;
    int cube_height = total_height - offset_y;

    // 4. Draw Front Rectangle
    Rect::center = cache_start;
    Rect::delta = Vector2{cube_width, cube_height};
    Rect::draw(hdc);

    Vector2 cache_cube1_start = Rect::center;
    Vector2 cache_cube1_end = Vector2{Rect::center.x + Rect::delta.x, Rect::center.y + Rect::delta.y};

    // 5. Draw Back Rectangle (shifted by offset, matching front size)
    int back_start_x = cache_start.x + offset_x;
    int back_start_y = cache_start.y + offset_y;

    Rect::center = Vector2{back_start_x, back_start_y};
    Rect::delta = Vector2{cube_width, cube_height};
    Rect::draw(hdc);

    Vector2 cache_cube2_start = Rect::center;
    Vector2 cache_cube2_end = Vector2{Rect::center.x + Rect::delta.x, Rect::center.y + Rect::delta.y};

    // 6. Connect corresponding corners with Lines
    Line::start = Vector2{cache_cube1_start.x, cache_cube1_end.y};
    Line::end = Vector2{cache_cube2_start.x, cache_cube2_end.y};
    Line::draw(hdc);

    Line::start = cache_cube1_end;
    Line::end = cache_cube2_end;
    Line::draw(hdc);

    Line::start = Vector2{cache_cube1_end.x, cache_cube1_start.y};
    Line::end = Vector2{cache_cube2_end.x, cache_cube2_start.y};
    Line::draw(hdc);

    Line::start = cache_cube1_start;
    Line::end = cache_cube2_start;
    Line::draw(hdc);

    // 7. Restore original Rect state
    Rect::center = cache_start;
    Rect::delta = cache_end;
}

void Cube::preview_draw(HDC hdc){
    draw(hdc);
}

void Cube::set_second(Vector2 value){
    Rect::set_second(value);
    Line::set_second(value);
}

std::pair<Vector2, Vector2> Cube::get_data(){
    return std::make_pair(Rect::center, Rect::delta);
}