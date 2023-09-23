#include "raylib.h" 
#include <stdio.h>
#include <stdint.h>


struct Console {
    const char* font;
    char* buffer;

    float size = 0.5;
    bool is_displayed = false;

    const char* init_message = "Console\n";
};

Console init_console(Console *console) {
    Console console;
    console.is_displayed = true;
    for(char c : console.init_message)
        console->buffer += c;
    return console;
     
}

void append_to_buffer(char append, char* buffer) {
    buffer += append;
}

