#include "paint_window.h"
#include <windowsx.h> // GET_X_LPARAM / GET_Y_LPARAM
#include <iostream>
#include <commdlg.h>
#include <string>
#include "../shapes/dot.h"
#include "../shapes/line.h"
#include "../shapes/rect.h"
#include "../shapes/elipse.h"
#include "../shapes/cube.h"
#include "../shapes/circled_line.h"

struct FileElement{
    std::wstring name;
    int x1;
    int x2;
    int y1;
    int y2;
};


bool WriteTextFile(const std::wstring& path, const std::wstring& text) {
    // wstring -> UTF-8 bytes
    int need = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(),
                                   nullptr, 0, nullptr, nullptr);
    std::string utf8(need, '\0');
    if (need > 0) {
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), (int)text.size(),
                            &utf8[0], need, nullptr, nullptr);
    }

    HANDLE h = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;

    // UTF-8 BOM so Notepad and Excel recognize the encoding (optional, my reader handles both)
    const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
    DWORD written = 0;
    BOOL ok = WriteFile(h, bom, sizeof(bom), &written, nullptr);

    if (ok && !utf8.empty()) {
        ok = WriteFile(h, utf8.data(), (DWORD)utf8.size(), &written, nullptr)
             && written == utf8.size();
    }
    CloseHandle(h);
    return ok;
}
bool PickSavePath(HWND owner, std::wstring& outPath) {
    wchar_t path[MAX_PATH] = L"";

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = owner;
    ofn.lpstrFilter = L"Text (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
    ofn.lpstrFile   = path;
    ofn.nMaxFile    = MAX_PATH;
    ofn.lpstrDefExt = L"txt";   // appended if the user types a name without an extension
    ofn.Flags       = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (!GetSaveFileNameW(&ofn)) return false;
    outPath = path;
    return true;
}
bool ReadFileBytes(const std::wstring& path, std::vector<unsigned char>& out) {
    HANDLE h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) return false;

    LARGE_INTEGER size;
    if (!GetFileSizeEx(h, &size) || size.QuadPart > 0xFFFFFFFFLL) {   // keep it simple: < 4 GB
        CloseHandle(h);
        return false;
    }

    out.resize((size_t)size.QuadPart);
    DWORD bytesRead = 0;
    BOOL ok = out.empty() || ReadFile(h, out.data(), (DWORD)out.size(), &bytesRead, nullptr);
    CloseHandle(h);
    return ok && bytesRead == out.size();
}
bool PickFile(HWND owner, std::wstring& outPath) {
    wchar_t path[MAX_PATH] = L"";

    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = owner;
    ofn.lpstrFilter = L"Text (*.txt)\0*.txt\0All files (*.*)\0*.*\0";  // pairs of "label\0pattern\0"
    ofn.lpstrFile   = path;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;

    if (!GetOpenFileNameW(&ofn)) return false;   // user pressed Cancel (or an error occurred)
    outPath = path;
    return true;
}
static bool Convert(UINT codePage, DWORD flags, const char* s, int len, std::wstring& out) {
    int need = MultiByteToWideChar(codePage, flags, s, len, nullptr, 0);
    if (need <= 0) return false;
    out.resize(need);
    MultiByteToWideChar(codePage, flags, s, len, &out[0], need);
    return true;
}
std::wstring BytesToWString(const std::vector<unsigned char>& bytes) {
    const unsigned char* p = bytes.data();
    size_t n = bytes.size();
    if (n == 0) return L"";

    // UTF-16 LE with BOM (what Windows Notepad calls "Unicode")
    if (n >= 2 && p[0] == 0xFF && p[1] == 0xFE) {
        return std::wstring((const wchar_t*)(p + 2), (n - 2) / 2);
    }

    // UTF-8, with or without BOM
    size_t skip = (n >= 3 && p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) ? 3 : 0;
    std::wstring result;
    if (Convert(CP_UTF8, MB_ERR_INVALID_CHARS, (const char*)p + skip, (int)(n - skip), result)) {
        return result;
    }

    // Not valid UTF-8: assume the system ANSI code page (e.g. cp1251 on a Ukrainian system)
    Convert(CP_ACP, 0, (const char*)p, (int)n, result);
    return result;
}
 
