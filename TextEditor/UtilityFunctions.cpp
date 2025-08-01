#include "TextFunctions.h"

void Editor::InitalizeTerminal(){
    original = TermUtil::DefaultTerminalInitalization();
    Viewport.UpdateSizeValues();
    Viewport.updateStandard(textBuffer.size(), 30); //change 30 to actual width later
    FrameBuild.ClearTerminalAndScrollback();
    FrameBuild.printBuffer(textBuffer, Viewport);
}

//For resizing with specific terminal anchor logic. 
void Editor::TerminalResize(){ //need to exchange atomic here. Logic no longer tied in Viewport
    int CurrentDelta = delta.exchange(0, std::memory_order_relaxed);
    FrameBuild.ClearTerminalAndScrollback();
    Viewport.updateResize(textBuffer.size(), 30, cursor, CurrentDelta);
    getRelativeCursor();
    FrameBuild.buildFrameWithCursor(textBuffer, relative_cursor, Viewport);
    //FrameBuild.debug(textBuffer.size());
    FrameBuild.flushFrame();
}

void Editor::getRelativeCursor(){ //position of cursor relative to window.
   relative_cursor.line = cursor.line - Viewport.getTopLine();
   relative_cursor.column = cursor.column;
}

void Editor::moveCursorTo(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H" << std::flush;
}

void Editor::shiftCursorUp(){
    if(cursor.line > 0)
        cursor.line--;
    if(cursor.line <= Viewport.getTopLine()){
        FrameBuild.ClearTerminalAndScrollback();
        Viewport.updateScroll(textBuffer.size(), 30, cursor);
        getRelativeCursor();
        FrameBuild.buildFrameWithCursor(textBuffer, relative_cursor, Viewport);
        //FrameBuild.debug(textBuffer.size());
        FrameBuild.flushFrame();
    }
    else{
    refreshLineRel(relative_cursor.line);
    refreshCurrentLine();
    }
}

void Editor::shiftCursorDown(){
    if(cursor.line <= textBuffer.size())
        cursor.line++;
    if(cursor.line >= Viewport.getBottomLine()){
        FrameBuild.ClearTerminalAndScrollback();
        Viewport.updateScroll(textBuffer.size(), 30, cursor);
        getRelativeCursor();
        FrameBuild.buildFrameWithCursor(textBuffer, relative_cursor, Viewport);
        //FrameBuild.debug(textBuffer.size());
        FrameBuild.flushFrame();
    }
    else{
    refreshLineRel(relative_cursor.line);
    refreshCurrentLine();
    }
}

/*if reading from file, there is likely a \r for some reason, you need to strip or else you get "ghosting"
this is probably the most frustrating bug ive encountered for this program. just because you don't realize its a probelm until you run the debugger
and analyze the vector contents.*/