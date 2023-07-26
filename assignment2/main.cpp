#include<iostream>
#include<cassert>
#include<cstring>
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/hit.hpp"
#include "../headers/Object3D/group.hpp"
#include "../headers/material.hpp"
#include "../headers/light.hpp"

#include "scene_parser.hpp"

using namespace std;

Vec3f absolute(const Vec3f & vec)
{
	return Vec3f(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]));
}

int main(int argc,char * argv[])
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    char *normal_file = NULL;
    bool shade_back=false;

    // sample command line:
    // ./raytracer -input in/scene2_01_diffuse.txt -size 200 200 -output out/output2_01.tga
    // ./raytracer -input in/scene2_02_ambient.txt -size 200 200 -output out/output2_02.tga
    // ./raytracer -input in/scene2_03_colored_lights.txt -size 200 200 -output out/output2_03.tga -normals out/normals2_03.tga
    // ./raytracer -input in/scene2_04_perspective.txt -size 200 200 -output out/output2_04.tga -normals out/normals2_04.tga
    // ./raytracer -input in/scene2_05_inside_sphere.txt -size 200 200 -output out/output2_05.tga -depth 9 11 out/depth2_05.tga -normals out/normals2_05.tga -shade_back
    // ./raytracer -input in/scene2_05_inside_sphere.txt -size 200 200 -output out/output2_05_no_back.tga
    // ./raytracer -input in/scene2_06_plane.txt -size 200 200 -output out/output2_06.tga -depth 8 20 out/depth2_06.tga -normals out/normals2_06.tga
    // ./raytracer -input in/scene2_07_sphere_triangles.txt -size 200 200 -output out/output2_07.tga -depth 9 11 out/depth2_07.tga -normals out/normals2_07.tga -shade_back
    // ./raytracer -input in/scene2_07_sphere_triangles.txt -size 200 200 -output out/output2_07_no_back.tga
    // ./raytracer -input in/scene2_08_cube.txt -size 200 200 -output out/output2_08.tga
    // ./raytracer -input in/scene2_09_bunny_200.txt -size 200 200 -output out/output2_09.tga -normals a.tga
    // ./raytracer -input in/scene2_10_bunny_1k.txt -size 200 200 -output out/output2_10.tga
    // ./raytracer -input in/scene2_11_squashed_sphere.txt -size 200 200 -output out/output2_11.tga -normals out/normals2_11.tga
    // ./raytracer -input in/scene2_12_rotated_sphere.txt -size 200 200 -output out/output2_12.tga -normals out/normals2_12.tga
    // ./raytracer -input in/scene2_13_rotated_squashed_sphere.txt -size 200 200 -output out/output2_13.tga -normals out/normals2_13.tga
    // ./raytracer -input in/scene2_14_axes_cube.txt -size 200 200 -output out/output2_14.tga
    // ./raytracer -input in/scene2_15_crazy_transforms.txt -size 200 200 -output out/output2_15.tga
    // ./raytracer -input in/scene2_16_t_scale.txt -size 200 200 -output out/output2_16.tga -depth 2 7 out/depth2_16.tga

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
        } 
        else if (!strcmp(argv[i],"-normals"))
        {
            i++; assert (i < argc); 
            normal_file = argv[i];
        }
        else if(!strcmp(argv[i],"-shade_back"))
        {
            shade_back=true;
        }
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    cout<<"hello!"<<endl;

    SceneParser sceneParser(input_file);
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();
    Image colorRes(width, height), depthRes(width, height),normalRes(width, height);
    
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            //std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/width, (j + 0.5f)/height));
            Hit h;
            if(group->intersect(r, h, camera->getTMin()))
            {
                //std::cout << i << ' ' << j << std::endl;
                //if(i>5)exit(0);

                // cout << h.getMaterial()->getDiffuseColor() << endl;
                //cout<<h.getT()<<endl;
                if(h.getNormal().Dot3(r.getDirection()) >= 0)
                {
                    if(shade_back)
                    {
                        Vec3f normal = h.getNormal();
                        normal.Negate();
                        h.set(h.getT(), h.getMaterial(), normal, r);
                    }
                    else
                    {
                        colorRes.SetPixel(i, j, Vec3f(0,0,0));
                        float d = (h.getT() - depth_min) / (depth_max - depth_min);
                        d = max(0.0f, min(1.0f, 1.0f-d));
                        depthRes.SetPixel(i, j, Vec3f(d, d, d));
                        normalRes.SetPixel(i, j, absolute(h.getNormal()));
                        continue;
                    }
                }
                Vec3f color=h.getMaterial()->getDiffuseColor();
                Vec3f::Mult(color,color,sceneParser.getAmbientLight());
                int numLights=sceneParser.getNumLights();
                for(int k=0;k<numLights;k++)
                {
                    Light* light=sceneParser.getLight(k);
                    Vec3f lightColor, dir;
                    light->getIllumination(h.getIntersectionPoint(),dir,lightColor);
                    Vec3f I;
                    Vec3f::Mult(I,lightColor,h.getMaterial()->getDiffuseColor());
                    color+=I*max(0.0f,dir.Dot3(h.getNormal()));
                }
                colorRes.SetPixel(i, j, color);
                //cout<<h.getT()<<endl;
                float d = (h.getT() - depth_min) / (depth_max - depth_min);
                d = max(0.0f, min(1.0f, 1.0f-d));
                depthRes.SetPixel(i, j, Vec3f(d, d, d));
                normalRes.SetPixel(i, j, absolute(h.getNormal()));
                //cout<<h.getNormal()<<endl;
            }
            else {
                colorRes.SetPixel(i, j, sceneParser.getBackgroundColor());
                depthRes.SetPixel(i, j, Vec3f(0, 0, 0));
                normalRes.SetPixel(i, j, Vec3f(0, 0, 0));
            }
        }
    }
    if(output_file!=NULL)
        colorRes.SaveTGA(output_file);
    if(depth_file!=NULL)
        depthRes.SaveTGA(depth_file);
    if(normal_file!=NULL)
        normalRes.SaveTGA(normal_file);

    return 0;
}