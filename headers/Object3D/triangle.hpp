#pragma once

#include "object3D.hpp"
#include "vectors.hpp"

class Triangle: public Object3D
{
public:
    Vec3f a,b,c,normal;
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
    {
        //cout<<"start build triangle: "<<a<<" "<<b<<" "<<c<<endl;
        this->a=a;
        this->b=b;
        this->c=c;
        material=m;
        Vec3f::Cross3(normal,b-a,c-a);
        normal.Normalize();
    }
    virtual bool intersect(const Ray &r, Hit &h, float tmin);  
};