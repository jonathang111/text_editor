#pragma once
#include <sys/ioctl.h> 
#include <iostream>
#include <vector>
#include <sstream>
#include "general.h"

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


enum Anchor{
    topline = 1,
    bottomline = 2
};

class RenderBuff{
    private:
    int relativeHeight, relativeWidth; //terminal measurements, updated by getTerminalSize()
    int topline, bottomline;
    std::ostringstream frame;

    bool getTerminalSize(); //used to calculate the terminal size

    public:
    bool updateBorderLines(int, int); //given absolute sizes (lines, columns) of struct, calculates the new topline & bottomline; returns true if change in terminal detected?
    void updateBorderLinesCurs(int, int, Cursor); //used in dynamic resizing relative to cursor. (scrolling)
    void updateBorderLinesResize(int , int, Cursor); //used when resizing window, keeps in mind the position of the cursor when changing window size

    void buildFrame(const std::vector<std::string>&);
    void buildFrameWithCursor(const std::vector<std::string>& buffer, Cursor cursor);
    void flushFrame();
    void clearFrameBuffer();

    template<typename T>
    void addToFrame(T object){
        if constexpr (is_iterable<T>::value && iterable_contains_string<T>::value) {
            for (auto& it : object)
                frame << it;
        }
        else if constexpr (is_string<T>::value) {
                frame << object;
        }
    }

    void moveUp();
    void moveDown();

    void printBuffer(std::vector<std::string>&); //prints full buffer given render lines
    void printBufferLine(std::vector<std::string>&, int);

    int getTopLine();
    int getBottomLine();

    void clearTerminalAndScrollback();
    void enableAlternateScreen();
    void disableAlternateScreen();
};