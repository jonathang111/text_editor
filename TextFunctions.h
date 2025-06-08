#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "general.h"
#include "RenderBuffer/RenderBuffer.h"
#include <sstream>
#include <chrono>
#include <atomic>

//when accessing renderbuffer functions, always ensure you update the relative size first.
//except for dynamic ones, they update themselves.
extern std::atomic<std::chrono::steady_clock::time_point> lastResizeRequestTime;

class Editor{
    private:
    std::vector<std::string> textBuffer;
    std::vector<std::string> line;
    std::vector<Delta> undo;
    std::vector<Delta> redo;
    Cursor cursor, relative_cursor;
    RenderBuff render;

    void dynamicPrint();

    void UpdateCursor();
    void getRelativeCursor();

    void InsertChar(char);
    void Backspace();
    void NewLine();

    void stripLastChar(int);

    //calculate longest column size

    struct sigaction sa;

    public:  
    static Editor* instance;

    void start(){
        sa.sa_handler = handle_sigwinch;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
    }

    Editor(std::vector<std::string> ref, Cursor curs) : textBuffer(ref), cursor(curs){
        instance = this;
        render.enableAlternateScreen();
        render.initalizeTerminal(textBuffer); //temporary?
    }

    ~Editor(){
        render.disableAlternateScreen();
    }

    static void handle_sigwinch(int sig){
        // if (instance)
        //     instance->TerminalResize(); //or maybe custom handler?
        lastResizeRequestTime.store(std::chrono::steady_clock::now(), std::memory_order_relaxed);
        delta.fetch_add(1, std::memory_order_relaxed);

        // lastSigTime = std::chrono::steady_clock::now();
        // pendingResize = true;
    }

    //void HandleSignal();

    void printCursorRelative();

    void printFrame();

    void printLine(int);

    void testRender();

    void ReadInput();

    void TerminalResize();
};
