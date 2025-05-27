#include "RenderBuffer.h"

int RenderBuff::getTerminalSize(){ //return 1 if change detected, else 0;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    if(relativeHeight == w.ws_row || relativeWidth == w.ws_col)
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
    int i = getTerminalSize();
    if(i == 1){
        if(relativeHeight >= absolute_height){
            topline = 0;
            bottomline = absolute_height;
        }
        else{
            topline = absolute_height - relativeHeight;
            bottomline = topline + relativeHeight;
            //std::cout << "Topline: " << topline << " Bottom line: " << bottomline << " Relative Height: " << relativeHeight << std::endl;
        }
        return false; //possibly change to false for only topline or bottom line change?
    }
    else
        return true;
}

void RenderBuff::printBuffer(std::vector<std::string>& buffer){
    for (int it = topline; it < bottomline; ++it){
        std::cout << it << buffer[it];
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