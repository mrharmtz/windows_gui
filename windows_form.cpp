#include "windows_form.hpp"
#include "win_opcode.hpp"

extern"C"
IMAGE_DOS_HEADER __ImageBase;

HINSTANCE g_instance = reinterpret_cast<HINSTANCE>(&__ImageBase);


/************************************** windows_widget **************************************/


const size_t windows_widget::MAX_ATTEMPTS = 10;
const int windows_widget::START_BUFFER_SIZE = 10;

windows_widget::windows_widget(windows_form& parent, LPCTSTR lpClassName,  int x, int y
                        , int nWidth, int nHeight, LPCTSTR lpWindowName, DWORD dwExStyle, DWORD dwStyle)
:_parent(parent){ 
    if(!(this->_handler = CreateWindowEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, _parent._handler, 0, NULL, NULL))){
        WIND_DEBUG_FORMATLINE("failed to create widget \"%ls\", \"%ls\" with code %lu", lpClassName, lpWindowName, GetLastError());
        throw std::runtime_error("failed to create widget");
    }

    _parent._add_widget(*this);

    WIND_DEBUG_FORMATLINE("widget %ls created named %ls", lpClassName, lpWindowName);
}

windows_widget::~windows_widget(){ 
    _parent._remove_widget(*this);
}

LRESULT windows_widget::_process(UINT uMsg, WPARAM wParam, LPARAM lParam){

    return _parent._process(uMsg, wParam, lParam);
}

BOOL windows_widget::set_position(HWND hWndInsertAfter, int  x, int  y, int  cx, int  cy, UINT uFlags){

    return SetWindowPos(this->_handler, hWndInsertAfter, x, y, cx, cy, uFlags);
}

std::wstring windows_widget::get_class_name() const{

    int buffer_size = windows_widget::START_BUFFER_SIZE, status = 0;
    size_t attempts = 0;
    WCHAR *buffer = 0;
    std::wstring result;

    do
    {
        if(!(buffer = new WCHAR[buffer_size + 1]))
            throw std::runtime_error("failed to allocate buffer");

        if(!(status = GetClassNameW(_handler, buffer, buffer_size + 1))){
            ++attempts;
            buffer_size *= 2;
        }else{
            result.assign(buffer);
        }

        delete[] buffer;

        if(attempts > windows_widget::MAX_ATTEMPTS)
            throw std::runtime_error("max attempts reached");

    } while (!status);
    
    return result;
}

std::wstring windows_widget::get_text() const{

    int length, status;
    WCHAR *buffer;
    std::wstring result;

    SetLastError(0);

    if(!(length = GetWindowTextLengthW(_handler)))
        throw std::runtime_error("failed to get text length, due to either not having any, or another reason");

    if(!(buffer = new WCHAR[length + 1]))
        throw std::runtime_error("failed to allocate buffer");

    if(!(status = GetWindowTextW(_handler, buffer, length + 1)))
        throw std::runtime_error("failed to get handler string");

    result.assign(buffer);

    delete[] buffer;

    return result;
}

HDC windows_widget::get_hdc(){
    return GetDC(_handler);
}

BOOL windows_widget::set_text(const std::wstring& text){
    return SetWindowTextW(_handler, text.c_str());
}

/************************************** windows_widget **************************************/
/************************************** windows_form **************************************/

WNDCLASSEX windows_form::_wc;
bool windows_form::_registered = false;

LRESULT CALLBACK windows_form::static_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    windows_form *pThis = NULL; // our "this" pointer will go here
    //WIND_DEBUG_FORMATLINE("hwnd at %p, message = %u", hwnd, uMsg);
    if (uMsg == WM_NCCREATE) {
        WIND_DEBUG_PRINTLINE("WM_NCCREATE");
        // Recover the "this" pointer which was passed as a parameter
        // to CreateWindow(Ex).
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<windows_form*>(lpcs->lpCreateParams);
        pThis->_handler = hwnd;
        // Put the value in a safe place for future use
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    } else {
        // Recover the "this" pointer from where our WM_NCCREATE handler
        // stashed it.
        pThis = reinterpret_cast<windows_form*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pThis) {
        // Now that we have recovered our "this" pointer, let the
        // member function finish the job.

        return pThis->_process_window(hwnd, uMsg, wParam, lParam);
    }else{
        WIND_DEBUG_FORMATLINE("unknwon source at %p, message = %u", hwnd, uMsg);
    }
	 // We don't know what our "this" pointer is, so just do the default
	 // thing. Hopefully, we didn't need to customize the behavior yet.
	 return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void windows_form::register_window(const wchar_t* class_name){

    if(windows_form::_registered)
        return ;

    memset(&(windows_form::_wc), 0, sizeof(windows_form::_wc));

    windows_form::_registered = true;
    windows_form::_wc.hInstance = g_instance;
    windows_form::_wc.lpszClassName = class_name;
    windows_form::_wc.lpfnWndProc = windows_form::static_wndproc;
    windows_form::_wc.style =  CS_DBLCLKS;
    windows_form::_wc.cbSize = sizeof(WNDCLASSEX);
    windows_form::_wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    windows_form::_wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    windows_form::_wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    windows_form::_wc.lpszMenuName = NULL;                 /* No menu */
    windows_form::_wc.cbClsExtra = 0;                      /* No extra bytes after the window class */
    windows_form::_wc.cbWndExtra = 0;                      /* structure or the window instance */
    windows_form::_wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND; /* Use Windows's default colour as the background of the window */
    if(!RegisterClassEx(&windows_form::_wc))
        throw std::runtime_error("failed to register window class");
}

