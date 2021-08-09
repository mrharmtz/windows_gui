#ifndef _WINDOWS_FORM_H_
#define _WINDOWS_FORM_H_

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdexcept>
#include <map>
#include <string>

#define WIND_DEBUG

#ifdef WIND_DEBUG
#include <cstdio>

#define WIND_DEBUG_PRINTLINE(FORMAT) printf("%s[%5d]" FORMAT "\n", __func__, __LINE__)
#define WIND_DEBUG_FORMATLINE(FORMAT, ...) printf("%s[%5d]" FORMAT "\n", __func__, __LINE__, __VA_ARGS__)

#else

#define WIND_DEBUG_PRINTLINE(FORMAT)
#define WIND_DEBUG_FORMATLINE(FORMAT, ...)

#endif // WIND_DEBUG

class windows_form;

class windows_widget
{
public:
    windows_widget(windows_form& parent, LPCTSTR lpClassName, int x, int y, int nWidth, int nHeight, LPCTSTR lpWindowName = L"", DWORD dwExStyle = WS_EX_CLIENTEDGE, DWORD dwStyle = WS_CHILD | WS_VISIBLE);
    virtual ~windows_widget();
    BOOL set_position(HWND hWndInsertAfter, int  X, int  Y, int  cx, int  cy, UINT uFlags);
    virtual std::wstring get_class_name() const;
    virtual std::wstring get_text() const;
    virtual HDC get_hdc();

    virtual BOOL set_text(const std::wstring& text);
    
protected:
    friend class windows_form;
    virtual LRESULT _process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    windows_form& _parent;
    HWND _handler;
private:
    static const size_t MAX_ATTEMPTS;
    static const int START_BUFFER_SIZE;
};

class windows_form
{
public:
    windows_form(const wchar_t* class_name, const wchar_t* window_title);
    windows_form(const wchar_t* class_name, const wchar_t* window_title, DWORD dwExStyle, DWORD dwStyle, int X,int Y, int nWidth, int nHeight, HMENU hMenu);
    ~windows_form();

    virtual void show(int nCmdShow);
    virtual void run();
    virtual RECT& get_rect();
    virtual void set_background_color(COLORREF bkcolor);

protected:
    friend class windows_widget;
    void _create_window(DWORD dwExStyle, DWORD dwStyle, int X,int Y, int nWidth, int nHeight, HMENU hMenu);
    virtual LRESULT _process_window(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) final;
    virtual LRESULT _process(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void _add_widget(windows_widget& child);
    virtual size_t _remove_widget(windows_widget& child);
    
    const wchar_t* _class_name;
    const wchar_t* _window_title;
    RECT _rcClient;
    HWND _handler;
    HBRUSH _bkbrush;
private:
        
        std::map<HWND, windows_widget*> _children;
        static LRESULT CALLBACK static_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        static WNDCLASSEX _wc;
        static bool _registered;
        static void register_window(const wchar_t* class_name);
};

#endif //_WINDOWS_FORM_H_
