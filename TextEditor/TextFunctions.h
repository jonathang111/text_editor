#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "../general.h"
#include "../RenderBuffer/TerminalCommands.h"
#include "../RenderBuffer/TerminalViewport/TerminalViewport.h"
#include "../RenderBuffer/Templates.h"
#include "../RenderBuffer/Renderer.h"
#include <sstream>
#include <chrono>
#include <atomic>
#include <thread>

//when accessing renderbuffer functions, always ensure you update the relative size first.
//except for dynamic ones, they update themselves.

class Editor{
    private:
    static std::atomic<int> delta;
    static std::atomic<std::chrono::steady_clock::time_point> lastResizeRequestTime;
    std::vector<std::string> textBuffer;
    std::vector<std::string> line;
    std::vector<Delta> undo;
    std::vector<Delta> redo;
    Cursor cursor, relative_cursor;
    TerminalViewport Viewport;
    FrameBuilder FrameBuild;
    termios original;

    // Logic Functions
    void InsertChar(char);
    void Backspace();
    void NewLine();

    void stripLastChar(int);

    // Print Functions

    void printFrame(); //for debugging

    void dynamicPrint(); //Used for frame updates where scroll happens, refreshes entire frame.

    void printLine(int); //for debugging

    void testRender(); //for debugging

    void refreshLineRel(int); //relative line only
    void refreshCurrentLine();

    // Utilities
    void InitalizeTerminal();
    void TerminalResize();

    void UpdateCursor();
    void getRelativeCursor();

    void shiftCursorUp();
    void shiftCursorDown();

    void moveCursorTo(int, int);

    struct sigaction sa;

    public:  
    static Editor* instance;

    void start();

    Editor(std::vector<std::string> ref, Cursor curs) : textBuffer(ref), cursor(curs){
        sa.sa_handler = handle_sigwinch;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        instance = this;
        TermUtil::enableAlternateScreen();
        InitalizeTerminal();
    }

    ~Editor(){
        TermUtil::disableAlternateScreen();
        TermUtil::SetTerminalSettings(original);
    }

    static void handle_sigwinch(int sig){
        Editor::lastResizeRequestTime.store(std::chrono::steady_clock::now(), std::memory_order_relaxed);
        Editor::delta.fetch_add(1, std::memory_order_relaxed);
    }

    void ReadInput();
};
