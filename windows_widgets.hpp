#ifndef _WINDOWS_WIDGET_H_
#define _WINDOWS_WIDGET_H_

#include "windows_form.hpp"
#include <functional>

class label_widget: public windows_widget
{
    public:
        label_widget(windows_form& parent, int x, int y, int nWidth, int nHeight, LPCTSTR lpWindowName = L"", DWORD dwExStyle = WS_EX_CLIENTEDGE, DWORD dwStyle = WS_CHILD | WS_VISIBLE);
        virtual ~label_widget();

    protected:

    private:
};

class button_widget: public windows_widget
{
    public:
        button_widget(windows_form& parent, int x, int y, int nWidth, int nHeight, std::function<void()> callback = [](){}, LPCTSTR lpWindowName = L"", DWORD dwExStyle = WS_EX_CLIENTEDGE, DWORD dwStyle = WS_CHILD | WS_VISIBLE);
        virtual ~button_widget();

        virtual void set_callback(std::function<void()> callback);

    protected:
        virtual LRESULT _process(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        std::function<void()> _callback;
    private:

};


#endif // _WINDOWS_WIDGET_H_
