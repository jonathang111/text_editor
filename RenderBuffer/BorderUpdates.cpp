#include "RenderBuffer.h"

std::atomic<int> delta = 0;
int internalDelta = 0;

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
    internalDelta = relativeHeight - previousHeight;
    objectDelta = delta.exchange(0, std::memory_order_relaxed);
    
    //distanceCalculations(cursor);

    if(internalDelta > 0)
        grow(cursor);
    if(internalDelta < 0 && bottomline-topline > relativeHeight) //need to add condition to not shrink is absolute < relative
        shrink(cursor);

    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
    if(topline != 0 && relativeHeight >= absolute_height){
        topline = 0;
        bottomline = absolute_height;
    }

    previousHeight = relativeHeight;

    //old logic using distance, delta is needed for actual smooth computation at high resize speeds
    //how can i determine whe the terminal is less than the VISIBLE current number of lines? i imagine just bottomline-topline, but perhaps not.
    // if(previousHeight > relativeHeight &&  bottomline-topline > relativeHeight && !(relativeHeight < 1)){   //shrink condition;
    //     if(currentAnchor == bottom){
    //         if(distanceToTop == 0)  //i think this is saying distance to bottom = 0 
    //             bottomline--;
    //         else
    //             topline++;
    //     }
    //     else if(currentAnchor == top){
    //         if(distanceToTop > 0)
    //             bottomline--;
    //         else
    //             topline++;
    //     }
    // }
    // else if(previousHeight < relativeHeight && !(relativeHeight > absolute_height)){
    //     if(currentAnchor == bottom){
    //         topline--;
    //     }
    //     else if(currentAnchor == top){
    //         bottomline--;
    //     }
    // }
    // if(topline < 0) topline = 0;
    // if (bottomline > absolute_height) bottomline = absolute_height;
    // if (bottomline - topline > relativeHeight) {
    //     topline = bottomline - relativeHeight;
    // }
    //previousHeight = relativeHeight;
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

void RenderBuff::distanceCalculations(Cursor cursor){ //redundant
    // distanceToTop = cursor.line-1 - topline;
    // distanceToBottom = bottomline - cursor.line;
} //regardless of increase or decrease, if anchored to bottom, you increase or decrease top.

void RenderBuff::grow(Cursor cursor){
    if(currentAnchor == bottom){
        topline-=objectDelta;
    }
    else if(currentAnchor == top){
        bottomline-=objectDelta;
    }
}

void RenderBuff::shrink(Cursor cursor){
    if(currentAnchor == bottom){
        if(topline == cursor.line)
            bottomline-=objectDelta; //change to internal delta.
        else
            topline+=objectDelta;
    }
    else if(currentAnchor == top){
        if(topline != cursor.line)
            bottomline-=objectDelta;
        else
            topline+=objectDelta;
    }
}