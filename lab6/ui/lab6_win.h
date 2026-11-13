#include "window.h"
#include "dialogue.h"
#include "../send_data.h"
class MainWindow : public Window{
    private:
        PCWSTR GetClassName() const override{
            return L"LAB6MAIN";
        }
    Dialogue dialogue;
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override;
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    void OnDestroy() override;
    public:
    GenData data;
    bool Create(PCWSTR title, DWORD style, DWORD exStyle = 0,
                        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
                        int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                        HWND parent = nullptr, HMENU menu = nullptr) override;
};
