#include <sstream>
#include <cstdio>
#include <cstdarg>

template <typename BASE, typename VALUE>
BASE& __values_to_text(BASE& base, VALUE value){

    return static_cast<BASE&>(base << value);
}

template <typename BASE, typename VALUE, typename... VALUES>
BASE& __values_to_text(BASE& base, VALUE value, VALUES... values){
    return __values_to_text(static_cast<BASE&>(base << value), values...);
}

template <typename BASE, typename... VALUES>
auto values_to_text(VALUES... values){

    BASE base;

    return __values_to_text(base, values...).str();
}

template <typename... VALUES>
auto values_to_wstring(VALUES... values){

    return values_to_text<std::wstringstream, VALUES...>(values...);
}

template <typename... VALUES>
auto values_to_string(VALUES... values){

    return values_to_text<std::stringstream, VALUES...>(values...);
}

std::string format_string(const char* fmt, ...){

    const int initial_buffer_size = 512;
    char initial_buffer[initial_buffer_size];
    int size;
    char *buffer;
    std::string result;

    //start argument list
    va_list args;
    va_start (args, fmt);

    //attempt to vsnprintf if string is sufficiently small
    va_list cpy_args;
    va_copy(cpy_args, args);
    size = vsnprintf (initial_buffer, initial_buffer_size, fmt, cpy_args);
    va_end(cpy_args);


    if(size < 0){ //encoding error occured
        va_end (args); //remember to stop argument list
        throw std::runtime_error("encoding error");
    }

    if(size < initial_buffer_size){//string was sufficiently small, vsnprintf succeded
        va_end (args); //remember to stop argument list
        result.assign(initial_buffer);
        return result;
    }

    if(!(buffer = new char[size + 1])){
        va_end (args); //remember to stop argument list
        throw std::runtime_error("failed to allocate buffer");
    }

    size = vsnprintf (buffer, size + 1, fmt, args);
    va_end (args); //remember to stop argument list

    if(size < 0){ //encoding error occured
        delete[] buffer;
        throw std::runtime_error("encoding error");
    }

    result.assign(buffer);
    delete[] buffer;

    return result;
}