#include "cpu_console.hpp"
#include "win_opcode.hpp"

const wchar_t* cpu_console::class_name = L"cpu window";

cpu_console::cpu_console(const wchar_t* window_title)//WS_EX_CLIENTEDGE,"Button","Append",WS_CHILD | WS_VISIBLE | ES_CENTER, 10, 41,75,30,hMainWindow,NULL,NULL,NULL
:windows_form(cpu_console::class_name, window_title, 0, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL)
,_txt_bx(*this, L"EDIT", 0, 0, 100, 100, L"", WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL)
,_label(*this, 20, 20, 300, 25, L"This is a button:")
,_button(*this, 0, 0, 100, 100, std::bind(&cpu_console::_btn_push, this), L"Push")
{
    WIND_DEBUG_FORMATLINE("cpu_console: %p", this);
}

cpu_console::~cpu_console()
{ }

void cpu_console::_btn_push(){
    
    set_background_color(RGB(255, 255, 0));
    _label.set_text(L"color should change");
}

LRESULT cpu_console::_process(UINT uMsg, WPARAM wParam, LPARAM lParam){

    switch (uMsg)
    {
    case WM_SIZE:
    {
        RECT& rcClient = this->get_rect();

        _txt_bx.set_position(NULL, 0, 0, rcClient.right, rcClient.bottom/2, SWP_NOZORDER);
        _button.set_position(NULL, rcClient.right/2, rcClient.bottom/2, rcClient.right/2, 25, SWP_NOZORDER);
        _label.set_position(NULL, 0, rcClient.bottom/2, rcClient.right/2, 25, SWP_NOZORDER);
    }
    return 0;
    case WM_DESTROY:
        this->_handler = NULL;
        PostQuitMessage(0);
        return 0;
    }

    return windows_form::_process(uMsg, wParam, lParam);
}
