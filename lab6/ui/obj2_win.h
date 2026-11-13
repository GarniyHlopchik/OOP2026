#include "window.h"
#include <vector>
class OBJ2Window : public Window{
    public:
        HWND parent;
    private:
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override;
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    void OnPaint() override;
    std::vector<double> numbers;
};
