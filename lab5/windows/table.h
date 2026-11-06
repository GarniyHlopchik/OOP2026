#pragma once

#include "dialogue_window.h"
#include "../shapes/shape.h"

class Table : public DialogueWindow{
    protected:

    void OnCreateControls() override;
    LRESULT OnCommand(int id, int code, HWND h) override;
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

    public:

    void SetData(Shape** arr, int size);

    
    void SetupCallbacks(void (*on_select)(int row), void (*on_delete)(int row), void (*on_close)()){
        m_on_select = on_select;
        m_on_delete = on_delete;
        m_on_close = on_close;
    }

    private:
    Shape** m_arr;
    int m_size;
    void (*m_on_select)(int row);
    void (*m_on_delete)(int row);
    void (*m_on_close)();
};