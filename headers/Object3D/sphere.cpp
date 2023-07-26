#include <iostream>
#include <cmath>

#include "sphere.hpp"
#include "../ray.hpp"
#include "../hit.hpp"

using namespace std;

Sphere::Sphere(Vec3f c, double r, Material *m)
{
    cout << "sphere init: " <<"r="<<r<<" c="<<c.x()<<" "<<c.y()<< endl;
    radius = r;
    center = c;
    material = m;
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f o=r.getOrigin();
    Vec3f d=r.getDirection();
    //cout<<"start judge whether the ray: "<<r<<" intersect with the sphere"<<endl;
    Vec3f tmp=center-o;
    double proj=d.Dot3(tmp);
    double dis =sqrt(tmp.Dot3(tmp)-proj*proj);
    //cout<<tmp<<" "<<d<<" "<<dis<<" "<<radius<<endl;
    if(dis>radius) return false;
    double half_xian=sqrt(radius*radius-(tmp.Dot3(tmp)-proj*proj));//sqrt(r^2-dis^2)
    
    double t1 = proj-half_xian;
    double t2 = proj+half_xian;
    double t;
    if(t1<tmin&&t2>tmin)
        t=t2;
    else t=t1;
    if(t<tmin)return false;
    Vec3f n=r.pointAtParameter(t)-center;
    //assert(r.getDirection().Dot3(r.getDirection())<=1.001);
    n.Normalize();
    //cout<<t<<endl;
    //cout<<r.getOrigin();
    //cout<<n<<endl;cout<<r.pointAtParameter(t)<<endl;exit(0);
    if(t<h.getT()||h.getMaterial()==NULL)
    {
        h.set(t, material, n, r);
        return true;
    }
    return false;
    //cout<<proj<<" "<<half_xian<<endl;
    //cout<<"!"<<endl;
}