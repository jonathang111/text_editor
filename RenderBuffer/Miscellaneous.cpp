#include "RenderBuffer.h"

bool RenderBuff::getTerminalSize(){ //return 1 if change detected, else 0;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    if(relativeHeight == w.ws_row && relativeWidth == w.ws_col)
        return 0;
    relativeHeight = w.ws_row;
    relativeWidth = w.ws_col;
    return 1;
}

void RenderBuff::moveUp(){ //add checks so that topline never surpasses 1
    
}

void RenderBuff::moveDown(){ //simiarly, move down should never be more than absolute. 
}

void RenderBuff::initalizeTerminal(std::vector<std::string>& buffer){
    getTerminalSize();
    updateBorderLines(buffer.size(), 30); //change 30 to actual width later
    previousHeight = relativeHeight;
    printBuffer(buffer);
}

int RenderBuff::getTopLine(){
    return topline;
}

int RenderBuff::getBottomLine(){
    return bottomline;
}

void RenderBuff::clearTerminalAndScrollback(){
    std::cout << "\x1b" "c" << std::flush;
}

void RenderBuff::enableAlternateScreen(){
    std::cout << "\x1b[?1049h" << std::flush;
}

void RenderBuff::disableAlternateScreen() {
    std::cout << "\x1b[?1049l" << std::flush;
}