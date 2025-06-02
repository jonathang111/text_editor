#include "RenderBuffer.h"

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
            if(it == bottomline - 1){
                frame << "relative height: " << relativeHeight << "previous height"  << previousHeight;
                continue;
            }
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
    frame.str("");
    frame.clear();
}