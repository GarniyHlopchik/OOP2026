#include "obj3_win.h"
#include "../send_data.h"   // GenData
#include <cmath>
#include <cwchar>
#include <string>

PCWSTR OBJ3Window::GetClassName() const {
    return L"OBJ3WindowClass";   // окреме ім'я, щоб не конфліктувати з базовим класом
}

// Читає текст із буфера обміну й розбирає його в числа (розділювачі: пробіли, \r, \n, табуляція)
bool OBJ3Window::LoadNumbersFromClipboard() {
    // Буфер міг бути ще зайнятий іншим процесом, тому кілька спроб
    bool opened = false;
    for (int i = 0; i < 10 && !(opened = OpenClipboard(m_hwnd)); ++i) {
        Sleep(20);
    }
    if (!opened) return false;

    std::wstring text;
    HANDLE h = GetClipboardData(CF_UNICODETEXT);
    if (h) {
        const wchar_t* s = static_cast<const wchar_t*>(GlobalLock(h));
        if (s) {
            text = s;
            GlobalUnlock(h);
        }
    }
    CloseClipboard();

    if (text.empty()) return false;

    m_values.clear();
    const wchar_t* p = text.c_str();
    while (*p) {
        wchar_t* end = nullptr;
        double v = wcstod(p, &end);
        if (end == p) {      // не число, пропускаємо символ
            ++p;
            continue;
        }
        m_values.push_back(v);
        p = end;
    }
    return !m_values.empty();
}

LRESULT OBJ3Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COPYDATA: {
            COPYDATASTRUCT* pcds = reinterpret_cast<COPYDATASTRUCT*>(lParam);
            if (pcds && pcds->dwData == 1 && pcds->cbData == sizeof(GenData)) {
                // Копіюємо значення: вказівник дійсний лише під час обробки повідомлення
                GenData in = *static_cast<GenData*>(pcds->lpData);
                m_min = in.min;
                m_max = in.max;
                LoadNumbersFromClipboard();
                InvalidateRect(m_hwnd, nullptr, FALSE);
            }
            return TRUE;
        }
        case WM_SIZE:
            InvalidateRect(m_hwnd, nullptr, FALSE);
            return 0;

        case WM_ERASEBKGND:
            return 1;   // фон малюємо самі у буфері, щоб не мерехтіло

        case WM_PAINT:
            OnPaint();
            return 0;
    }
    return Window::HandleMessage(msg, wParam, lParam);
}

void OBJ3Window::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);

    RECT client;
    GetClientRect(m_hwnd, &client);
    int w = client.right, h = client.bottom;

    // Малюємо у пам'ять, потім одним блоком копіюємо на екран
    HDC mem = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
    HGDIOBJ oldBmp = SelectObject(mem, bmp);

    FillRect(mem, &client, (HBRUSH)GetStockObject(WHITE_BRUSH));
    DrawChart(mem, client);

    BitBlt(hdc, 0, 0, w, h, mem, 0, 0, SRCCOPY);

    SelectObject(mem, oldBmp);
    DeleteObject(bmp);
    DeleteDC(mem);
    EndPaint(m_hwnd, &ps);
}

