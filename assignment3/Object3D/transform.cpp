#include "transform.hpp"
#include "../hit.hpp"
#include "../ray.hpp"
#include "GL/gl.h"


bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec4f origin=Vec4f(r.getOrigin().x(),r.getOrigin().y(),r.getOrigin().z(),1);
    Vec4f direction4=Vec4f(r.getDirection().x(),r.getDirection().y(),r.getDirection().z(),0);
    Matrix tmp;
    matrix.Inverse(tmp,1e-3);
    tmp.Transform(origin);
    tmp.Transform(direction4);
    Vec3f direction(direction4.x(),direction4.y(),direction4.z());
    double length=direction.Length();
    direction.Normalize();
    //assert(direction.w()<=0.001);
    //double _t=h.getT();
    h.set(h.getT()*length, h.getMaterial(), h.getNormal(), r);
    bool flag=o->intersect(Ray(Vec3f(origin.x()/origin.w(),origin.y()/origin.w(),origin.z()/origin.w()),
        direction),h,tmin);
    h.set(h.getT()/length, h.getMaterial(), h.getNormal(), r);
    if(flag)
    {
        //if(_t==(double)h.getT())return true;
        //if(_t!=0)cout<<_t<<" "<<h.getT()<<endl;
        Vec4f _normal4(h.getNormal(),0);
        tmp.Transpose();
        tmp.Transform(_normal4);
        Vec3f _normal(_normal4.x(), _normal4.y(), _normal4.z());
        _normal.Normalize();
        h.set(h.getT(), h.getMaterial(), _normal, r);
        return true;
    }
    return false;
}

void Transform::paint(void)
{
	glPushMatrix();
	GLfloat* glMatrix = matrix.glGet();
	glMultMatrixf(glMatrix);
	delete[] glMatrix;
    o->paint();
	glPopMatrix();
	
}