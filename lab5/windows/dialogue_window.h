#ifndef _DIALOGUE_WINDOW_H_
#define _DIALOGUE_WINDOW_H_
 
#include "window.h"
#include <vector>
 

class DialogueWindow : public Window {
public:
    void RebuildControls();
    DialogueWindow();
    ~DialogueWindow() override;
 
    // Створює вікно. Стилі WS_POPUP | WS_CAPTION | WS_SYSMENU додаються автоматично,
    // 'parent' використовується як власник (owner).
    bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                HWND parent = nullptr, HMENU menu = nullptr) override;
 
    // розмір задається для клієнтської області,
    // вікно центрується відносно власника і одразу показується.
    bool Open(PCWSTR title, int clientWidth, int clientHeight, HWND owner = nullptr);
 
    void Close();
    bool IsOpen() const;
    HWND GetOwner() const { return m_owner; }
 
    // Передає повідомлення всім відкритим діалогам (Tab, Enter, Esc...).
    // Повертає true, якщо повідомлення оброблено і його не треба диспетчеризувати далі.
    static bool IsDialogueMessage(MSG* msg);
 
protected:
    // Викликається з WM_CREATE: тут похідні класи створюють свої елементи керування.
    virtual void OnCreateControls();
 
    // Викликається після знищення вікна (WM_DESTROY).
    virtual void OnClosed();
 
    // Створює дочірній елемент керування зі стандартним шрифтом.
    HWND AddControl(PCWSTR className, PCWSTR text, DWORD style,
                    int x, int y, int width, int height, int id);
 
    PCWSTR  GetClassName() const override;
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override;
    void    OnDestroy() override;
 
private:
    HWND m_owner;
    static std::vector<DialogueWindow*> s_dialogues;
};
 
#endif // _DIALOGUE_WINDOW_H_
 