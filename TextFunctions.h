#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "general.h"
#include "RenderBuffer.h"

//when accessing renderbuffer functions, always ensure you update the relative size first.
class Editor{
    private:
    std::vector<std::string> textBuffer;
    std::vector<std::string> line;
    std::vector<Delta> undo;
    std::vector<Delta> redo;
    Cursor cursor, relative_cursor;
    RenderBuff render;

    void UpdateCursor();

    void InsertChar(char);

    void Backspace();

    void NewLine();
    
    void getRelativeCursor();

    void stripLastChar(int);
    //calculate longest column size
    public:  
    Editor(std::vector<std::string> ref, Cursor curs) : textBuffer(ref), cursor(curs){}

    void printCursorRelative();

    void printFrame();

    void dynamicPrint();

    void printLine(int);

    void testRender();

    void ReadInput();

};
