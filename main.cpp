#include<fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "TextEditor/TextFunctions.h"
#include <termios.h>
#include <fcntl.h>

// Main is currently for testing;

int main(){
    char* gato = new char[10]; //temp for testing
    Cursor position;
    
    std::vector<std::string> text;
    std::ifstream Document("text.txt");
    std::string st;
    //test construction
    while(std::getline(Document, st)){
        text.push_back(st);
    }

    Cursor cursor;
    cursor.column = text[text.size()-1].length()+1;
    cursor.line = text.size();

    Editor test1(text, cursor);
    test1.start(); //process runs here

    Document.close();
    delete[] gato;
}