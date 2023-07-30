#ifndef _INPUT_PARSER_H
#define _INPUT_PARSER_H

#include <iostream>
#include <cstring>
#include <cassert>

class InputParser {
public:
    static void init(int argc, char *argv[]);

    static int width, height, phiStep, thetaStep, max_bounces;
    static float depth_min, depth_max, cutoff_weight;
    static bool shade_back, gui, gouraud, shadows, visualize_grid;
    static char *input_file, *output_file, *depth_file, *normal_file;
    static int nx, ny, nz;
};
#endif