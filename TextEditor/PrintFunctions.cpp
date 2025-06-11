#include "TextFunctions.h"

void Editor::printFrame(){ //for just printing the buffer, dynamic to window but not relative to cursor, should only be used for initial print
    Viewport.updateStandard(textBuffer.size(), 30); //this function is why initalization should be done with this frame. It is an absolute calculator for topline and bottomline
    FrameBuild.printBuffer(textBuffer, Viewport);
}

void Editor::dynamicPrint(){ //for printing relative to a cursor, for scrolling or refresh when cursor awareness is needed.
    Viewport.updateScroll(textBuffer.size(), 30, cursor);
    FrameBuild.ClearTerminalAndScrollback();
    FrameBuild.printBuffer(textBuffer, Viewport);
    //FrameBuild.clearFrameBuffer();
    //FrameBuild.flushFrame();
    printCursorRelative();
}

void Editor::printLine(int line){
    Viewport.updateScroll(textBuffer.size(), 30, cursor);
    FrameBuild.printBufferLine(textBuffer, line-1);
}

void Editor::printCursorRelative(){
    getRelativeCursor();
    std::cout << "\033[" << relative_cursor.line << ";" <<relative_cursor.column << "H" << std::flush;
}