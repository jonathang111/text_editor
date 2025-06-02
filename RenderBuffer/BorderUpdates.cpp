#include "RenderBuffer.h"

bool RenderBuff::updateBorderLines(int absolute_height , int absolute_width){ //return true if change in terminal size detected
    bool ret = getTerminalSize(); //possibly remove this conenction with getTerminalSize() and place into textfunctions instead. 
    if (relativeHeight >= absolute_height) {
        topline = 0;
        bottomline = absolute_height;
    } else {
        topline = absolute_height - relativeHeight;
        if (topline < 0) topline = 0;
        bottomline = topline + relativeHeight;
        if (bottomline >=absolute_height) bottomline = absolute_height;
    }
    return ret;
}

void RenderBuff::updateBorderLinesResize(int absolute_height, int absolute_width, Cursor cursor){
    //two cases, shrink or grow. both need distance calc
    getTerminalSize();
    distanceCalculations(cursor);
    
    //how can i determine whe the terminal is less than the VISIBLE current number of lines? i imagine just bottomline-topline, but perhaps not.
    if(previousHeight > relativeHeight &&  bottomline-topline > relativeHeight && !(relativeHeight < 1)){   //shrink condition;
        if(currentAnchor == bottom){
            if(distanceToTop == 0)  //i think this is saying distance to bottom = 0 
                bottomline--;
            else
                topline++;
        }
        else if(currentAnchor == top){
            if(distanceToTop > 0)
                bottomline--;
            else
                topline++;
        }
    }
    else{
    }
    previousHeight = relativeHeight;
}

void RenderBuff::updateBorderLinesCurs(int absolute_height, int absolute_width, Cursor cursor){
    getTerminalSize();
    if (cursor.line <= topline){ //cursor is above topline or at topline (topline is 0 indexed, cursor is 1 indexed)
        topline = cursor.line-1;
        bottomline = topline + relativeHeight;
    } else if (cursor.line >= bottomline){ //cursor is below bottomline
        bottomline = cursor.line + 1;
        topline = bottomline - relativeHeight;
    }

    //Clamps (double check these!)
    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
}

void RenderBuff::distanceCalculations(Cursor cursor){
    distanceToTop = cursor.line-1 - topline;
    distanceToBottom = bottomline - cursor.line;
}