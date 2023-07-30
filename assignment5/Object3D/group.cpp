#include <iostream>

#include "group.hpp"
#include "object3D.hpp"
#include "../boundingbox.hpp"

using namespace std;


bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag=false;
    for(const auto& m:members)
    {
        if(m->intersect(r,h,tmin))
            flag=true;
    }
    return flag;
}

void Group::paint()
{
    for(const auto& m:members)
    {
        m->paint();
    }
}

void Group::addObject(int index, Object3D *obj)
{
    members.push_back(obj);
    if(boundingBox == NULL)
    {
        boundingBox = new BoundingBox(obj->getBoundingBox()->getMin(), obj->getBoundingBox()->getMax());
        boundingBox->Print();
    }
    else
    {
        boundingBox->Print();
        if(obj->getBoundingBox() != NULL)
            boundingBox->Extend(obj->getBoundingBox());
    }
}

void Group::insertIntoGrid(Grid *g, Matrix *mat)
{
    for(Object3D* m:members)
    {
        m->insertIntoGrid(g, mat);
    }
}
