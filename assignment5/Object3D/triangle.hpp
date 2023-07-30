#pragma once

#include "object3D.hpp"
#include "../vectors.hpp"

class Triangle: public Object3D
{
public:
    Vec3f a,b,c,normal;
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  
    virtual void paint();
    void insertIntoGrid(Grid* g, Matrix* m);
};