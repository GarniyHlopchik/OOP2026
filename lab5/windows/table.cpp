#include "table.h"
#include <string>

void Table::OnCreateControls() {
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);

    HWND lv = AddControl(WC_LISTVIEWW, L"", LVS_REPORT | LVS_SINGLESEL | WS_BORDER | WS_TABSTOP,
                         10, 10, 500, 200, 200);
    ListView_SetExtendedListViewStyle(lv, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Columns
    LVCOLUMNW col = {};
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = 120; col.pszText = (LPWSTR)L"Фігура";  ListView_InsertColumn(lv, 0, &col);
    col.cx = 80;  col.pszText = (LPWSTR)L"X1";       ListView_InsertColumn(lv, 1, &col);
    col.cx = 80;  col.pszText = (LPWSTR)L"Y1";       ListView_InsertColumn(lv, 2, &col);
    col.cx = 80;  col.pszText = (LPWSTR)L"X2";       ListView_InsertColumn(lv, 3, &col);
    col.cx = 80;  col.pszText = (LPWSTR)L"Y2";       ListView_InsertColumn(lv, 4, &col);

    for(int i = 0; i < m_size; i++){
        // A row
        LVITEMW item = {};
        item.mask = LVIF_TEXT;
        item.iItem = 0;
        item.pszText = (LPWSTR)m_arr[i]->name();
        ListView_InsertItem(lv, &item);
                             // sets column 0
        
        ListView_SetItemText(lv, 0, 1, (LPWSTR)std::to_wstring(m_arr[i]->get_data().first.x).c_str());          // column 1
        ListView_SetItemText(lv, 0, 2, (LPWSTR)std::to_wstring(m_arr[i]->get_data().first.y).c_str());           // column 2
        ListView_SetItemText(lv, 0, 3, (LPWSTR)std::to_wstring(m_arr[i]->get_data().second.x).c_str());          // column 3
        ListView_SetItemText(lv, 0, 4, (LPWSTR)std::to_wstring(m_arr[i]->get_data().second.y).c_str());      //4
    }
    

    AddControl(L"BUTTON", L"OK", BS_DEFPUSHBUTTON | WS_TABSTOP, 50, 250, 70, 26, IDOK);
    AddControl(L"BUTTON", L"Скасувати", BS_PUSHBUTTON | WS_TABSTOP, 130, 250, 80, 26, IDCANCEL);
}

LRESULT Table::OnCommand(int id, int code, HWND h){
    if (id == IDOK) { /* apply settings */ Close(); return 0; }
    return DialogueWindow::OnCommand(id, code, h);
}

void Table::SetData(Shape** arr, int size){
    m_arr = arr;
    m_size = size;
}