#pragma once

#include "dialogue_window.h"
#include "../shapes/shape.h"

class Table : public DialogueWindow{
    protected:

    void OnCreateControls() override;
    LRESULT OnCommand(int id, int code, HWND h) override;

    public:

    void SetData(Shape** arr, int size);

    private:
    Shape** m_arr;
    int m_size;
};