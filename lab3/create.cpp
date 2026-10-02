#include "create.h"
#include "shapes/dot.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/elipse.h"
Shape* CreateShapeObject(Vector2 vec1, Vector2 vec2, int shape_id){
    Shape* shape;
    Vector2 delta = Vector2{vec2.x - vec1.x, vec2.y - vec1.y};
    switch (shape_id) {
        case 2: shape = new Dot(vec1); break;
        case 3: shape = new Line(vec1, vec2); break;
        case 4: shape = new Rect(vec1, delta); break;
        case 5: shape = new Elipse(vec1, vec2); break;
    }
    return shape;
}