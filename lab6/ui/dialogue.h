#ifndef _DIALOGUE_H_
#define _DIALOGUE_H_

#include "dialogue_window.h"
class MainWindow;
class Dialogue : public DialogueWindow {
public:
    MainWindow* parent_ptr;
protected:
    void    OnCreateControls() override;
    LRESULT OnCommand(int id, int eventCode, HWND controlHandle) override;

private:
    enum ControlId {
        IDC_EDIT_N = 101,
        IDC_EDIT_MIN,
        IDC_EDIT_MAX
    };

    void OnOk();
    bool ReadInt(int controlId, int& value) const;
};

#endif
