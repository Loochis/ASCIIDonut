#include <iostream>
#include <memory>
#include <math.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "asciichis.h"

using namespace std;
using namespace ASCIICHIS;

shared_ptr<asciirenderer> renderer;

static const float RADIUS_MAJOR = 5.0f;
static const float RADIUS_MINOR = 2.0f;

static const float SCALE_FACTOR = 5.0f;
static const float FILL_RATE = 500.0f;

static const float CHAR_HEIGHT_FACTOR = 1.0f/2.2f;

static vector<float> depth_buffer;
static glm::mat4 donut_matrix = glm::mat4(1);

static glm::vec3 light_dir = glm::normalize(glm::vec3(1, -2, -3));

// 10 LEVELS
static const wchar_t gradient[] = {L' ', L'.', L':', L'-', L'=', L'+', L'*', L'#', L'%', L'@'};

// DEPTH BUFFER UTILS
void FlushDepth() {
    for (float & i : depth_buffer)
        i = INFINITY;
}

int DB_IdxFromCoord(int x, int y) {
    if (x < 0 || x >= renderer->width || y < 0 || y >= renderer->height)
        throw std::out_of_range("Coord->Index is out of bounds");
    return x + y * renderer->width;
}

// TRANSFORMATION
void WorldToScreen(float x, float y, float z, int &x_o, int &y_o) {
    x_o = static_cast<int>(x*SCALE_FACTOR) + renderer->width_c;
    y_o = static_cast<int>(y*SCALE_FACTOR*CHAR_HEIGHT_FACTOR) + renderer->height_c;
}

// DONUT DRAWING METHOD
void DrawDonut() {
    for (int t_int = 0; t_int < M_PI*2*FILL_RATE; t_int++) {
        float t_minor = (float)t_int;
        float t_major = t_minor/FILL_RATE;

        // precompute reused values
        float sin_major = sin(t_major);
        float cos_major = cos(t_major);
        float sin_minor = sin(t_minor);
        float cos_minor = cos(t_minor);

        glm::vec4 donut_point = glm::vec4(
                RADIUS_MAJOR*sin_major + RADIUS_MINOR*sin_minor*sin_major,
                RADIUS_MAJOR*cos_major + RADIUS_MINOR*sin_minor*cos_major,
                RADIUS_MINOR*cos_minor,
                1);

        // TRANSFORM MODEL -> WORLD
        donut_point = donut_matrix*donut_point;

        // TRANSFORM WORLD -> SCREEN
        int x_p, y_p;
        WorldToScreen(donut_point.x, donut_point.y, donut_point.z, x_p, y_p);

        // COMPUTE NORMALS
        glm::vec4 donut_norm = glm::vec4(
                sin_minor*sin_major,
                sin_minor*cos_major,
                cos_minor,
                0);
        donut_norm = donut_matrix*donut_norm;

        float light = glm::dot(glm::vec3(donut_norm), light_dir);
        if (light < 0)
            light = 0;

        // FILL IN PATCH

        for (int x_patch = -1; x_patch < 2; x_patch++) {
            for (int y_patch = -1; y_patch < 2; y_patch++) {
                int x_final = x_p + x_patch;
                int y_final = y_p + y_patch;

                // DEPTH TEST
                int db_idx = DB_IdxFromCoord(x_final, y_final);
                if (depth_buffer[db_idx] < donut_point.z)
                    continue;
                else
                    depth_buffer[db_idx] = donut_point.z;

                int grad_idx = (int)(light*9.0f);
                if (grad_idx < 0) grad_idx = 0;
                if (grad_idx > 9) grad_idx = 9;

                asciirenderer::color col = asciirenderer::color(1,1,1,gradient[grad_idx]);
                renderer->setpix(x_final,y_final, col);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    renderer = make_shared<asciirenderer>();
    depth_buffer.resize(renderer->width * renderer->height);

    while(true) {
        donut_matrix = glm::rotate(donut_matrix, glm::radians(0.1f), glm::vec3(1.0f, 0.5f, 0.0f));
        donut_matrix = glm::rotate(donut_matrix, glm::radians(0.052f), glm::vec3(0.3f, 0.0f, 0.3f));

        FlushDepth();

        renderer->flushscreen();
        DrawDonut();
        renderer->pushscreen();
    }
    return 0;
}
