#pragma once
#include <sys/ioctl.h> 
#include <iostream>
#include <vector>
class RenderBuff{
    private:
    int relativeHeight, relativeWidth; //terminal measurements, updated by getTerminalSize()
    int topline, bottomline;

    void getTerminalSize(); //used to calculate the terminal size
    void getBorderLines();
    public:
    void updateRender(int, int);
    void moveUp();
    void moveDown();
    void printBuffer(std::vector<std::string>&);
};