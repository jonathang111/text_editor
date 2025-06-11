#pragma once
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <optional>

namespace TermUtil{ //maybe this should be a class so that you can just store the settings in the object rather than having to manually store it
    std::optional<std::pair<int,int>> GetTerminalSize(); //used to calculate the terminal size

    termios DefaultTerminalInitalization(); //returns a copy of the original settings
    void SetTerminalSettings(termios&); //forces settings to be set to passed through struct, usually used to restore settings

    void enableAlternateScreen();
    void disableAlternateScreen();
}