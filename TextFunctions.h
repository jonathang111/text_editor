#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "RenderBuffer.h"

typedef enum{
    none = 0,
    backspace = 1,
    space = 2,
    insert = 3,
    newline = 4
} Actions;

struct Delta{
    Actions action = none;
    int line = 0;
    int column = 0;
};

struct Cursor{
    int line = 0;
    int column = 0;
};

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
    //calculate longest column size
    public:  
    Editor(std::vector<std::string> ref, Cursor curs) : textBuffer(ref), cursor(curs){}

    void printCursorRelative();

    void printFrame();

    void printLine(int);

    void testRender();

    void ReadInput();
    
};
