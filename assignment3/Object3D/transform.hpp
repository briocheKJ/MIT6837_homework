#pragma once

#include "object3D.hpp"
#include "../matrix.hpp"

class Transform: public Object3D
{
public:
    Matrix matrix;
    Object3D *o;
public:
    Transform(Matrix &m, Object3D *o)
    {
        matrix=m;
        this->o=o;
    }
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  

    virtual void paint();
};