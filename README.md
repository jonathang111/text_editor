# text_editor
a simple text editor

DONT RUN ON YOUR VSCODE TERMINAL, electron slows down the program, causing a laggy appearance.

Personal Notes:
Buffer only flushes on newline, fills up, or if we explicitly flush it.
Need to strategically place the flushes. if you flush too much then you get weird visual artifacts; too little then same problem.
for some reason the entire things is laggy if cursor is not at the bottom.

it took about a million and half years to make "addToFrame" function accept a dynamic amount of obejects. but it worked eventually. and it sucked.
It still doesn't fix the fact that moving the cursor anywhere besides the last line causes visual artifacts.
Likely the terminal needs to calculate and redraw the specific line with the cursor there, i.e. exactly what im doing except now we have a virtualized two step layer here.
Best solution would be to instead of telling the terminal to redraw, i do the cursor drawing and treat the cursor as a characther to be placed into the buffer.

the issue is the scrollback buffer thingy. after growing enough printing to terminal becomes very taxing on the system causing visual glitches. not sure what a fix would be.

After timing the render, it seems like the problem was never the rendering of the lines. rather something else that i can't identity. Perhaps I/O? maybe the refresh rate.

turns out after hours of trying to figure it out, the issue is not anything software related. the issue is that the terminal in vscode runs on in Electron and has more overhead for printing
if i wanted to get that smooth id implement some GPU acceleration. I was only able to discover this when i tried running nano to compare the refresh speed and realized they also suffered from "lag" when resizing.
So then i ran the program in a dedicated terminal and bam, its actually fast.

lesson learned, the enviornment the software runs on is far more important to its perceptible performance than i initially thought. 

next steps are to implement a frame system for all other functions that require a full screen refresh. and for all render funcitons they should allow for the custome cursor logic.

then i need to add a wrapping feature, a shit and drag feature, as well as multiple char deletion and replacement.

Then finally a save feature for saving, as well as loading specific cursor locations and storing them too.

need to fix the down cursor logic when updating terminal

Theres no possible way to drag up and expand up then drag down and expand down. you need to choose and anchor.

OK so when i update the winodw resize logic i need it to work in this manner
-first assume i have function that can calculate the distance from cursor to topline/bottomline
-second assume there is a specified "anchor" size, i.e. top, bottom, or even cursor.


1. create two branches, one for shrink, one for grow.

For the shrink, if the distance is not 0, then continue to change using distance from cursor to calculate the new topline or bottomline

One thing is that we can alaways infer the direction of the windows "drag" by considering that we start the document with the cursor at the bottom of the document. we start at the bottom, so assume any shrink is directed from top to down, i.e. increase topline
Else, if we are not at a border, we always assume the anchor of the most previous border that was 0. for example, if i started at distanceFromBottom = 0, then i assume shrink from top, if distanceFromTop = 0 then assume shrink from bellow.
Though im not sure if this is the best approach, or if it should stay as a static preference that always shrink from top when available, then switch to bttom once distanceFromTop = 0, of it the anchor side should switch based on last known distance = 0.

so if the user is scrolling up, then of course the distance is 0 for the topline and thus any shrink switch to a topline anchor, even if they scroll a little bit down, but if they were scrolling down then assume a bottom line anchor

Right now i think a major issue is that SIGWINCH does not interrupt another signal. And since the function is running in the signal handling, then when a window is resized fast enough, it causes an incorrect print. This also explain why it would cause seg fault since likely it added one more or less in the terminalResize function, thus accessing an out of range index.

There are a few fixes ive been able to find. First is self-pipe, basically just a mini queue where we use a read and write stream to write and read to some pipe which lets us know if a process was requested. This lets us always capture the last signal, which is ideal in every case.

The second, and likely the one i will implement, is called debouncing. Basically you use a signal, set some bool, and instead of instantly trying to run terminalResize(), you instead wait a bit for another signal change. This introduces some latency, but it could also be seen as a good thing since it takes off some load on the CPU and also lets us always capture that last one. Plus we could additionally control the frame rate using that, though id have to calculate what the true frame rate would be since the loop rate and the handling rate would be added together.

Refactor has been completed. After some realization that RenderBuffer had too many responsibilities, i split it up into more modular files. TextEditor is still the same except that it holds the main logic loop and is in charge of settings up the terminal enviornment. 

Current folders inlcude /RengerBuffer, which holds two main classes, FramePrinting, and TerminalViewport. TerminalViewport updates the borderlines and holds the logic for figuring out the topline/bottomline of the visible lines given a buffer. FramPrinting just prints the TerminalViewport. The idea is that you pass in an object to print it.

