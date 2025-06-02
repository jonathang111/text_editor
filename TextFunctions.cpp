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
            default: InsertChar(seq);
        }
    }
}

void Editor::Backspace(){
    raise(SIGWINCH);
    if(cursor.column == 1 && cursor.line != 1){
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
    //textBuffer[cursor.line-1].append("\n");
    std::cout << "\033[" << 1 << ";" << 1 << "H";
    std::cout << "\033[2J\033[H";
    // for(auto& string : textBuffer)
    //     std::cout << string << std::endl;
    cursor.line++;
    cursor.column = 1;
    //printFrame();
    dynamicPrint();
}

void Editor::InsertChar(char input){
 textBuffer[cursor.line-1].insert(cursor.column-1, 1 , input);
 std::cout << "\033[" << relative_cursor.line << ";" << 1 << "H";
    printLine(cursor.line); //DO NOT print the relative cursor line, we need the actual line stored in buffer.
    //std::cout << textBuffer[cursor.line-1].substr(cursor.column-1, textBuffer[cursor.line-1].length()-cursor.column);
    cursor.column++;
    printCursorRelative();
    //std::cout << "\033[" << relative_cursor.line << ";" << relative_cursor.column << "H";
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
    if(cursor.line <= render.getTopLine() || cursor.line > render.getBottomLine())
        dynamicPrint();
    else
        printCursorRelative();
}


void Editor::printFrame(){ //for just printing the buffer, dynamic to window but not relative to cursor, should only be used for initial print
    render.updateBorderLines(textBuffer.size(), 30); //this function is why initalization should be done with this frame. It is an absolute calculator for topline and bottomline
    render.printBuffer(textBuffer);
}

void Editor::dynamicPrint(){ //for printing relative to a cursor, for scrolling or refresh when cursor awareness is needed.
    render.updateBorderLinesCurs(textBuffer.size(), 30, cursor);
    std::cout << "\033[H";
    std::cout << "\033[2J\033[H";
    render.printBuffer(textBuffer);
    getRelativeCursor();
    printCursorRelative();
}

void Editor::TerminalResize(){ //for resizing, with specific terminal anchor logic.
    render.clearTerminalAndScrollback();
    getRelativeCursor();
    render.updateBorderLinesResize(textBuffer.size(), 30, cursor);
    render.buildFrameWithCursor(textBuffer, relative_cursor);
    render.flushFrame();
}

void Editor::printLine(int line){
    render.updateBorderLinesCurs(textBuffer.size(), 30, cursor);
    render.printBufferLine(textBuffer, line-1);
}

void Editor::getRelativeCursor(){ //position of cursor relative to window.
   relative_cursor.line = cursor.line - render.getTopLine();
   relative_cursor.column = cursor.column;
}

void Editor::printCursorRelative(){
    getRelativeCursor();
    std::cout << "\033[" << relative_cursor.line << ";" <<relative_cursor.column << "H" << std::flush;
}

/*if reading from file, there is likely a \r for some reason, you need to strip or else you get "ghosting"
this is probably the most frustrating bug ive encountered for this program. just because you don't realize its a probelm until you run the debugger
and analyze the vector contents.*/
void Editor::stripLastChar(int line){
    if (!textBuffer[line].empty() && textBuffer[line].back() == '\r') {
        textBuffer[line].pop_back();
}
}