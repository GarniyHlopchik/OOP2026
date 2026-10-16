#include "create.h"
#include "shapes/dot.h"
#include "shapes/line.h"
#include "shapes/rect.h"
#include "shapes/elipse.h"
#include "shapes/circled_line.h"
#include "shapes/cube.h"
#include "shapes/shape.h"
#include "windows.h"
Shape* CreateShapeObject(Vector2 vec1, Vector2 vec2, int shape_id){
    Shape* shape;
    Vector2 delta = Vector2{vec2.x - vec1.x, vec2.y - vec1.y};
    switch (shape_id) {
        case 2: shape = new Dot(vec1); break;
        case 3: shape = new Line(vec1, vec2); break;
        case 4: shape = new Rect(vec1, delta); break;
        case 5: shape = new Elipse(vec1, vec2); break;
        case 6: shape = new CircledLine(vec1,vec2); break;
        case 7: shape = new Cube(vec1,Vector2{0,0}); break;
    }
    return shape;
}
HICON CreateIconFromBitmap(HBITMAP hBitmap, COLORREF transparentColor) {
    BITMAP bm;
    GetObject(hBitmap, sizeof(BITMAP), &bm);

    // 1. Create a monochrome mask bitmap for transparency
    // (If the pixel matches transparentColor, it's transparent; otherwise opaque)
    HDC hDC = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hDC);
    HDC hMaskDC = CreateCompatibleDC(hDC);

    // Create the color and mask bitmaps
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);
    
    // Create monochrome mask (1 bit per pixel)
    HBITMAP hMaskBmp = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    HBITMAP hOldMaskBmp = (HBITMAP)SelectObject(hMaskDC, hMaskBmp);

    // Set background color to the transparent color for mask generation
    COLORREF oldBkColor = SetBkColor(hMemDC, transparentColor);

    // Copy bits: pixels matching transparentColor become white (transparent), others black (opaque)
    BitBlt(hMaskDC, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

    // Clean up DC selections
    SetBkColor(hMemDC, oldBkColor);
    SelectObject(hMemDC, hOldBmp);
    SelectObject(hMaskDC, hOldMaskBmp);
    DeleteDC(hMemDC);
    DeleteDC(hMaskDC);
    ReleaseDC(NULL, hDC);

    // 2. Populate the ICONINFO structure
    ICONINFO iconInfo = {};
    iconInfo.fIcon = TRUE; // TRUE for icon, FALSE for cursor
    iconInfo.xHotspot = 0;
    iconInfo.yHotspot = 0;
    iconInfo.hbmMask = hMaskBmp;   // The transparency mask
    iconInfo.hbmColor = hBitmap;   // The actual color bitmap

    // 3. Create the icon
    HICON hIcon = CreateIconIndirect(&iconInfo);

    // Clean up the mask bitmap (CreateIconIndirect makes a copy of the bitmaps)
    DeleteObject(hMaskBmp);

    return hIcon;
}
HICON LoadIconFromFile(const wchar_t* filePath, int width, int height) {
    // 1. Load the bitmap from an external file
    HBITMAP hBmp = (HBITMAP)LoadImageW(
        NULL, 
        filePath, 
        IMAGE_BITMAP, 
        width, 
        height, 
        LR_LOADFROMFILE | LR_CREATEDIBSECTION
    );

    if (!hBmp) return NULL;

    // 2. Convert to icon (using Magenta RGB(255, 0, 255) as the transparent color, for example)
    HICON hIcon = CreateIconFromBitmap(hBmp, RGB(255, 0, 255));

    // You can delete the source HBITMAP now if you no longer need it independently
    DeleteObject(hBmp); 

    return hIcon;
}