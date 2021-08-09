#include "windows_widgets.hpp"
#include "windowsx.h"

label_widget::label_widget(windows_form& parent, int x, int y, int nWidth, int nHeight, LPCTSTR lpWindowName, DWORD dwExStyle, DWORD dwStyle)
:windows_widget(parent, L"STATIC", x, y, nWidth, nHeight, lpWindowName, dwExStyle, dwStyle){
    
    //EnableWindow()
}

label_widget::~label_widget()
{
    //dtor
}


button_widget::button_widget(windows_form& parent, int x, int y, int nWidth, int nHeight,std::function<void()> callback, LPCTSTR lpWindowName, DWORD dwExStyle, DWORD dwStyle)
:windows_widget(parent, L"BUTTON", x, y, nWidth, nHeight, lpWindowName, dwExStyle, dwStyle)
,_callback(callback)
{ }

button_widget::~button_widget(){ }

void button_widget::set_callback(std::function<void()> callback){
    _callback = callback;
}

LRESULT button_widget::_process(UINT uMsg, WPARAM wParam, LPARAM lParam){

    switch(uMsg){
        case WM_COMMAND:
            _callback();
            return 0;
    }

    return windows_widget::_process(uMsg, wParam, lParam);
}