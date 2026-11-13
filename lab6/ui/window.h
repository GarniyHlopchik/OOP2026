#ifndef _WINDOW_H_
#define _WINDOW_H_
 
#include <windows.h>
#include <commctrl.h>
 
class Window {
public:
    Window();
    virtual ~Window();
 
    // Метод створення вікна
    virtual bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                        int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                        HWND parent = nullptr, HMENU menu = nullptr);
 
    void Show(int cmdShow = SW_SHOW);
 
    HWND GetHandle() const { return m_hwnd; }
 
protected:
    // Головний диспетчер повідомлень класу
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
 
    // Обробник повідомлення WM_NOTIFY (Метод-член класу)
    virtual LRESULT OnNotify(int controlId, NMHDR* pNotificationHeader);
 
    // Обробник команд (кнопки, меню тощо)
    virtual LRESULT OnCommand(int id, int eventCode, HWND controlHandle);
 
    // Обробник малювання
    virtual void OnPaint();
 
    // Обробник закриття/знищення вікна
    virtual void OnDestroy();
 
    virtual PCWSTR GetClassName() const;
 
    HWND m_hwnd;
 
private:
    // Внутрішній статичний метод-місток для зв'язку WinAPI та C++ об'єкта
    static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
 
#endif // _WINDOW_H_