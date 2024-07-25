#include <iostream>
#include <memory>

#include "asciichis.h"
#include "viewmanager.h"
#include "Drawables/line.h"

using namespace std;
using namespace ASCIICHIS;

unsigned long total_t = 0;
unsigned long delta_t = 0;


int main() {
    shared_ptr<asciirenderer> rend = make_shared<asciirenderer>();
    viewmanager manager = viewmanager(rend);

    char tmpchar[4] = "@";
    manager.drawables.push_back(
            make_shared<line>
            (0,0.5,
             drawable::vector_i(1,1), drawable::vector_i(220,1),
             asciirenderer::color(0x88, 0x88, 0x88), tmpchar,
             drawable::DrawMode::FADE_BLOCK, rend));

    manager.drawables.push_back(
            make_shared<line>
                    (0.2,0.4,
                     drawable::vector_i(1,11), drawable::vector_i(220,11),
                     asciirenderer::color(0x88, 0x88, 0x88), tmpchar,
                     drawable::DrawMode::FADE_BLOCK, rend));

    manager.drawables.push_back(
            make_shared<line>
                    (0.4,0.3,
                     drawable::vector_i(1,21), drawable::vector_i(220,21),
                     asciirenderer::color(0x88, 0x88, 0x88), tmpchar,
                     drawable::DrawMode::FADE_BLOCK, rend));

    srand(1);

    while(true) {
        manager.Update();
    }
    return 0;
}
