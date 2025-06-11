#pragma once
#include <sstream>
#include <iostream>
#include <vector> //possibly replace the vec inputs with type traits instead
#include "../general.h"
#include "TerminalViewport/TerminalViewport.h"
#include "Templates.h"

class FrameBuilder{
    private:
    std::ostringstream frame;
    
    public:
    void printBuffer(std::vector<std::string>&, TerminalViewport);
    void printBufferLine(std::vector<std::string>&, int);

    void buildFrame(const std::vector<std::string>& buffer, TerminalViewport);
    void buildFrameWithCursor(const std::vector<std::string>& buffer, Cursor cursor, TerminalViewport);

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

    void ClearTerminalAndScrollback();

    void flushFrame();
    void clearFrameBuffer();

    void debug(int input, TerminalViewport);
};