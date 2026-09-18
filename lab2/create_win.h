#ifndef _CREATE_WIN_
#define _CREATE_WIN_

#include <windows.h>
#include "shapes/shape.h"

#define DOT    2
#define LINE   3
#define RECT   4
#define ELIPSE 5
struct ArrData{
    Shape** array;
    int* size;
};
int create_shape(HWND hwnd, int shape, Shape** arr, int* size);

#endif