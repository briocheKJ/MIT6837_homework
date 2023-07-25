#include "plane.hpp"
#include "../hit.hpp"

Plane::Plane(Vec3f &normal, float d, Material *m)
{
    this->normal=normal;
    normal.Normalize();
    this->d=d;
    material=m;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    if(r.getDirection().Dot3(normal)==0)return false;
    double t=(-r.getOrigin().Dot3(normal)+d)/r.getDirection().Dot3(normal);
    if(t<tmin)return false;
    if(t<h.getT()||h.getMaterial()==NULL)
        h.set(t, material, normal, r);
    return true;
}