Additionally, the folder includes various headers and header definition files used for manipulating the terminal. Such as eneabling an alternate screen, setting and disabling various terminal options (termios), and clearing the screen (alternate terminal specific clearing).

The other folder is /TextEditor which includes the higher level class that is interacted with in main. Currently the only usable functions are debuggers, various prints (for testing) and most importantly, start(). The start() function automatically deals with the terminal settings, as well as the termianl alternate. Future plans include implementing a flag like setting that is able to set various options, such as debugging, different terminal settings, etc.

Current Offset Realizations:
- Current line is offset by 1 from the cursor, so current line is cursor.line-1.
- Text file test has 21 lines exactly.
Currently need to work on:

COMPLETE:
- TextFunction contains a printCursorRelative; this should not be printing a frame or any layout-related content. In fact, this should be the responsibility of frame printing, not the TextFunction itself.
    - Currently, I’m removing the cursor-relative logic and replacing it with an abstracted function in the editor that uses renderer.printCursorOnly. This method prints only the current line with the cursor highlighted. I can use this instead.
        - FIXNOTES: Done. Plus, frambuilder was renamed to renderer. PrintFunctions in Editor contains RefreshLine() now.
- Typing causes the highlight to appear on top of the current character instead of in front of it. i.e., it overwrites the character instead of showing the cursor before it.
    - Look into how insert-character mode moves the cursor.
    - FIXENOTES: Prolbem was just that column was added AFTER the line refresh, which wouldn't update it properly before refreshing the line. Just moved the column update to before refreshline was called.
- If cursor is at the end of the line when shifted up or down, it is not deleted, however, if in middle, it is correctly refreshed.
    - FIXNOTES: Problem was the that while yes, the line was being refreshed, it was not being deleted, meaning that if the cursor was at the end, i.e. vector[line].size()+1 column, then the new refresh would not even reach that end cursor point to delete it. Solution was just to add a clear line code in refreshLineRel();

Printing does work relatively, but:
- There’s a problem refreshing the previous line properly.
    - It also doesn’t handle printing correctly when the terminal is not at full size.
    - When trying to clear the previous line, we are currently printing the line that the cursor is currently one, which is wrong.
        - Probably because we use the refreshline function, which uses the cursors position even if we explictily state to reprint a specific line. To fix this, lets just use a printLine only function.
            - Fixed, problem was with deciding which line to fix, and also reworked "printBufferLine" function to not need manual subtraction of 1 from the current line to print x buffer.
    -FIXNOTES: First, the terminal now works with resizing, the anchor issue is not yet placed, but do not that it may already be fixed by the time anyone reads this, these little fix notes are not updated. Line is properly printed, this was fixed by creating shift functions, which pushes cursor up, refreshes line, then refreshes previous line, which deletes the cursor. 

    And it works relatively, being able to get the previous cursor via the collaboration of 'Cursor cursor' in TextFunctions.h refreshLineRel() func and the printBufferLine() OR printCursorOnly() function. As the name sugguests, this prints a line in the terminal with logic that utilizies the cursor.line, but only takes an input of a line number relative to the current viewport size. It even does some lazy computing, if you refresh the current line, and the cursor is still on the targeted line, i.e. you made some inline edit, it will print the cursor with printCursorOnly(), which refreshes the line with a cursor added. Else, if you ask to print a line that does NOT contain a cursor, then it prints the line with the printBufferLine() function, which does not even attempt to add a cursor to save time.

- Fix the cursor based scrolling after previous update, currently only topline updates, likely because the shift up function does not account for out of bounds. We need to add a check, if the cursor reaches beyond the current viewport, refresh entire screen instead of line refresh.
    - FIXNOTES: fixed, used condition: cursor.line <= Viewport.getTopLine(), which is the same logic used in resize, but, in this case it will only refresh the entire terminal only if the topline viewport is passed by the cursor line. Else, if will continue with the regular logic, which is: refreshLineRel(relative_cursor.line); AND refreshCurrentLine();, thus not wasting resources.

