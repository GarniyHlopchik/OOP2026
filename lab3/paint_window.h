#ifndef _PAINT_WINDOW_H_
#define _PAINT_WINDOW_H_

#include "window.h"
#include "shapes/shape.h"
#include "create.h"

#define MENU_FILE    1
#define MENU_DOT     2
#define MENU_LINE    3
#define MENU_RECT    4
#define MENU_ELIPSE  5
#define MENU_ABOUT   6
#define TOOL_DOT     7
#define TOOL_LINE    8
#define TOOL_RECT    9
#define TOOL_ELIPSE  10
#define TOOL_BAR     11

class PaintWindow : public Window {
public:
    bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                HWND parent = nullptr, HMENU menu = nullptr) override{
        
        //building menu
        HMENU hMenu = CreateMenu();
        AppendMenuW(hMenu, MF_STRING, MENU_FILE, L"Файл");
        HMENU hActionMenu = CreatePopupMenu();
        AppendMenuW(hActionMenu, MF_STRING, MENU_DOT, L"Крапка");
        AppendMenuW(hActionMenu, MF_STRING, MENU_LINE, L"Лінія");
        AppendMenuW(hActionMenu, MF_STRING, MENU_RECT, L"Прямокутник");
        AppendMenuW(hActionMenu, MF_STRING, MENU_ELIPSE, L"Еліпс");
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

        // 3. Add Standard System Images (Cut, Copy, Paste, New, Open, Save, etc.)
        TBADDBITMAP tbAddBitmap;
        tbAddBitmap.hInst = HINST_COMMCTRL;
        tbAddBitmap.nID = IDB_STD_SMALL_COLOR;
        SendMessage(hToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbAddBitmap);

        // 4. Define Toolbar Buttons
        TBBUTTON tbb[7] = { 0 };

        // Button 1: Dot
        tbb[0].iBitmap = STD_FILENEW;
        tbb[0].idCommand = TOOL_DOT;
        tbb[0].fsState = TBSTATE_ENABLED;
        tbb[0].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[1].fsStyle = BTNS_SEP;

        // Button 2: Line
        tbb[2].iBitmap = STD_PROPERTIES;
        tbb[2].idCommand = TOOL_LINE;
        tbb[2].fsState = TBSTATE_ENABLED;
        tbb[2].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[3].fsStyle = BTNS_SEP;

        // Button 3: Rect
        tbb[4].iBitmap = STD_FILENEW;
        tbb[4].idCommand = TOOL_RECT;
        tbb[4].fsState = TBSTATE_ENABLED;
        tbb[4].fsStyle = BTNS_BUTTON;

        // Separator
        tbb[5].fsStyle = BTNS_SEP;

        // Button 3: Elipse
        tbb[6].iBitmap = STD_FIND;
        tbb[6].idCommand = TOOL_ELIPSE;
        tbb[6].fsState = TBSTATE_ENABLED;
        tbb[6].fsStyle = BTNS_BUTTON;

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
                    is_expecting = true;
                    shape_id = id;
                    InvalidateRect(m_hwnd,NULL,TRUE);
                    break;
                case TOOL_DOT:
                case TOOL_ELIPSE:
                case TOOL_LINE:
                case TOOL_RECT:
                    is_expecting = true;
                    shape_id = id-5;
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

                delete preview_shape;
                preview_shape = nullptr;

                shapes[amount] = CreateShapeObject(start_pos,Vector2{endX,endY},shape_id);
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
    Vector2 start_pos;
    Shape* shapes[104];
    int amount = 0;
    int shape_id = 0;
    Shape* preview_shape = nullptr;
    bool is_expecting = false;
    bool is_dragging = false;
};

#endif