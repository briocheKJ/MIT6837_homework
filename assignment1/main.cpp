#include<iostream>
#include<cassert>
#include<cstring>
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/hit.hpp"
#include "../headers/Object3D/group.hpp"
#include "../headers/material.hpp"

#include "scene_parser.hpp"

using namespace std;

int main(int argc,char * argv[])
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

    // sample command line:
    // ./raytracer -input in/scene1_01.txt -size 200 200 -output out/output1_1.tga -depth 9 10 out/depth1_1.tga
    // ./raytracer -input in/scene1_02.txt -size 200 200 -output out/output1_02.tga -depth 8 12 out/depth1_02.tga
    // ./raytracer -input in/scene1_03.txt -size 200 200 -output out/output1_03.tga -depth 8 12 out/depth1_03.tga
    // ./raytracer -input in/scene1_04.txt -size 200 200 -output out/output1_04.tga -depth 12 17 out/depth1_04.tga
    // ./raytracer -input in/scene1_05.txt -size 200 200 -output out/output1_05.tga -depth 14.5 19.5 out/depth1_05.tga
    // ./raytracer -input in/scene1_06.txt -size 200 200 -output out/output1_06.tga -depth 3 7 out/depth1_06.tga
    // ./raytracer -input in/scene1_07.txt -size 200 200 -output out/output1_07.tga -depth -2 2 out/depth1_07.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    cout<<"hello!"<<endl;

    SceneParser sceneParser(input_file);
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();
    Image colorRes(width, height), depthRes(width, height);
    
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            //std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/width, (j + 0.5f)/height));
            Hit h;
            if(group->intersect(r, h, camera->getTMin()))
            {
                // cout << h.getMaterial()->getDiffuseColor() << endl;
                //cout<<"!#@!#@"<<endl;
                //cout<<h.getT()<<endl;
                colorRes.SetPixel(i, j, (h.getMaterial())->getDiffuseColor());
                //cout<<h.getT()<<endl;
                float d = (h.getT() - depth_min) / (depth_max - depth_min);
                //d = max(0.0f, min(1.0f, 1.0f-d));
                d=1-d;
                depthRes.SetPixel(i, j, Vec3f(d, d, d));
            }
            else {
                colorRes.SetPixel(i, j, sceneParser.getBackgroundColor());
                depthRes.SetPixel(i, j, Vec3f(0, 0, 0));
            }
        }
    }
    colorRes.SaveTGA(output_file);
    depthRes.SaveTGA(depth_file);

    return 0;
}