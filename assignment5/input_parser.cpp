#include "input_parser.hpp"

int InputParser::width = 100;
int InputParser::height = 100;
int InputParser::phiStep = 0;
int InputParser::thetaStep = 0;
int InputParser::max_bounces = 0;
float InputParser::depth_min = 0;
float InputParser::depth_max = 1;
float InputParser::cutoff_weight = 0;
bool InputParser::shade_back = false;
bool InputParser::gui = false;
bool InputParser::gouraud = false;
bool InputParser::shadows = false;
char* InputParser::input_file = nullptr;
char* InputParser::output_file = nullptr;
char* InputParser::depth_file = nullptr;
char* InputParser::normal_file = nullptr;
int InputParser::nx=0,InputParser::ny=0,InputParser::nz=0;
bool InputParser::visualize_grid=false;

void InputParser::init(int argc, char *argv[])
{
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-input")) {
                i++; assert (i < argc);
                input_file = argv[i];
            } else if (!strcmp(argv[i], "-output")) {
                i++; assert (i < argc);
                output_file = argv[i];
            } else if (!strcmp(argv[i], "-size")) {
                i++; assert (i < argc);
                width = atoi(argv[i]);
                i++; assert (i < argc);
                height = atoi(argv[i]);
            } else if (!strcmp(argv[i], "-depth")) {
                i++; assert (i < argc);
                depth_min = atof(argv[i]);
                i++; assert (i < argc);
                depth_max = atof(argv[i]);
                i++; assert (i < argc);
                depth_file = argv[i];
            } else if (!strcmp(argv[i], "-normals")) {
                i++; assert (i < argc);
                normal_file = argv[i];
            } else if (!strcmp(argv[i], "-shade_back")) {
                shade_back = true;
            } else if (!strcmp(argv[i], "-gui")) {
                gui = true;
            } else if (!strcmp(argv[i], "-tessellation")) {
                i++; assert (i < argc);
                thetaStep = atof(argv[i]);
                i++; assert (i < argc);
                phiStep = atof(argv[i]);
            } else if (!strcmp(argv[i], "-gouraud")) {
                gouraud = true;
            } else if (!strcmp(argv[i], "-shadows")) {
                shadows = true;
            } else if (!strcmp(argv[i], "-bounces")) {
                i++; assert (i < argc);
                max_bounces = atoi(argv[i]);
            } else if (!strcmp(argv[i], "-weight")) {
                i++; assert (i < argc);
                cutoff_weight = atof(argv[i]);
            } else if (!strcmp(argv[i], "-grid"))
            {
                i++;
                assert(i < argc);
                nx = atoi(argv[i]);
                i++;
                assert(i < argc);
                ny = atoi(argv[i]);
                i++;
                assert(i < argc);
                nz = atoi(argv[i]);
            }
            else if (!strcmp(argv[i], "-visualize_grid"))
            {
                visualize_grid = true;
            } else {
                printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
                assert(0);
            }
        }
}