#include "TextFunctions.h"

void Editor::printFrame(){ //for just printing the buffer, dynamic to window but not relative to cursor, should only be used for initial print
    Viewport.updateStandard(textBuffer.size(), 30); //this function is why initalization should be done with this frame. It is an absolute calculator for topline and bottomline
    FrameBuild.printBuffer(textBuffer, Viewport);
}

void Editor::dynamicPrint(){ //for printing relative to a cursor, for scrolling or refresh when cursor awareness is needed.
    Viewport.updateScroll(textBuffer.size(), 30, cursor);
    FrameBuild.ClearTerminalAndScrollback();
    FrameBuild.printBuffer(textBuffer, Viewport);
    refreshCurrentLine();
    //Need to move this print function to FramePrinting, and make it utilize the highlight char instead of the regular one
}

void Editor::printLine(int line){
    Viewport.updateScroll(textBuffer.size(), 30, cursor);
    FrameBuild.printBufferLine(textBuffer, line);
}

void Editor::refreshLineRel(int line){ //refreshed current line with correct cursor highlight. Relative line only
    int absoluteLine = Viewport.getTopLine()+line;
    moveCursorTo(line, 1);
    std::cout << "\33[2K" << std::flush;
    if(cursor.line == absoluteLine)
        FrameBuild.printCursorOnly(textBuffer, cursor, Viewport);
    else
        FrameBuild.printBufferLine(textBuffer, absoluteLine);
}

void Editor::refreshCurrentLine(){ //is this needed?
    Viewport.updateScroll(textBuffer.size(), 30, cursor);
    getRelativeCursor();
    moveCursorTo(relative_cursor.line, 1);
    FrameBuild.printCursorOnly(textBuffer, cursor, Viewport);
}