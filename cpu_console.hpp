#ifndef _CPU_CONSOLE_H_
#define _CPU_CONSOLE_H_

#ifndef UNICODE
#define UNICODE
#endif

#include "windows_widgets.hpp"

/*
**
** helpful links:
** https://devblogs.microsoft.com/oldnewthing/?p=1893
** https://stackoverflow.com/questions/41516594/createwindowex-lpparam-for-button-etc-in-wm-nccreate
** http://www.winprog.org/tutorial/app_one.html
**
*/

class cpu_console: public windows_form
{
    public:
        cpu_console(const wchar_t* window_title);
        virtual ~cpu_console();

    protected:
        virtual LRESULT _process(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        void _btn_push();
    private:

        static const wchar_t* class_name;
        windows_widget _txt_bx;
        label_widget _label;
        button_widget _button;
};

#endif // _CPU_CONSOLE_H_
