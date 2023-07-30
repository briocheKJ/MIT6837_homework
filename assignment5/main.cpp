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
#include "rayTracer.hpp"
#include "input_parser.hpp"
#include "rayTree.hpp"
#include "Object3D/grid.hpp"

using namespace std;

SceneParser* sceneParser=NULL;
RayTracer* rayTracer=NULL;
Grid* grid=NULL;

Vec3f absolute(const Vec3f & vec){return Vec3f(fabsf(vec[0]), fabsf(vec[1]), fabsf(vec[2]));}
void render();
void trace(float x, float y);

int main(int argc,char * argv[])
{
    InputParser::init(argc,argv);

    cout<<"hello!"<<endl;
    

    sceneParser = new SceneParser(InputParser::input_file);

    if(InputParser::nx > 0) 
    {
        grid = new Grid(sceneParser->getGroup()->getBoundingBox(), InputParser::nx, InputParser::ny, InputParser::nz);
        sceneParser->getGroup()->insertIntoGrid(grid, NULL);
    }

    rayTracer = new RayTracer(sceneParser, InputParser::max_bounces, InputParser::cutoff_weight, InputParser::shadows, InputParser::shade_back, grid);
    
    if(InputParser::gui)
    {
        GLCanvas canvas;
        glutInit(&argc,argv);
		canvas.initialize(sceneParser, render, trace, grid, InputParser::visualize_grid);
        delete sceneParser;
    }
    else{
        render();
    }
    delete grid;
    return 0;
}

void trace(float x, float y)
{
    Camera * camera = sceneParser->getCamera();
	Ray r = camera->generateRay(Vec2f((x*InputParser::width + 0.5f)/InputParser::width, (y*InputParser::height + 0.5f)/InputParser::height));
	Hit hit(camera->getTMin(), nullptr, Vec3f(0, 0, 0));
	Vec3f color = rayTracer->traceRay(r, camera->getTMin(), 0, 1, 1, hit);
	RayTree::SetMainSegment(r, 0, hit.getT());
	RayTree::Print();
}

void render()
{
    Camera* camera = sceneParser->getCamera();
    Group* group = sceneParser->getGroup();
    Image colorRes(InputParser::width, InputParser::height);
    Image depthRes(InputParser::width, InputParser::height);
    Image normalRes(InputParser::width, InputParser::height);
    colorRes.SetAllPixels(sceneParser->getBackgroundColor());
	depthRes.SetAllPixels(Vec3f(0, 0, 0));
	normalRes.SetAllPixels(Vec3f(0, 0, 0));
    for(int i=0;i<InputParser::width;++i)
    {
        for(int j=0;j<InputParser::height;++j)
        {
            //std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/InputParser::width, (j + 0.5f)/InputParser::height));
            Hit h(camera->getTMin(), nullptr, Vec3f(0, 0, 0));
			Vec3f color = rayTracer->traceRay(r, camera->getTMin(), 0, 1, 1, h);
			//cout << "COLOR: " << i << ", " << j << ", " << color << endl;
			if (h.getT() > camera->getTMin())
			{
				colorRes.SetPixel(i, j, color);
                normalRes.SetPixel(i, j, absolute(h.getNormal()));
				float d = (h.getT() - InputParser::depth_min) / (InputParser::depth_max - InputParser::depth_min);
                d = max(0.0f, min(1.0f, 1.0f-d));
                depthRes.SetPixel(i, j, Vec3f(d, d, d));
                normalRes.SetPixel(i, j, absolute(h.getNormal()));
			}
        }
    }
    if(InputParser::output_file!=NULL)
        colorRes.SaveTGA(InputParser::output_file);
    if(InputParser::depth_file!=NULL)
        depthRes.SaveTGA(InputParser::depth_file);
    if(InputParser::normal_file!=NULL)
        normalRes.SaveTGA(InputParser::normal_file);
}