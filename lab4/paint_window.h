#ifndef _PAINT_WINDOW_H_
#define _PAINT_WINDOW_H_

#include "window.h"
#include "shapes/shape.h"
#include "create.h"
#define BUTTON_AMOUNT 6
#define MENU_FILE    1
#define MENU_DOT     2
#define MENU_LINE    3
#define MENU_RECT    4
#define MENU_ELIPSE  5
#define MENU_CIRCLED_LINE 6
#define MENU_CUBE    7
#define MENU_ABOUT   8
#define TOOL_DOT     9
#define TOOL_LINE    10
#define TOOL_RECT    11
#define TOOL_ELIPSE  12
#define TOOL_CIRCLED_LINE 13
#define TOOL_CUBE 14
#define TOOL_BAR     15

class PaintWindow : public Window {
public:
    ~PaintWindow(){
        for(int i = 0; i < 6; i++){
            DestroyIcon(icons[i]);
        }
    }
    bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                HWND parent = nullptr, HMENU menu = nullptr) override{
        
        //load icons
        icons[0] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
        icons[1] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
        icons[2] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
        icons[3] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
        icons[4] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
        icons[5] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);

        //building menu
        HMENU hMenu = CreateMenu();
        AppendMenuW(hMenu, MF_STRING, MENU_FILE, L"Файл");
        HMENU hActionMenu = CreatePopupMenu();
        AppendMenuW(hActionMenu, MF_STRING, MENU_DOT, L"Крапка");
        AppendMenuW(hActionMenu, MF_STRING, MENU_LINE, L"Лінія");
        AppendMenuW(hActionMenu, MF_STRING, MENU_RECT, L"Прямокутник");
        AppendMenuW(hActionMenu, MF_STRING, MENU_ELIPSE, L"Еліпс");
        AppendMenuW(hActionMenu, MF_STRING, MENU_CIRCLED_LINE, L"Круголінія");
        AppendMenuW(hActionMenu, MF_STRING, MENU_CUBE, L"Куб");
        AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hActionMenu, L"Об'єкти");
        AppendMenuW(hMenu, MF_STRING, MENU_ABOUT, L"Довідка");
        if(!Window::Create(title,style,exStyle,x,y,width,height,parent,hMenu)){return false;}

        // 1. Initialize Common Controls
        INITCOMMONCONTROLSEX icex = { 0 };
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_BAR_CLASSES;
        InitCommonControlsEx(&icex);

        // 2. Create Toolbar Window
        HWND hToolbar = CreateWindowEx(
            0,
            TOOLBARCLASSNAME,
            NULL,
            WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
            0, 0, 0, 0,
            m_hwnd,
            (HMENU)TOOL_BAR,
            GetModuleHandle(NULL),
            NULL
        );

        if (!hToolbar) return -1;

        // Send size structure for compatibility
        SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

        HIMAGELIST hMyImageList = ImageList_Create(64, 64, ILC_COLOR32 | ILC_MASK, 5, 0);

        HBITMAP hCustomBmp = (HBITMAP)LoadImageW(
            NULL, 
            L"icons\\icons.bmp", // A strip of images placed side-by-side
            IMAGE_BITMAP, 
            0, 0, 
            LR_LOADFROMFILE | LR_CREATEDIBSECTION
        );

        if (hCustomBmp) {
            // Add the bitmap to the image list. 
            // RGB(255, 0, 255) is Magenta, which acts as the transparent color mask.
            ImageList_AddMasked(hMyImageList, hCustomBmp, RGB(255, 0, 255));
            
            // Clean up the raw bitmap handle (ImageList makes its own copy)
            DeleteObject(hCustomBmp);
        }
        SendMessage(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hMyImageList);

        // 4. Define Toolbar Buttons
        TBBUTTON tbb[11] = { 0 };

        // Button 1: Dot
        tbb[0].iBitmap = 2;
        tbb[0].idCommand = TOOL_DOT;
        tbb[0].fsState = TBSTATE_ENABLED;
        tbb[0].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[1].fsStyle = BTNS_SEP;

        // Button 2: Line
        tbb[2].iBitmap = 3;
        tbb[2].idCommand = TOOL_LINE;
        tbb[2].fsState = TBSTATE_ENABLED;
        tbb[2].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[3].fsStyle = BTNS_SEP;

        // Button 3: Rect
        tbb[4].iBitmap = 1;
        tbb[4].idCommand = TOOL_RECT;
        tbb[4].fsState = TBSTATE_ENABLED;
        tbb[4].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[5].fsStyle = BTNS_SEP;

        // Button 3: Elipse
        tbb[6].iBitmap = 0;
        tbb[6].idCommand = TOOL_ELIPSE;
        tbb[6].fsState = TBSTATE_ENABLED;
        tbb[6].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[7].fsStyle = BTNS_SEP;

        // Button 3: CircledLine
        tbb[8].iBitmap = 5;
        tbb[8].idCommand = TOOL_CIRCLED_LINE;
        tbb[8].fsState = TBSTATE_ENABLED;
        tbb[8].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[9].fsStyle = BTNS_SEP;

        // Button 3: CircledLine
        tbb[10].iBitmap = 4;
        tbb[10].idCommand = TOOL_CUBE;
        tbb[10].fsState = TBSTATE_ENABLED;
        tbb[10].fsStyle = BTNS_BUTTON;

        // 5. Add Buttons to Toolbar & Auto-size
        SendMessage(hToolbar, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);
        SendMessage(hToolbar, TB_AUTOSIZE, 0, 0);

        return true;
        


    }
