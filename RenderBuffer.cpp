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
        if (bottomline >=absolute_height) bottomline = absolute_height;
    }
    return ret;
}

void RenderBuff::updateBorderLinesResize(int , int, Cursor){

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
    std::cout << std::flush;
}

void RenderBuff::buildFrame(const std::vector<std::string>& buffer){
    frame << "\033[2J\033[H";
        for (int it = topline; it < bottomline; ++it){
        frame << buffer[it];
        if(it == bottomline - 1)
            continue;
        frame << '\n';
    }
}

void RenderBuff::buildFrameWithCursor(const std::vector<std::string>& buffer, Cursor cursor){
    //frame << "\033[2J\033[H"; //may need to remove, unless scrollback is removed.
    clearFrameBuffer();
    cursor.line += topline;
        for (int it = topline; it < bottomline; ++it){
            if(it == cursor.line-1){ //work on this
                std::string cursLine = buffer[it];
                frame << cursLine.substr(0, cursor.column-1);
                if(cursor.column < cursLine.size()){
                frame << "\033[7m" << cursLine.substr(cursor.column-1, 1) << "\033[0m"; 
                frame << cursLine.substr(cursor.column, cursLine.size());
                }
                else
                frame << "\033[7m" << " " << "\033[0m"; 
            }
            else
                frame << buffer[it];
            if(it == bottomline - 1)
                continue;
            frame << '\n';
    }
}

void RenderBuff::flushFrame(){
    std::cout << frame.str() << std::flush;
}

void RenderBuff::printBufferLine(std::vector<std::string>& buffer, int line){
    if(line > 0 && !(line > buffer.size())){
        std::cout << buffer[line];
    }
    else
        return;
}

void RenderBuff::clearFrameBuffer() {
    frame.str("");       // Clear the content of the stringstream
    frame.clear();       // Reset any error flags
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