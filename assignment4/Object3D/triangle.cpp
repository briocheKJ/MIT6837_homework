#include "triangle.hpp"
#include "../hit.hpp"
#include "../matrix.hpp"
#include "../material.hpp"
#include <GL/gl.h>

float det3x3(float a1,float a2,float a3,
	     float b1,float b2,float b3,
	     float c1,float c2,float c3);
float det2x2(float a, float b,
	     float c, float d);

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    float belta, gamma, t;
    float A;
    A = det3x3(
        a.x() - b.x(), a.x() - c.x(), r.getDirection().x(),
        a.y() - b.y(), a.y() - c.y(), r.getDirection().y(),
        a.z() - b.z(), a.z() - c.z(), r.getDirection().z()
    );
 
    belta = det3x3(
        a.x() - r.getOrigin().x(), a.x() - c.x(), r.getDirection().x(),
        a.y() - r.getOrigin().y(), a.y() - c.y(), r.getDirection().y(),
        a.z() - r.getOrigin().z(), a.z() - c.z(), r.getDirection().z()
    );
    gamma = det3x3(
        a.x() - b.x(), a.x() - r.getOrigin().x(), r.getDirection().x(),
        a.y() - b.y(), a.y() - r.getOrigin().y(), r.getDirection().y(),
        a.z() - b.z(), a.z() - r.getOrigin().z(), r.getDirection().z()
    );
    t = det3x3(
        a.x() - b.x(), a.x() - c.x(), a.x() - r.getOrigin().x(),
        a.y() - b.y(), a.y() - c.y(), a.y() - r.getOrigin().y(),
        a.z() - b.z(), a.z() - c.z(), a.z() - r.getOrigin().z()
    );

    belta /= A;
    gamma /= A;
    t /= A;
 
    if (belta > 1 || belta < 0) return false;
    if (gamma > 1 || gamma < 0) return false;
    if (1 - belta - gamma > 1 || 1 - belta - gamma < 0) return false;

    if(t<tmin)return false;
    if (t < h.getT()||h.getMaterial()==NULL)
    {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}

void Triangle::paint()
{
    glBegin(GL_TRIANGLES);	
	material->glSetMaterial();
    glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}