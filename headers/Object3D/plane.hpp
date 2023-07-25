#pragma once

#include "object3D.hpp"
#include "vectors.hpp"

class Plane: public Object3D
{
public:
    Vec3f normal;
    double d;
public:
    Plane(Vec3f &normal, float d, Material *m);
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  
};