#pragma once

#include "object3D.hpp"
#include "../vectors.hpp"

#include <vector>

class Group: public Object3D
{
public:
    std::vector<Object3D*> members;
public:
    Group(int num_objects){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  
    void addObject(int index, Object3D *obj); 
};