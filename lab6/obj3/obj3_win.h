#ifndef _OBJ3_WIN_H_
#define _OBJ3_WIN_H_

#include "window.h"
#include <vector>

class OBJ3Window : public Window {
public:
    OBJ3Window() : m_min(0), m_max(100) {}

protected:
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
    void    OnPaint() override;
    PCWSTR  GetClassName() const override;

private:
    bool LoadNumbersFromClipboard();
    void DrawChart(HDC hdc, const RECT& client) const;

    std::vector<double> m_values;
    int m_min;   // межі осі Y беруться зі структури GenData
    int m_max;
};

#endif // _OBJ3_WIN_H_
