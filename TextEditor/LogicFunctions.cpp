#include "TextFunctions.h"

void Editor::ReadInput(){
    char seq, seq1;
    seq = getchar();
    if(seq == '\033'){
        seq1 = getchar();
        if(seq1 == '['){
            UpdateCursor();
        }
    }
    else{
        switch(seq){
            case 127: Backspace(); break;
            case '\n': NewLine(); break;
            case '\t':
            default: 
            if(seq >= 32 && seq <= 128)
                InsertChar(seq);
            else break;
        }
    }
}

void Editor::Backspace(){
    //raise(SIGWINCH); //for debugging
    if(cursor.column == 1 && cursor.line != 1){ //in case line needs to be removed
        cursor.column = textBuffer[cursor.line-2].size();
        stripLastChar(cursor.line-2);
        textBuffer[cursor.line-2] += textBuffer[cursor.line-1];
        textBuffer.erase(textBuffer.begin()+cursor.line-1);
        cursor.line--;
        // std::cout << "\033[" << 1 << ";" << 1 << "H";
        // std::cout << "\033[2J\033[H";
        //printFrame();
        dynamicPrint();
    }
    else{
    textBuffer[cursor.line-1].erase(cursor.column-2, 1);
    cursor.column--;
    std::cout << "\033[" << relative_cursor.line << ";" << 1 << "H";
    std::cout << "\033[2K";
    printLine(cursor.line);
    printCursorRelative();
    }
    // std::cout << "\033[0K"; deletes in front; probably should implement instead of delete whole line (current impl.).
}

void Editor::NewLine(){
    auto it = textBuffer.begin();
    it+= cursor.line;
    textBuffer.emplace(it, textBuffer[cursor.line-1].substr(cursor.column-1, textBuffer[cursor.line-1].length()));
    textBuffer[cursor.line-1].erase(cursor.column-1, textBuffer[cursor.line-1].length()-cursor.column);
    cursor.line++;
    cursor.column = 1;
    dynamicPrint();
}

void Editor::InsertChar(char input){
 textBuffer[cursor.line-1].insert(cursor.column-1, 1 , input);
 std::cout << "\033[" << relative_cursor.line << ";" << 1 << "H";
    printLine(cursor.line); //DO NOT print the relative cursor line, we need the actual line stored in buffer.
    cursor.column++;
    printCursorRelative();
}

void Editor::UpdateCursor(){
    char seq2;
    seq2 = getchar();
    switch(seq2){
        case 'A': 
            if(cursor.line!=1){
                cursor.line--;
                if(cursor.column > textBuffer[cursor.line-1].length())
                    cursor.column = textBuffer[cursor.line-1].length();
            } 
            break;
        case 'B': 
            if(cursor.line < textBuffer.size()){
                cursor.line++; 
                if(cursor.column > textBuffer[cursor.line-1].length())
                cursor.column = textBuffer[cursor.line-1].length();
            }
            break;
        case 'C': 
            /*as long as we are not on the last line or we arent on the last lines length, continue adding.*/
            if(!(cursor.line == textBuffer.size()) || !(cursor.column > textBuffer[textBuffer.size()-1].length())) 
                cursor.column++; 
            if(cursor.column > textBuffer[cursor.line-1].length() && cursor.line!= textBuffer.size()){
                cursor.column = 1;
                cursor.line++;
            }
            break;
        case 'D':
            if(cursor.line != 1 || cursor.column != 1)
                cursor.column--;
            if (cursor.column < 1 && cursor.line > 0){
                cursor.line--;
                cursor.column = textBuffer[cursor.line-1].length();
            }
    }
    if(cursor.line <= Viewport.getTopLine() || cursor.line > Viewport.getBottomLine())
        dynamicPrint();
    else
        printCursorRelative();
}

void Editor::stripLastChar(int line){
    if (!textBuffer[line].empty() && textBuffer[line].back() == '\r') {
        textBuffer[line].pop_back();
}
}