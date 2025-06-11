#pragma once
#include <string>

template<typename T, typename = void>
struct is_iterable : std::false_type{};

template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>>:
std::false_type{};

template<typename T>
struct is_string : std::false_type {};

template<>
struct is_string<std::string> : std::true_type {};

template<>
struct is_string<const char*> : std::true_type {};

template<>
struct is_string<char*> : std::true_type {};

// Detect if type has value_type
template<typename, typename = void>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

// Check if container holds strings
template<typename Container, typename = void>
struct iterable_contains_string : std::false_type {};

template<typename Container>
struct iterable_contains_string<Container, std::void_t<typename Container::value_type>>
    : is_string<typename Container::value_type> {};