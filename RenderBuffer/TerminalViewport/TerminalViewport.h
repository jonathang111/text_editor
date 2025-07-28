#pragma once
#include "../TerminalCommands.h"
#include "../../general.h"
#include <atomic>
#include <vector>
#include <optional>

//figure out some way to avoid these gloabls;
extern std::atomic<int> delta;

enum Anchor{
    top = 1,
    bottom = 2
};
class TerminalViewport{
private:
    int relativeHeight = 0, relativeWidth = 0; //terminal measurements, updated by getTerminalSize()
    int topline = 0, bottomline = 0;
    //int distanceToTop, distanceToBottom;
    int previousHeight, previousWidth;
    Anchor currentAnchor = bottom;

    //functions used to grow viewable port using atomic detla.
    void grow(Cursor, int);
    void shrink(Cursor, int);

    //checks to see if anchor switch is needed, if it is, switch
    void shrinkAnchorCheck(Cursor, int, int);
    void growAnchorCheck(Cursor, int);
public:
    void UpdateSizeValues();
    void initalizeTerminal(std::vector<std::string>& buffer);
    //need to change names and make notes to clearly differentiate functions
    void updateRefresh(int, int, Cursor); //will replace the standard; will be more dynamic and should make the standard obsolete.
    bool updateStandard(int, int); //this needs to be replaced by a smarter function.

    //used for scroll logic, keeps in mind current location of cursor.
    void updateScroll(int, int, Cursor);

    //used on terminal resize, contains grow and shrink logic, anchor and cursor logic.
    void updateResize(int , int, Cursor, int); 

    void moveUp(); //not defined yet
    void moveDown();

    int getTopLine();
    int getBottomLine();
    int getRelativeHeight();
    int getRelativeWidth();
};