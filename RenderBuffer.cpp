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
    topline--;
    bottomline--;
}

void RenderBuff::moveDown(){ //simiarly, move down should never be more than absolute. 
    topline++;
    bottomline++;
}

bool RenderBuff::updateBorderLines(int absolute_height , int absolute_width){ //return true if change in terminal size detected
    bool ret = getTerminalSize(); //possibly remove this conenction with getTerminalSize() and place into textfunctions instead. 
    if (relativeHeight >= absolute_height) {
        topline = 0;
        bottomline = absolute_height;
    } else {
        topline = absolute_height - relativeHeight;
        if (topline < 0) topline = 0;
        bottomline = topline + relativeHeight;
        if (bottomline > absolute_height) bottomline = absolute_height;
    }
    return ret;
}

void RenderBuff::updateBorderLinesCurs(int absolute_height, int absolute_width, Cursor cursor){
    getTerminalSize();
    if (cursor.line <= topline){ //cursor is above topline or at topline (topline is 0 indexed, cursor is 1 indexed)
        topline = cursor.line-1;
        bottomline = topline + relativeHeight;
    } else if (cursor.line >= bottomline){ //cursor is below bottomline
        bottomline = cursor.line + 1;
        topline = bottomline - relativeHeight;
    }

    //Clamps (double check these!)
    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
}

void RenderBuff::printBuffer(std::vector<std::string>& buffer){
    for (int it = topline; it < bottomline; ++it){
        std::cout << buffer[it];
        if(it == bottomline - 1)
            continue;
        std::cout << std::endl;
    }
}

void RenderBuff::printBufferLine(std::vector<std::string>& buffer, int line){
    if(line > 0 && !(line > buffer.size())){
        std::cout << buffer[line];
    }
    else
        return;
}

int RenderBuff::getTopLine(){
    return topline;
}

int RenderBuff::getBottomLine(){
    return bottomline;
}