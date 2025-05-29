#pragma once

typedef enum{
    none = 0,
    backspace = 1,
    space = 2,
    insert = 3,
    newline = 4
} Actions;

struct Delta{
    Actions action = none;
    int line = 0;
    int column = 0;
};

struct Cursor{
    int line = 0;
    int column = 0;
};
