#pragma once

#include "../boundingbox.hpp"

class Material;
class Ray;
class Hit;
class Grid;

class Object3D 
{
public:
    Material * material;
    BoundingBox *boundingBox;

public:
    virtual ~Object3D(){delete boundingBox;}
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m) ;

    BoundingBox * getBoundingBox(){return boundingBox;}
};