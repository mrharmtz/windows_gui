#include <iostream>
#include <thread>
#include <future>
#include "cpu_console.hpp"
#include "utilities.hpp"

using namespace std;

/*
#define PUT_LINE_0(value)              cout << "line 0:" << value << endl
#define PUT_LINE_1(value)              cout << "line 1:" << value << endl

#define GET_LAST_ARG(..., arg ) arg
#define PRINT_STRING_MACRO_CHOOSER(...) GET_LAST_ARG(PUT_LINE_0, __VA_ARGS__)

#define PRINT_STRING(...) PRINT_STRING_MACRO_CHOOSER(__VA_ARGS__)
*/

int searchLowestMSB(unsigned int value){

        int result = 32;

        if(!value) return -1;

        while(!(value & 0x80000000)){
            value = value << 1;
            --result;
        }

        while(value & 0x80000000){
            value = value << 1;
            --result;
        }

        return result;
    }

    int searchHighstLSB(unsigned int value){

        int result = 0;

        if(!value) return -1;

        while(!(value & 0x1)){ value = value >> 1; ++result; }

        while(value & 0x1){ value = value >> 1; ++result; }

        return result;
    }


void flip(uint8_t* start, unsigned size){

    uint8_t *end = start + size - 1, temp;

    while(end > start){
        temp = *start;
        *(start++) = *end;
        *(end--) = temp;
    }

}

void test_something(){
/*int value = 0xe0;
    cout << value << " highest lsb is " << searchHighstLSB(value) << endl;
    cout << value << " lowest msb is " << searchLowestMSB(value) << endl;*/


    uint8_t arr[] = {1,2,3};

    printf("before arr = ");

    for(int i = 0 ; i < sizeof(arr) ; ++i)
        printf("%d, ", arr[i]);

    printf("\n");

    flip(arr, sizeof(arr));

    printf("after arr = ");

    for(int i = 0 ; i < sizeof(arr) ; ++i)
        printf("%d, ", arr[i]);

    printf("\n");

    /*int length = 5;
    int address = 0;

    for(; length ; --length, ++address){
        printf("address %d\n", address);
    }*/
}

void run_console(const wchar_t* window_name){

    cpu_console console(window_name);

    console.show(SW_SHOWNORMAL);
    console.run();
}

int* simplefunc(std::string a)
{
    for(int  i = 0 ; i < 10 ; ++i){
        cout << "thread: " << i << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    int *value = new int;
    *value = a.size();
    return value;
}

template<typename RET_TYPE>
void print_future_status(std::future<RET_TYPE>& result){

    std::future_status fstat;

    try{
        fstat = result.wait_for(std::chrono::seconds(0));
    }catch(...){
        cout << "fstat = exception" << endl;
        return;
    }

    switch(fstat){

    case std::future_status::deferred:
        cout << "fstat = deferred(" << (int)fstat << ")" << endl;
        break;
    case std::future_status::ready:
        cout << "fstat = ready(" << (int)fstat << ")" << endl;
        break;
    case std::future_status::timeout:
        cout << "fstat = timeout(" << (int)fstat << ")" << endl;
        break;
    default:
        cout << "fstat = unknown(" << (int)fstat << ")" << endl;
    }
}

int main(int argc, char * const argv[])
{
    std::future<int*> future;

    print_future_status(future);

    run_console(L"console");
    /*std::thread console_thread_0(run_console, L"console 0");
    std::thread console_thread_1(run_console, L"console 1");*/

    //future = std::async(simplefunc, "hello world");

    print_future_status(future);

    /*cout << "main: " << (long long)(1e9 * 0.05) << endl;
    while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready){
        std::cout << '.';
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    print_future_status(future);
    //int *simple = future.get();
    print_future_status(future);*/
    /*try{
        simple = future.get();
    }catch(...){
        cout << "get value already called" << endl;
    }

    cout << "simple(" << simple << ") = " << *simple << endl;

    delete simple;*/

    /*console_thread_0.join();
    console_thread_1.join();*/

    return 0;
}