void OBJ3Window::DrawChart(HDC hdc, const RECT& client) const {
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    SetBkMode(hdc, TRANSPARENT);

    const int left = 70, right = 25, top = 25, bottom = 45;
    RECT plot = { left, top, client.right - right, client.bottom - bottom };
    int plotW = plot.right - plot.left;
    int plotH = plot.bottom - plot.top;
    if (plotW < 50 || plotH < 50) return;   // вікно закрите замало

    TEXTMETRICW tm;
    GetTextMetricsW(hdc, &tm);

    if (m_values.empty()) {
        RECT r = client;
        DrawTextW(hdc, L"Немає даних", -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        return;
    }

    const int n = static_cast<int>(m_values.size());
    double yMin = m_min, yMax = m_max;
    if (yMax <= yMin) yMax = yMin + 1;   // захист від ділення на нуль
    const double yRange = yMax - yMin;

    auto toX = [&](double i) { return plot.left + static_cast<int>(std::lround(i / n * plotW)); };
    auto toY = [&](double v) { return plot.bottom - static_cast<int>(std::lround((v - yMin) / yRange * plotH)); };

    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(225, 225, 225));
    HPEN axisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HPEN linePen = CreatePen(PS_SOLID, 2, RGB(30, 100, 220));
    HGDIOBJ oldPen = SelectObject(hdc, gridPen);

    wchar_t buf[64];

    // ---- Вісь Y: позначки та горизонтальна сітка ----
    const int yDivs = 10;
    for (int i = 0; i <= yDivs; ++i) {
        double val = yMin + yRange * i / yDivs;
        int y = toY(val);

        SelectObject(hdc, gridPen);
        MoveToEx(hdc, plot.left, y, nullptr);
        LineTo(hdc, plot.right, y);

        SelectObject(hdc, axisPen);
        MoveToEx(hdc, plot.left - 5, y, nullptr);
        LineTo(hdc, plot.left, y);

        swprintf_s(buf, L"%.1f", val);
        RECT lr = { 0, y - tm.tmHeight / 2, plot.left - 8, y + tm.tmHeight / 2 + 1 };
        DrawTextW(hdc, buf, -1, &lr, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
    }

    // ---- Вісь X: індекси 0..n ----
    // Підписуємо кожен індекс; якщо вони не вміщаються, крок збільшується,
    // інакше підписи злипнуться.
    swprintf_s(buf, L"%d", n);
    SIZE sz;
    GetTextExtentPoint32W(hdc, buf, (int)wcslen(buf), &sz);
    double pxPerIndex = static_cast<double>(plotW) / n;
    int step = static_cast<int>(std::ceil((sz.cx + 6) / pxPerIndex));
    if (step < 1) step = 1;

    for (int i = 0; i <= n; ++i) {
        int x = toX(i);

        if (i % step == 0) {
            SelectObject(hdc, gridPen);
            MoveToEx(hdc, x, plot.top, nullptr);
            LineTo(hdc, x, plot.bottom);

            SelectObject(hdc, axisPen);
            MoveToEx(hdc, x, plot.bottom, nullptr);
            LineTo(hdc, x, plot.bottom + 5);

            swprintf_s(buf, L"%d", i);
            RECT lr = { x - 30, plot.bottom + 7, x + 30, plot.bottom + 7 + tm.tmHeight };
            DrawTextW(hdc, buf, -1, &lr, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX);
        }
    }

    // ---- Осі ----
    SelectObject(hdc, axisPen);
    MoveToEx(hdc, plot.left, plot.top, nullptr);
    LineTo(hdc, plot.left, plot.bottom);
    LineTo(hdc, plot.right, plot.bottom);

    // ---- Графік ----
    SelectObject(hdc, linePen);
    for (int i = 0; i < n; ++i) {
        double v = m_values[i];
        if (v < yMin) v = yMin;      // тримаємо точки в межах осі
        if (v > yMax) v = yMax;
        int x = toX(i);
        int y = toY(v);
        if (i == 0) MoveToEx(hdc, x, y, nullptr);
        else        LineTo(hdc, x, y);
    }

    HBRUSH dot = CreateSolidBrush(RGB(220, 40, 40));
    HGDIOBJ oldBrush = SelectObject(hdc, dot);
    for (int i = 0; i < n; ++i) {
        double v = m_values[i];
        if (v < yMin) v = yMin;
        if (v > yMax) v = yMax;
        int x = toX(i), y = toY(v);
        Ellipse(hdc, x - 3, y - 3, x + 4, y + 4);
    }

    // ---- Підписи осей ----
    RECT xl = { plot.left, client.bottom - tm.tmHeight - 4, plot.right, client.bottom - 2 };
    DrawTextW(hdc, L"індекс", -1, &xl, DT_CENTER | DT_SINGLELINE);
    TextOutW(hdc, 5, 5, L"значення", 8);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(dot);
    DeleteObject(gridPen);
    DeleteObject(axisPen);
    DeleteObject(linePen);
}
