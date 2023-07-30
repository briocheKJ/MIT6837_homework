#pragma once

#include "object3D.hpp"
#include "../vectors.hpp"

class Sphere: public Object3D
{
public:
    Vec3f center;
    double radius;
public:
    Sphere(Vec3f c, double r, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  
    virtual void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    Vec3f getPoint(float phi, float theta) const;
};