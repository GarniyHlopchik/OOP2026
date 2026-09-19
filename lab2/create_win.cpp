#include "create_win.h"
#include "shapes/shape.h"
#include "shapes/dot.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/elipse.h"
#include "shapes/vec2.h"

static int chosen_shape;

#define MSG_1 1000
#define MSG_2 1001
#define MSG_3 1002
#define MSG_4 1003

#define DATA_1 1004
#define DATA_2 1005
#define DATA_3 1006
#define DATA_4 1007

#define BTN_OK     1008
#define BTN_CANCEL 1009

struct DialogContext {
    HWND hMainWnd;
    ArrData arrData;
};
extern Shape* PreviewShape;
Shape* CreateShapeObject(Vector2 vec1, Vector2 vec2, int shape_id){
    Shape* shape;
    switch (shape_id) {
        case 2: shape = new Dot(vec1); break;
        case 3: shape = new Line(vec1, vec2); break;
        case 4: shape = new Rect(vec1, vec2); break;
        case 5: shape = new Elipse(vec1, vec2); break;
    }
    return shape;
}

HWND create_shape(HWND hwnd, int shape, Shape** arr, int* size) {
    
}