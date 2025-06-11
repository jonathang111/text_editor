#include "FramePrinting.h"

void FrameBuilder::printBuffer(std::vector<std::string>& buffer, TerminalViewport term){
    for (int it = term.getTopLine(); it < term.getBottomLine(); ++it){
        std::cout << buffer[it];
        if(it == term.getTopLine() - 1)
            continue;
        std::cout << std::endl;
    }
    std::cout << std::flush;
}

void FrameBuilder::printBufferLine(std::vector<std::string>& buffer, int line){
    if(line > 0 && !(line > buffer.size())){
        std::cout << buffer[line];
    }
    else
        return;
}

void FrameBuilder::buildFrame(const std::vector<std::string>& buffer, TerminalViewport term){
    frame << "\033[2J\033[H";
        for (int it = term.getTopLine(); it < term.getBottomLine(); ++it){
        frame << buffer[it];
        if(it == term.getBottomLine() - 1)
            continue;
        frame << '\n';
    }
}

void FrameBuilder::buildFrameWithCursor(const std::vector<std::string>& buffer, Cursor cursor, TerminalViewport term){
    //frame << "\033[2J\033[H"; //may need to remove, unless scrollback is removed.
    clearFrameBuffer();
    cursor.line += term.getTopLine();
    for (int it = term.getTopLine(); it < term.getBottomLine(); ++it){
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
        if(it == term.getBottomLine() - 1){
            //frame << "relative height: " << relativeHeight << "previous height"  << previousHeight << " topline: " << topline;
            continue;
        }
        frame << '\n';
    }
}

void FrameBuilder::ClearTerminalAndScrollback(){
    std::cout << "\x1b" "c" << std::flush;
}

void FrameBuilder::flushFrame(){
    std::cout << frame.str() << std::flush;
}

void FrameBuilder::clearFrameBuffer() {
    frame.str("");
    frame.clear();
}

void FrameBuilder::debug(int input, TerminalViewport term){
    frame << "relative Height: " << term.getRelativeHeight() << " absolute Height: " << input;
    frame << " topline: " << term.getTopLine() << " bottomline: " << term.getBottomLine();
}