#include "RenderBuffer.h"

std::atomic<int> delta = 0;
int internalDelta = 0;

bool RenderBuff::updateBorderLines(int absolute_height , int absolute_width){ //return true if change in terminal size detected
    bool ret = getTerminalSize();
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

void RenderBuff::updateBorderLinesRefresh(int absolute_height, int absolute_width, Cursor cursor){
    getTerminalSize();

    if(absolute_height );
}

void RenderBuff::updateBorderLinesResize(int absolute_height, int absolute_width, Cursor cursor){
    //two cases, shrink or grow. both need distance calc
    getTerminalSize();
    int ShrinkGrowCalculation = relativeHeight - previousHeight;
    
    //distanceCalculations(cursor);

    if(ShrinkGrowCalculation > 0)
        grow(cursor);
    else if(ShrinkGrowCalculation < 0 && bottomline-topline > relativeHeight) //need to add condition to not shrink is absolute < relative
        shrink(cursor);

    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
    if(topline != 0 && relativeHeight >= absolute_height){ //likely ioctl dsync issue? clamp is only fix rn
        topline = 0;
        bottomline = absolute_height;
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

    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
}

void RenderBuff::grow(Cursor cursor){
    int objectDelta = delta.exchange(0, std::memory_order_relaxed);
    if(currentAnchor == bottom){
        topline-=objectDelta;
    }
    else if(currentAnchor == top){
        bottomline-=objectDelta;
    }
}

void RenderBuff::shrink(Cursor cursor){
    int objectDelta = delta.exchange(0, std::memory_order_relaxed);
    if(currentAnchor == bottom){
        if(topline == cursor.line)
            bottomline-=objectDelta;
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