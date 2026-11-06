#ifndef _PAINT_WINDOW_H_
#define _PAINT_WINDOW_H_
 
#include "window.h"
#include "table.h"
#include "../shapes/shape.h"
#include "../create.h"
 
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
#define TOOL_CUBE    14
#define TOOL_BAR     15
#define MENU_TABLE   16
#define MENU_SAVE 17
#define MENU_LOAD 18
 
class PaintWindow : public Window {
public:
    PaintWindow(const PaintWindow&) = delete;
    PaintWindow& operator=(const PaintWindow&) = delete;
    ~PaintWindow() override;
 
    bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                HWND parent = nullptr, HMENU menu = nullptr) override;

    void DeleteShape(int index);
    void SelectShape(int index);
    void UnselectAll();

    static PaintWindow& getInstance();
 
protected:
    PCWSTR GetClassName() const override;
 
    LRESULT OnNotify(int controlId, NMHDR* pNotificationHeader) override;
 
    // Перевизначення обробника команд
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override;
 
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
 
    void OnPaint() override;
 
private:
    PaintWindow() = default;

    HICON icons[6] = {};
    Vector2 start_pos;
    Shape* shapes[104];
    int amount = 0;
    int shape_id = 0;
    Shape* preview_shape = nullptr;
    bool is_expecting = false;
    bool is_dragging = false;

    Table table;
};

#endif // _PAINT_WINDOW_H_