PaintWindow::~PaintWindow() {
    for (int i = 0; i < 6; i++) {
        DestroyIcon(icons[i]);
    }
}

PaintWindow& PaintWindow::getInstance() {
    static PaintWindow instance;
    return instance;
}

bool PaintWindow::Create(PCWSTR title, DWORD style, DWORD exStyle,
                         int x, int y, int width, int height,
                         HWND parent, HMENU menu)
{
    //load icons
    icons[0] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
    icons[1] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
    icons[2] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
    icons[3] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
    icons[4] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
    icons[5] = LoadIconFromFile(L".\\icons\\ellipse.bmp", 64, 64);
 
    //building menu
    HMENU hMenu = CreateMenu();
    //AppendMenuW(hMenu, MF_STRING, MENU_FILE, L"Файл");
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenuW(hFileMenu, MF_STRING, MENU_SAVE, L"Зберегти");
    AppendMenuW(hFileMenu, MF_STRING, MENU_LOAD, L"Відкрити");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"Меню");
    HMENU hActionMenu = CreatePopupMenu();
    AppendMenuW(hActionMenu, MF_STRING, MENU_DOT, L"Крапка");
    AppendMenuW(hActionMenu, MF_STRING, MENU_LINE, L"Лінія");
    AppendMenuW(hActionMenu, MF_STRING, MENU_RECT, L"Прямокутник");
    AppendMenuW(hActionMenu, MF_STRING, MENU_ELIPSE, L"Еліпс");
    AppendMenuW(hActionMenu, MF_STRING, MENU_CIRCLED_LINE, L"Круголінія");
    AppendMenuW(hActionMenu, MF_STRING, MENU_CUBE, L"Куб");
    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hActionMenu, L"Об'єкти");
    AppendMenuW(hMenu, MF_STRING, MENU_ABOUT, L"Довідка");
    AppendMenuW(hMenu, MF_STRING, MENU_TABLE, L"Таблиця");
    if (!Window::Create(title, style, exStyle, x, y, width, height, parent, hMenu)) { return false; }
 
    // 1. Initialize Common Controls
    INITCOMMONCONTROLSEX icex = { 0 };
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
 
    // 2. Create Toolbar Window
    HWND hToolbar = CreateWindowExW(
        0,
        TOOLBARCLASSNAMEW,
        NULL,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0,
        m_hwnd,
        (HMENU)TOOL_BAR,
        GetModuleHandle(NULL),
        NULL
    );
 
    if (!hToolbar) return false;
 
    // Send size structure for compatibility
    SendMessageW(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
 
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
    SendMessageW(hToolbar, TB_SETIMAGELIST, 0, (LPARAM)hMyImageList);
 
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
    SendMessageW(hToolbar, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);
    SendMessageW(hToolbar, TB_AUTOSIZE, 0, 0);
 
    return true;
}
 
PCWSTR PaintWindow::GetClassName() const {
    return L"PaintWindowClass";
}
 
