#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "TextEditor/TextFunctions.h"
#include <termios.h>
#include <fcntl.h>

int main(){
    char* gato = new char[10];
    // termios settings, original;
    // tcgetattr(0, &settings);
    // original = settings;
    // settings.c_lflag &= ~(ICANON | ECHO);
    // tcsetattr(0, TCSAFLUSH, &settings);
    // fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    Cursor position;

    std::vector<std::string> text;
    std::ifstream Document("text.txt");
    std::string st;
    //test construction
    while(std::getline(Document, st)){
        text.push_back(st);
    }

    // prints the text for initial, likely to be replaced by terminal render function.
    // for(int i = 0; i < text.size(); i++) //note that endline is added in, i don't believe the strings have that on their own.
    //     std::cout << text[i] << std::endl;;

    Cursor cursor;
    //text[text.size()-1].append("\n");
    cursor.column = text[text.size()-1].length()+1;
    cursor.line = text.size();

    Editor test1(text, cursor);
    test1.start();
    //std::cout << "\033[1;1H";
    //test1.printFrame();
    // signal(SIGWINCH, Editor::handle_sigwinch);
    // while(true){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(8));
    //     auto now = std::chrono::steady_clock::now();
    //     auto last = lastResizeRequestTime.load(std::memory_order_relaxed);
    //     // if (now - last >= debounceDelay) {
    //     //     test1.TerminalResize();
    //     //     lastResizeRequestTime.store(std::chrono::steady_clock::time_point::min(), std::memory_order_relaxed); // or set to max time to suppress repeats
    //     // }
    //     int ifdelta = delta.load(); //change to exchange here. if process happens in between, it will be set to 0
    //     if (now - last >= debounceDelay || ifdelta >= 1) {
    //         //int ifdelta = delta.exchange(0, std::memory_order_relaxed);
    //         test1.TerminalResize();
    //         lastResizeRequestTime.store(std::chrono::steady_clock::time_point::min(), std::memory_order_relaxed);
    //     }

    //     test1.ReadInput();
    //     }

    // test1.testRender();
    // tcsetattr(0, TCSAFLUSH, &original);
    // fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) & ~O_NONBLOCK);
    Document.close();
    delete[] gato;
}