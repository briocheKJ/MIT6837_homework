#include "transform.hpp"
#include "../hit.hpp"
#include "../ray.hpp"


bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec4f origin=Vec4f(r.getOrigin().x(),r.getOrigin().y(),r.getOrigin().z(),1);
    Vec4f direction=Vec4f(r.getDirection().x(),r.getDirection().y(),r.getDirection().z(),0);
    Matrix tmp;
    matrix.Inverse(tmp,1e-3);
    tmp.Transform(origin);
    tmp.Transform(direction);
    
    return o->intersect(Ray(Vec3f(origin.x()/origin.w(),origin.y()/origin.w(),origin.z()/origin.w()),
        Vec3f(direction.x(),direction.y(),direction.z())),h,tmin);
}