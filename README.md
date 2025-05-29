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