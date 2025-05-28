#pragma once
#include <sys/ioctl.h> 
#include <iostream>
#include <vector>
#include "general.h"

class RenderBuff{
    private:
    int relativeHeight, relativeWidth; //terminal measurements, updated by getTerminalSize()
    int topline, bottomline;

    bool getTerminalSize(); //used to calculate the terminal size
    public:
    bool updateBorderLines(int, int); //given absolute sizes (lines, columns) of struct, calculates the new topline & bottomline, returns true if change in terminal detected?
    void updateBorderLinesCurs(int, int, Cursor);

    void moveUp();
    void moveDown();

    void printBuffer(std::vector<std::string>&); //prints full buffer given render lines
    void printBufferLine(std::vector<std::string>&, int);

    int getTopLine();
    int getBottomLine();
};