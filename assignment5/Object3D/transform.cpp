#include "transform.hpp"
#include "../hit.hpp"
#include "../ray.hpp"
#include "GL/gl.h"
#include "../boundingbox.hpp"
#include "triangle.hpp"

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

Transform::Transform(Matrix &m, Object3D *o)
{
    matrix=m;
    this->o=o;
    Matrix minv,minvT;
    m.Inverse(minv);
    minv.Transpose(minvT);

	//calculate bounding box
	if (typeid(*o) == typeid(Triangle)) {
		//special case for triangles to get tighter bounding boxes
		Triangle *tri = (Triangle*)o;
		Vec3f p, p2, p3;
		p  = tri->a;
        p2  = tri->b;
        p3  = tri->c;
		m.Transform(p);
		m.Transform(p2);
		m.Transform(p3);
		boundingBox = new BoundingBox(p, p);
		boundingBox->Extend(p2);
		boundingBox->Extend(p3);
	}
	else {
		//general code based on transforming the bounding box
		BoundingBox *pbb0 = o->getBoundingBox();
		Vec3f dia = pbb0->getMax() - pbb0->getMin();
		Vec3f edges[3];
		edges[0] = Vec3f(dia.x(), 0, 0);
		edges[1] = Vec3f(0, dia.y(), 0);
		edges[2] = Vec3f(0, 0, dia.z());
		Vec3f mvmin = pbb0->getMin();
		m.Transform(mvmin);
		boundingBox = new BoundingBox(mvmin, mvmin);
		for (int i = 1; i < 8; i++) {
			Vec3f vertex = pbb0->getMin();
			for (int j = 0; j < 3; j++) {
				if ((1 << j)&i) {
					vertex += edges[j];
				}
			}
			//here for simplicity I use vec3f to transform
			m.Transform(vertex);
			boundingBox->Extend(vertex);
		}
	}
}

void Transform::insertIntoGrid(Grid * g, Matrix * m)
{
    if (m==NULL)
    {
        Matrix newm = matrix;
        o->insertIntoGrid(g, &newm);
    }
    else
    {
        Matrix newm = (*m) * matrix;
        o->insertIntoGrid(g, &newm);
    }
    
}