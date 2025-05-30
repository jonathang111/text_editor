# text_editor
a simple text editor

DONT RUN ON YOUR VSCODE TERMINAL, it slows the progrma a lot more than i thought it would.

Personal Notes:
Buffer only flushes on newline, fills up, or if we explicitly flush it.
Need to strategically place the flushes. if you flush too much then you get weird visual artifacts; too little then same problem.
for some reason the entire things is laggy if cursor is not at the bottom.

it took about a million and half years to make "addToFrame" function accept a dynamic amount of obejetcs. but it worked eventually. and it sucked.
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