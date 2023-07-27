#include<iostream>
#include<cassert>
#include<cstring>
#include<GL/glut.h>
#include "image.hpp"
#include "camera.hpp"
#include "hit.hpp"
#include "Object3D/group.hpp"
#include "material.hpp"
#include "light.hpp"
#include "glCanvas.hpp"
#include "scene_parser.hpp"

using namespace std;

SceneParser* sceneParser=NULL;

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back=false, gui = false;

extern int phiStep, thetaStep;
extern bool gouraud;
bool gouraud=false;
int phiStep=0,thetaStep=0;

Vec3f absolute(const Vec3f & vec){return Vec3f(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]));}
void render();

int main(int argc,char * argv[])
{
    
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
        else if (!strcmp(argv[i], "-tessellation"))
		{
			i++; assert(i < argc);
			thetaStep = atoi(argv[i]);
			i++; assert(i < argc);
			phiStep = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud"))
		{
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-gui")){gui = true;}
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    cout<<"hello!"<<endl;
    
    sceneParser = new SceneParser(input_file);
    
    if(gui)
    {
        GLCanvas canvas;
        glutInit(&argc,argv);
		canvas.initialize(sceneParser, render);
        delete sceneParser;
    }
    else{
        render();
    }
    return 0;
}
void render()
{
    Camera* camera = sceneParser->getCamera();
    Group* group = sceneParser->getGroup();
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
                Vec3f::Mult(color,color,sceneParser->getAmbientLight());
                int numLights=sceneParser->getNumLights();
                for(int k=0;k<numLights;k++)
                {
                    Light* light=sceneParser->getLight(k);
                    Vec3f lightColor, dir;
                    float distance;
                    light->getIllumination(h.getIntersectionPoint(),dir,lightColor,distance);
                    // Vec3f I;
                    // Vec3f::Mult(I,lightColor,h.getMaterial()->getDiffuseColor());
                    // color+=I*max(0.0f,dir.Dot3(h.getNormal()));
                    color += h.getMaterial()->Shade(r, h, dir, lightColor);
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
                colorRes.SetPixel(i, j, sceneParser->getBackgroundColor());
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
}