- Shiftdown prints line +1 of what it should on the previous line. I.e. if you had 1 \n 2 \n 3\n, shift down would print -> 3 \n 2\n 3\n
    -FIXNOTES: Problem was that the refresh line realtive was in fact, not relative. It would use cursor.line() in order to find the line to print. Issue was with how we compute the absolute line, which would use cursor.line, which as noted above, is always offset by +1. So then because the function was reliant on the cursor, it would work for something like shift up since the internal "absolute_line" variable was computed with cursor.line+1, i.e. an absolute index for textBuffer. When you shifted down, it would look at the line ahead of the cursor, cursor.line+1, but the problem is that you targeted a line before the target, which wouldnt work, to visualize this:
                line1: apple
                line2: bannana(C)
                line3: cat
                line4: rat
                - where (C) is cursor
                shiftUp(), using refreshLineRel(2):
                line1: apple(C)
                line2: bannana <- this is cursor.line()+1
                line3: cat
                line4: rat
                - since cusror.line()+1 is absolute, it will print this at line 2, which is correct as the line at line 2 is in fact cursor.line+1
                shiftDown(), using refreshLineRel(2):
                line1: apple
                line2: rat <- incorrect line
                line3: cat (c)
                line4: rat <- this is cursor.line+1
                - thus showing the importantsnce of a relative function being a relative function. This was a pretty dumb mistake

- Moving left or right from a smaller than fit all window size causes print of last line to lastline-1 line.
    - problem is in "getRealtiveCursor()" update function.
    - Shifting topline up works because it refreshes the entire frame, using a for loop until it reaches the relative_cursor.line and adds the cursor.
    - Any function that uses the refresh current line does not seem to work because the relative line update does NOT seem to work.
    - Further Questions: Even if relative cursor works on frame rebuild, why does it actually go to the correct line? Also, why does refresh current line still work on every other line but that forst one?
    -FIXNOTES: problem was with the updateScroll function, which would adjust the window to increase buffer visible if cursor.line >= bottomLine, which is wrong since it woudl scroll on the last line. Since it was being called in the refreshCurrentLine(), it would cause an improper update of topline, thus causing an incorrect relative cursor. This also seemed to scroll resize problem wher the bottom line was not completely printing, even when the viewport size can clearly fit the buffer.

- Expanding sometimes fails to print the lower content.
    - FIXNOTES: fixed by updating updatescroll logic.

- Resizing does shrink well, but fails to grow the bottom line past its last known position.
    - FIXNOTES: Works, fixed by adding proper anchor switch logic for both gorwth and shrink.

- No anchor switch logic on resize
    - Added in
WIP:
- Anchor logic works on both resize and growth, but only when slowly done. Any fast shifts in terminal size result in incorrect resize. Debouncing should have solved this, but either its a bad implementation of debouncing or some other unkown bug. 
- Scrolling left out of bounds causes seg fault.
- Arrow scolling "too fast" sometimes causes highlight to dissapear
- very last char highlight is invisible. 
- Rendering lag when using another program. Likely a non-issue since even other editors face the same issue

Models:
    updateResize:
        ViewPort.topLine = V_T
        ViewPort.bottomLine = V_B
        ViewPort.previousHeight = V_PH
        ViewPort.currentAnchor = V_CA
        ViewPort.relativeHeight = V_RH

        Cursor.line = C_l
        
        func inputs:
        absolute_height = absH
        delta = delta


        Flow:
        ShrinkGrowCalculation = V_RH - V_PH

        if(ShrinkGrowCalculation > 0)
            grow(cursor, delta);
            growAnchorCheck(cursor, absolute_height);

        else if(ShrinkGrowCalculation < 0 AND V_B - V_T > V_RH)
            shrink(cursor, delta);
            shrinkAnchorCheck(cursor, absH, 2); (Note that 2 is a placeholder, does nothing)

        Notes: Currently delta is updated by the interrupt for SIGWINCH, given here:
                int CurrentDelta = delta.exchange(0, std::memory_order_relaxed);
                FrameBuild.ClearTerminalAndScrollback();
                Viewport.updateResize(textBuffer.size(), 30, cursor, CurrentDelta);
                getRelativeCursor();
                FrameBuild.buildFrameWithCursor(textBuffer, relative_cursor, Viewport);
                FrameBuild.flushFrame(); 

            A possible bug occurs because we rely on the interrupt being in charge of updating the delta. Previously, one major reason a "bug (though not sure if it has been fixed)" was that currentDetla would be erased at the end of the resize call, meaning that when it did finally resize, any terminal changes that occured while running this resize instruction would have resulted in the deletion of any changes done while resizing. One possible fix is the rely on calculating a delta based on the size of the terminal and the previous state of the terminal viewport rather than relying on recording the number of changes that have occured.

            Another possible problem, in a similar vein to the delta miscalculations is ShrinkGrowCalculations. Since both V_RH and V_PH are so reliant on the previous state of the object, then it could result in an improper calculation. A fix could include recordin the absolute size of the terminal instead.
        
