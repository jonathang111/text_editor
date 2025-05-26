#include "RenderBuffer.h"

void RenderBuff::getTerminalSize(){ //static is temp;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    relativeHeight = w.ws_row;
    relativeWidth = w.ws_col;
}

void RenderBuff::moveUp(){
    topline--;
    bottomline--;
}

void RenderBuff::moveDown(){
    topline++;
    bottomline++;
}

void RenderBuff::updateRender(int absolute_height, int absolute_width){
    getTerminalSize();
    if(relativeHeight >= absolute_height){
        std::cout << "HIT if\n";
        topline = 0;
        bottomline = absolute_height;
        return;
    }
    else{
        std::cout << "HIT else\n";
        topline = absolute_height - relativeHeight;
        bottomline = topline + relativeHeight;
        return;
    }
}

void RenderBuff::printBuffer(std::vector<std::string>& buffer){
    int it = 0;
    it += topline;
    for(; it < bottomline; it++)
        std::cout << buffer[it] << std::endl;
}