#include "TerminalViewport.h"
int internalDelta = 0;

void TerminalViewport::UpdateSizeValues(){ //possible return bool too?
    auto ret = TermUtil::GetTerminalSize();
    if(ret){
        previousHeight = relativeHeight;
        previousWidth = relativeWidth;
        auto NewValues = *ret;
        relativeHeight = NewValues.first;
        relativeWidth = NewValues.second;
    }
    else
        std::cerr << "Failure in terminal resize\n";
}

bool TerminalViewport::updateStandard(int absolute_height , int absolute_width){ //return true if change in terminal size detected
    UpdateSizeValues();
    if(relativeHeight >= absolute_height){
        topline = 0;
        bottomline = absolute_height;
    }else {
        topline = absolute_height - relativeHeight;
        if(topline < 0) topline = 0;
        bottomline = topline + relativeHeight;
        if(bottomline >=absolute_height) bottomline = absolute_height;
    }
    return false; //alter this, return should mean something
}

void TerminalViewport::updateRefresh(int absolute_height, int absolute_width, Cursor cursor){
    UpdateSizeValues();

    if(absolute_height );
}

void TerminalViewport::updateResize(int absolute_height, int absolute_width, Cursor cursor, int delta){
    //two cases, shrink or grow. both need distance calc
    UpdateSizeValues();
    int ShrinkGrowCalculation = relativeHeight - previousHeight;
    /* Given the size of the terminal subtracted by the previous height,
    if the difference is more than 0, then it must be a growth since current height > previous height;
    else if negative, then previous height must be larger than current. */

    //distanceCalculations(cursor);
    if(ShrinkGrowCalculation > 0){
        grow(cursor, delta);
        growAnchorCheck(cursor, absolute_height);
    }
    else if(ShrinkGrowCalculation < 0 && bottomline-topline > relativeHeight){ //need to add condition to not shrink if absolute < relative
        shrink(cursor, delta);
        shrinkAnchorCheck(cursor, absolute_height, 2);
    }

    //clamps
    if(topline < 0) 
        topline = 0;

    if(bottomline > absolute_height) 
        bottomline = absolute_height;

    if(bottomline - topline > relativeHeight) 
        topline = bottomline - relativeHeight;

    if(topline != 0 && relativeHeight >= absolute_height){ //likely ioctl dsync issue? clamp is only fix rn
        topline = 0;
        bottomline = absolute_height;
    }

    previousHeight = relativeHeight;
}

void TerminalViewport::updateScroll(int absolute_height, int absolute_width, Cursor cursor){
    UpdateSizeValues();
    if (cursor.line <= topline){ //cursor is above topline or at topline (topline is 0 indexed, cursor is 1 indexed)
        topline = cursor.line-1;
        bottomline = topline + relativeHeight;
    } else if (cursor.line > bottomline){ //cursor is below bottomline
        bottomline = cursor.line + 1;
        topline = bottomline - relativeHeight;
    }

    if (topline < 0) topline = 0;
    if (bottomline > absolute_height) bottomline = absolute_height;
    if (bottomline - topline > relativeHeight) {
        topline = bottomline - relativeHeight;
    }
}

void TerminalViewport::grow(Cursor cursor, int delta){
    int objectDelta = delta;
    if(currentAnchor == bottom){
        topline -= objectDelta;
    }
    else if(currentAnchor == top){
        bottomline += objectDelta;
    }
}

void TerminalViewport::shrink(Cursor cursor, int delta){
    int objectDelta = delta;
    if(currentAnchor == bottom){
        if(topline == cursor.line) //shirnk but cursor is top of screen
            bottomline -= objectDelta;
        else
            topline += objectDelta;
    }
    else if(currentAnchor == top){
        if(topline != cursor.line)
            bottomline-=objectDelta;
        else
            topline+=objectDelta;
    }
}

void TerminalViewport::shrinkAnchorCheck(Cursor cursor, int absolute_height, int absolute_width){
    if(cursor.line == topline + 1 || cursor.line == bottomline - 1){
        if(cursor.line == topline + 1)
            currentAnchor = top;
        else
            currentAnchor = bottom;
    }

    // if(currentAnchor != top && topline == 1 && bottomline != absolute_height)
    //     currentAnchor = top;
    // else if(currentAnchor != bottom && bottomline == absolute_height && topline != 1)
    //     currentAnchor = bottom;

    return;
}

void TerminalViewport::growAnchorCheck(Cursor cursor, int absolute_height){
    if(currentAnchor == top && bottomline == absolute_height && topline != 1)
    currentAnchor = bottom;
}