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

class Editor{
    private:
    std::vector<std::string> textBuffer;
    std::vector<std::string> line;
    std::vector<Delta> undo;
    std::vector<Delta> redo;
    Cursor cursor;

    void UpdateCursor();

    void InsertChar(char);

    void Backspace();

    void NewLine();

    public:  
    Editor(std::vector<std::string> ref, Cursor curs) : textBuffer(ref), cursor(curs){}

    RenderBuff render;

    void testRender();

    void ReadInput();
    
};