protected:
    PCWSTR GetClassName() const override { 
        return L"PaintWindowClass"; 
    }
    

    LRESULT OnNotify(int controlId, NMHDR* pNotificationHeader) override {
    // Check for Tooltip Request
    if (pNotificationHeader->code == TTN_GETDISPINFOW) {
        LPNMTTDISPINFOW pTTDI = (LPNMTTDISPINFOW)pNotificationHeader;

        // pTTDI->hdr.idFrom contains the ID of the toolbar button (e.g. TOOL_DOT)
        switch (pTTDI->hdr.idFrom) {
            case TOOL_DOT:
                pTTDI->lpszText = (LPWSTR)L"Намалювати крапку";
                break;
            case TOOL_LINE:
                pTTDI->lpszText = (LPWSTR)L"Намалювати лінію";
                break;
            case TOOL_RECT:
                pTTDI->lpszText = (LPWSTR)L"Намалювати прямокутник";
                break;
            case TOOL_ELIPSE:
                pTTDI->lpszText = (LPWSTR)L"Намалювати еліпс";
                break;
            case TOOL_CUBE:
                pTTDI->lpszText = (LPWSTR)L"Намалювати куб";
                break;
            case TOOL_CIRCLED_LINE:
                pTTDI->lpszText = (LPWSTR)L"Намалювати круголінію";
                break;
            default:
                pTTDI->lpszText = (LPWSTR)L"";
                break;
        }
        return 0;
    }


    return Window::OnNotify(controlId, pNotificationHeader);
    }

    // Перевизначення обробника команд
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override {
        if (eventCode == BN_CLICKED) {
            switch(id){
                case MENU_DOT:
                case MENU_ELIPSE:
                case MENU_LINE:
                case MENU_RECT:
                case MENU_CIRCLED_LINE:
                case MENU_CUBE:
                    is_expecting = true;
                    shape_id = id;
                    InvalidateRect(m_hwnd,NULL,TRUE);
                    break;
                case TOOL_DOT:
                case TOOL_ELIPSE:
                case TOOL_LINE:
                case TOOL_RECT:
                case TOOL_CIRCLED_LINE:
                case TOOL_CUBE:
                    is_expecting = true;
                    shape_id = id-BUTTON_AMOUNT-1;
                    InvalidateRect(m_hwnd,NULL,TRUE);
                    break;
            }
        }

        return Window::OnCommand(id, eventCode, controlHandle);
    }
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override{
        switch (msg) {
            case WM_LBUTTONDOWN:{
                if (shape_id == 0) return 0;
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);
                start_pos = Vector2{xPos,yPos};
                preview_shape = CreateShapeObject(Vector2{xPos,yPos},Vector2{xPos,yPos},shape_id);
                is_dragging = true;
                SetCapture(m_hwnd);
                InvalidateRect(m_hwnd, NULL, TRUE);
            return 0;
            }
            case WM_MOUSEMOVE: {
            // 2. TRACK: Only record movement if currently in a drag state
            if (is_dragging) {
                Vector2 pos;
                pos.x = GET_X_LPARAM(lParam);
                pos.y = GET_Y_LPARAM(lParam);

                preview_shape->set_second(pos);

                // Example: Trigger a window repaint to show a selection box or line
                InvalidateRect(m_hwnd, NULL, TRUE);
            }
            return 0;
            }
            case WM_LBUTTONUP: {
            // 3. FINISH: End tracking when button is released
            if (is_dragging) {
                is_dragging = false;
                is_expecting = false;
                
                // Always release capture when finished
                ReleaseCapture();

                // Final position when released
                int endX = GET_X_LPARAM(lParam);
                int endY = GET_Y_LPARAM(lParam);

                //delete preview_shape;
                //preview_shape = nullptr;

                //shapes[amount] = CreateShapeObject(start_pos,Vector2{endX,endY},shape_id);
                shapes[amount] = preview_shape;
                preview_shape = nullptr;
                amount+=1;
                // Perform final action (e.g., commit selection, drop object)

                InvalidateRect(m_hwnd, NULL, TRUE);
            }
            return 0;
            }

        }
        return Window::HandleMessage(msg, wParam, lParam);
    }
    void OnPaint() override {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);
        for(int i = 0; i<amount; i++){
            shapes[i]->draw(hdc);
        }
        if(preview_shape != nullptr){
            preview_shape->preview_draw(hdc);
        }
        EndPaint(m_hwnd, &ps);
    }
    
private:
    HICON icons[6];
    Vector2 start_pos;
    Shape* shapes[104];
    int amount = 0;
    int shape_id = 0;
    Shape* preview_shape = nullptr;
    bool is_expecting = false;
    bool is_dragging = false;
};

#endif