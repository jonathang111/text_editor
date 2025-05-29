#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "TextFunctions.h"
#include <termios.h>

Editor* Editor::instance = nullptr;

int main(){
    char* gato = new char[10];
    termios settings, original;
    tcgetattr(0, &settings);
    original = settings;
    settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSAFLUSH, &settings);

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
    std::cout << "\033[1;1H";
    test1.printFrame();
    signal(SIGWINCH, Editor::handle_sigwinch);
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
        test1.ReadInput();
    }
    // test1.testRender();
    tcsetattr(0, TCSAFLUSH, &original);
    Document.close();
    delete[] gato;
}