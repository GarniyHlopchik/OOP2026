#include "shapes/shape.h"
#ifndef _CREATE_H_
#define _CREATE_H_
Shape* CreateShapeObject(Vector2 vec1, Vector2 vec2, int shape_id);
HICON CreateIconFromBitmap(HBITMAP hBitmap, COLORREF transparentColor);
HICON LoadIconFromFile(const wchar_t* filePath, int width, int height);

#endif