windows_form::windows_form(const wchar_t* class_name, const wchar_t* window_title)
:_class_name(class_name), _window_title(window_title), _bkbrush(NULL)
{
    WIND_DEBUG_FORMATLINE("windows_form: %p", this);
    windows_form::register_window(class_name);
}

windows_form::windows_form(const wchar_t* class_name, const wchar_t* window_title, DWORD dwExStyle, DWORD dwStyle, int X,int Y, int nWidth, int nHeight, HMENU hMenu)
:_class_name(class_name), _window_title(window_title), _bkbrush(NULL)
{
    windows_form::register_window(class_name);
    _create_window(dwExStyle, dwStyle, X,Y, nWidth, nHeight, hMenu);
}

void windows_form::_create_window(DWORD dwExStyle, DWORD dwStyle, int X,int Y, int nWidth, int nHeight, HMENU hMenu){
    WIND_DEBUG_FORMATLINE("windows_form: %p", this);
    if(!(this->_handler = CreateWindowEx(
        0,
        this->_class_name,                     // Window class
        this->_window_title,            // Window text
        dwStyle,                        // Window style

        // Size and position
        X, Y, nWidth, nHeight,

        HWND_DESKTOP,       // Parent window
        hMenu,       // Menu
        g_instance,  // Instance handle
        this        // Additional application data
        )))
            throw std::runtime_error("error failed to create window");
}

LRESULT windows_form::_process_window(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

    std::map<HWND, windows_widget*>::iterator child;

    if((child = _children.find((HWND)wParam)) != _children.end()){
         WIND_DEBUG_FORMATLINE("%ls: %-30s; wParam(%30ls), lParam(%30lld)", this->_window_title, wmcode_to_str(uMsg), child->second->get_class_name().c_str(), lParam);
        return child->second->_process(uMsg, wParam, lParam);
    }
    
    if ((child = _children.find((HWND)lParam)) != _children.end()){
        WIND_DEBUG_FORMATLINE("%ls: %-30s; wParam(%30llu), lParam(%30ls)", this->_window_title, wmcode_to_str(uMsg), wParam, child->second->get_class_name().c_str());
        return child->second->_process(uMsg, wParam, lParam);
    }
    
    WIND_DEBUG_FORMATLINE("%ls: %-30s; wParam(%30llu), lParam(%30lld)", this->_window_title, wmcode_to_str(uMsg), wParam, lParam);
    return _process(uMsg, wParam, lParam);
}

LRESULT windows_form::_process(UINT uMsg, WPARAM wParam, LPARAM lParam){
    return DefWindowProc(_handler, uMsg, wParam, lParam);
}

void    windows_form::_add_widget(windows_widget& child){

    _children[child._handler] = &child;
}

size_t windows_form::_remove_widget(windows_widget& child){

    return _children.erase(child._handler);
}

windows_form::~windows_form()
{ }

RECT& windows_form::get_rect(){
    
    GetClientRect(_handler, &_rcClient);
    return _rcClient;
}

void windows_form::show(int nCmdShow){

    ShowWindow(this->_handler, nCmdShow);
}

void windows_form::run(){

    MSG msg = { };
    while (GetMessage(&msg, this->_handler, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void windows_form::set_background_color(COLORREF bkcolor){

    if(_bkbrush)
        DeleteObject(_bkbrush);

    _bkbrush = CreateSolidBrush(bkcolor);
    SetClassLongPtrW(_handler, GCLP_HBRBACKGROUND, (LONG_PTR)_bkbrush);
    InvalidateRect(_handler, NULL, TRUE);
}

/************************************** windows_form **************************************/