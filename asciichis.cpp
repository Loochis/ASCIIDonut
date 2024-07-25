//
// Created by loochis on 10/11/23.
//

// REFERENCE
// \033[38;2;<r>;<g>;<b>m # Select RGB foreground color
// \033[48;2;<r>;<g>;<b>m # Select RGB background color

// coord wrapping:
// 0 1 2 3 4 ...
// 5 6 7 8 9 ...
// A B C D E ...

#include <stdio.h>
#include "asciichis.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <string> // for string class
#include <iostream>
#include <chrono>
#include <thread>
#include <cstring>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
using namespace std;

namespace ASCIICHIS {
    asciirenderer::asciirenderer() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        width = w.ws_col;
        height = w.ws_row;
        width_c = width/2;
        height_c = height/2;

        vram.resize(width*height);

        // Disable the cursor
        printf("\033[?25l");
    }

    int asciirenderer::bytetocol(ulong out_col, color &channels, char* drawchar) {
        channels.r = (unsigned char)(out_col & 0xFF);
        channels.g = (unsigned char)((out_col >> 8) & 0xFF);
        channels.b = (unsigned char)((out_col >> 16) & 0xFF);

        drawchar[0] = (char)((out_col >> 32) & 0xFF);
        drawchar[1] = (char)((out_col >> 40) & 0xFF);
        drawchar[2] = (char)((out_col >> 48) & 0xFF);
        drawchar[3] = (char)((out_col >> 56) & 0xFF);
        return 0;
    }

    ulong asciirenderer::coltobyte(color channels, const char *drawchar) {
        ulong retlong = 0;

        // ITS ALL BYTES BATMAN -Joker
        memcpy(&retlong, drawchar, 4);

        retlong <<= 16;
        retlong |= channels.b;
        retlong <<= 8;
        retlong |= channels.g;
        retlong <<= 8;
        retlong |= channels.r;

        return retlong;
    }

    int asciirenderer::setpix(int x, int y, ulong col) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return 1;
        vram[idxfromcoord(x, y)] = col;
        return 0;
    }

    ulong asciirenderer::getpix(int x, int y) {
        return vram[idxfromcoord(x, y)];
    }

    int asciirenderer::drawbox(int x1, int y1, int x2, int y2, int col) {
        for (int x = x1; x < x2; x++) {
            //float xPerc = (float)(x-x1) / (float)(x2-x1);
            for (int y = y1; y < y2; y++) {
                //float yPerc = (float)(y-y1) / (float)(y2-y1);
                setpix(x, y, col);
            }
        }
        return 0;
    }

    int asciirenderer::idxfromcoord(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw std::out_of_range("Coord->Index is out of bounds");
        return x + y * width;
    }

    int asciirenderer::coordfromidx(int i, int &x, int &y) {
        if (i < 0 || i >= width*height)
            throw std::out_of_range("Index->Coord is out of bounds");
        x = i % width;
        y = i / width;

        return 0;
    }

    // Push to screen
    // \033[38;2;<r>;<g>;<b>m # Select RGB foreground color

    int asciirenderer::pushscreen() {
        int ctr = 0;
        color channels;
        char drawchar[4] = "█";
        string text = "";
        for (int i = 0; i < width*height; i++) {
            bytetocol(vram[i], channels, drawchar);
            //printf("\033[38;2;%d;%d;%dm@", r, g, b);
            //text += "\033[48;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+
            //        ";38;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m@";
            if (drawchar[0] == 0x00 && drawchar[1] == 0x00 && drawchar[2] == 0x00 && drawchar[3] == 0x00)
                text += "\033[49;39m ";
            else
                text += "\033[49;38;2;"+to_string(channels.r)+";"+to_string(channels.g)+";"+to_string(channels.b)+"m" + drawchar;
        }

        //cout << "\033[2J\033[1;1H";
        cout << "\033[1;1H";
        //sleep_for(milliseconds(2));
        cout << text.c_str();
        return 0;
    }

    int asciirenderer::flushscreen() {
        for (int i = 0; i < width*height; i++)
            vram[i] = 0;
        return 0;
    }
} // ASCIICHIS