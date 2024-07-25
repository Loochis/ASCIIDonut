//
// Created by loochis on 10/11/23.
//

#ifndef ASCII_RENDER_ASCIICHIS_H
#define ASCII_RENDER_ASCIICHIS_H

#include <cstdlib>
#include <vector>

using namespace std;

namespace ASCIICHIS {

    class asciirenderer {
    public:
        int width;
        int height;
        int width_c;
        int height_c;

        int wrap_mode = 0; // 0 = Do Nothing, 1 = Wrap, 2 = Error

        struct color {
            unsigned char r, g, b;
            color() : r(0), g(0), b(0) {}
            color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
        };

        asciirenderer();

        static int bytetocol(ulong out_col, color &channels, char* drawchar);
        static ulong coltobyte(color channels, const char* drawchar);

        int setpix(int x, int y, ulong col);
        ulong getpix(int x, int y);
        int drawbox(int x1, int y1, int x2, int y2, int col);

        int pushscreen();
        int flushscreen();
    private:
        vector<ulong> vram;

        int idxfromcoord(int x, int y);
        int coordfromidx(int i, int& x, int& y);
    };

} // ASCIICHIS

#endif //ASCII_RENDER_ASCIICHIS_H
