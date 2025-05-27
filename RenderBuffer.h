#pragma once
#include <sys/ioctl.h> 
#include <iostream>
#include <vector>
class RenderBuff{
    private:
    int relativeHeight, relativeWidth; //terminal measurements, updated by getTerminalSize()
    int topline, bottomline;

    int getTerminalSize(); //used to calculate the terminal size
    public:
    bool updateBorderLines(int, int); //given absolute sizes (lines, columns) of struct, calculates the new topline & bottomline, returns true if change in terminal detected?

    void moveUp();
    void moveDown();

    void printBuffer(std::vector<std::string>&); //prints full buffer given render lines
    void printBufferLine(std::vector<std::string>&, int);

    int getTopLine();
};