LRESULT PaintWindow::OnNotify(int controlId, NMHDR* pNotificationHeader) {
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
 
LRESULT PaintWindow::OnCommand(int id, int eventCode, HWND controlHandle) {
    if (eventCode == BN_CLICKED) {
        switch (id) {
            case MENU_SAVE:{
                std::wstring save_path;
                bool result = PickSavePath(m_hwnd,save_path);
                if(result){
                    std::wstring text_to_write;
                    for(int i = 0; i< amount; i++){
                        text_to_write += shapes[i]->name();
                        text_to_write += L",";
                        auto data = shapes[i]->get_data();
                        text_to_write += std::to_wstring(data.first.x) + L"," +std::to_wstring(data.first.y) + L"," + std::to_wstring(data.second.x) + L","+std::to_wstring(data.second.y) + L",\r\n";
                    }
                    bool result = WriteTextFile(save_path, text_to_write);
                    
                }
                break;
            }
            case MENU_LOAD:{
                std::wstring load_path;
                bool result = PickFile(m_hwnd, load_path);
                if(result){
                    std::vector<unsigned char> bytes;
                    bool result = ReadFileBytes(load_path, bytes);
                    if(result){
                        std::wstring file_contents = BytesToWString(bytes);
                        std::vector<FileElement> data_rows;
                        std::wstring buffer;
                        FileElement el;
                        int filled = 0;
                        for(auto c : file_contents){
                            if(c == ','){
                                switch (filled)
                                {
                                case 0:
                                    el.name = buffer;
                                    break;
                                case 1:
                                    el.x1 = stoi(buffer);
                                    break;
                                case 2:
                                    el.y1 = stoi(buffer);
                                    break;
                                case 3:
                                    el.x2 = stoi(buffer);
                                    break;
                                case 4:
                                    el.y2 = stoi(buffer);
                                    break;
                                default:
                                    break;
                                }
                                filled+=1;
                                buffer = L"";
                            }else if(c == '\r'){}
                            else if(c == '\n'){
                                data_rows.push_back(el);
                                filled = 0;
                            }
                            else{
                                buffer += c;
                            }
                        }
                        amount = 0;
                        for(auto el : data_rows){
                            if(el.name == L"Прямокутник"){
                                shapes[amount] = new Rect(Vector2{el.x1, el.y1}, Vector2{el.x2, el.y2});
                            }else if(el.name == L"Лінія"){
                                shapes[amount] = new Line(Vector2{el.x1, el.y1}, Vector2{el.x2, el.y2});
                            }else if(el.name == L"Еліпс"){
                                shapes[amount] = new Elipse(Vector2{el.x1, el.y1}, Vector2{el.x2, el.y2});
                            }else if(el.name == L"Точка"){
                                shapes[amount] = new Dot(Vector2{el.x1, el.y1});
                            }else if(el.name == L"Куб"){
                                shapes[amount] = new Cube(Vector2{el.x1, el.y1}, Vector2{el.x2, el.y2});
                            }else{
                                shapes[amount] = new CircledLine(Vector2{el.x1, el.y1}, Vector2{el.x2, el.y2});
                            }
                            amount+=1;
                        }
                    }
                }
                InvalidateRect(m_hwnd,NULL,TRUE);
                break;
            }
            case MENU_TABLE:
                table.SetData(shapes, amount);
                table.Open(L"Таблиця", 600, 300, m_hwnd);
                break;
            case MENU_DOT:
            case MENU_ELIPSE:
            case MENU_LINE:
            case MENU_RECT:
            case MENU_CIRCLED_LINE:
            case MENU_CUBE:
                is_expecting = true;
                shape_id = id;
                InvalidateRect(m_hwnd, NULL, TRUE);
                break;
            case TOOL_DOT:
            case TOOL_ELIPSE:
            case TOOL_LINE:
            case TOOL_RECT:
            case TOOL_CIRCLED_LINE:
            case TOOL_CUBE:
                is_expecting = true;
                shape_id = id - BUTTON_AMOUNT - 1;
                InvalidateRect(m_hwnd, NULL, TRUE);
                break;
        }
    }
 
    return Window::OnCommand(id, eventCode, controlHandle);
}
 
LRESULT PaintWindow::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_LBUTTONDOWN: {
            if (shape_id == 0) return 0;
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            start_pos = Vector2{xPos, yPos};
            preview_shape = CreateShapeObject(Vector2{xPos, yPos}, Vector2{xPos, yPos}, shape_id);
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
                amount += 1;
                // Perform final action (e.g., commit selection, drop object)
 
                InvalidateRect(m_hwnd, NULL, TRUE);
            }
            return 0;
        }
    }
    return Window::HandleMessage(msg, wParam, lParam);
}
 
void PaintWindow::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);
    for (int i = 0; i < amount; i++) {
        shapes[i]->draw(hdc);
    }
    if (preview_shape != nullptr) {
        preview_shape->preview_draw(hdc);
    }
    EndPaint(m_hwnd, &ps);
}
 