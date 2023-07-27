#include "plane.hpp"
#include "../hit.hpp"
#include "../material.hpp"

#include "GL/gl.h"

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

void Plane::paint(void)
{
	//cout<<1<<endl;
    Vec3f v=Vec3f(1, 0, 0);
    if(normal.Dot3(Vec3f(1, 0, 0)) == 1)
        v = Vec3f(0, 1, 0);
	Vec3f center, b1, b2;
	Vec3f::Cross3(b1, v, normal);
	Vec3f::Cross3(b2, normal, b1);
	float big = 5e5;
	b1.Normalize();b2.Normalize();
	center = normal * d;

    Vec3f x1,x2,x3,x4;
	x1 = center - big * b1 - big * b2;
	x2 = center - big * b1 + big * b2;
	x3 = center + big * b1 + big * b2;
	x4 = center + big * b1 - big * b2;
	
	glBegin(GL_QUADS);
	material->glSetMaterial();
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(x1.x(), x1.y(), x1.z());
	glVertex3f(x2.x(), x2.y(), x2.z());
	glVertex3f(x3.x(), x3.y(), x3.z());
	glVertex3f(x4.x(), x4.y(), x4.z());
	glEnd();
}