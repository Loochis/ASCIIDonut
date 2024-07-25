//
// Created by loochis on 10/11/23.
//

#ifndef ASCII_RENDER_ASCIICHIS_H
#define ASCII_RENDER_ASCIICHIS_H

#include <cstdlib>
#include <vector>
#include <cstdint>

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
            float r, g, b;
            wchar_t d_char;
            color() : r(0), g(0), b(0), d_char(L'@') {}
            color(float r, float g, float b, wchar_t d_char) : r(r), g(g), b(b), d_char(d_char) {}

            ulong bytes();
        };

        asciirenderer();

        static int bytetocol(ulong out_col, uint8_t &r, uint8_t &g, uint8_t &b, char* drawchar);

        int setpix(int x, int y, ulong col_bytes);
        int setpix(int x, int y, color col);
        ulong getpix(int x, int y);

        int pushscreen();
        int flushscreen();
    private:
        vector<ulong> vram;

        int idxfromcoord(int x, int y);
        int coordfromidx(int i, int& x, int& y);
    };

} // ASCIICHIS

#endif //ASCII_RENDER_ASCIICHIS_H
