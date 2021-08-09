#include <sstream>

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