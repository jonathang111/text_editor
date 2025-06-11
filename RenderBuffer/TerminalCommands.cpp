#include "TerminalCommands.h"

//perhaps this should return a pair?
std::optional<std::pair<int,int>> TermUtil::GetTerminalSize(){ //return 1 if change detected, else 0;
    std::pair<int, int> ret = {0,0};

    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    ret.first = w.ws_row;
    ret.second = w.ws_col;
    
    return ret;
}

termios TermUtil::DefaultTerminalInitalization(){
    termios settings, original;
    tcgetattr(0, &settings);
    original = settings;
    settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSAFLUSH, &settings);
    fcntl(0, F_SETFL, O_NONBLOCK);
    return original;
}

void TermUtil::SetTerminalSettings(termios& termStruct){
    tcsetattr(0, TCSAFLUSH, &termStruct);
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);
    return;
}

void TermUtil::enableAlternateScreen(){
    std::cout << "\x1b[?1049h" << std::flush;
}

void TermUtil::disableAlternateScreen(){
    std::cout << "\x1b[?1049l" << std::flush;
}