#include "triangle.hpp"
#include "../hit.hpp"
#include "../matrix.hpp"
#include "../material.hpp"
#include <GL/gl.h>
#include "../boundingbox.hpp"
#include "grid.hpp"

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

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
{
    //cout<<"start build triangle: "<<a<<" "<<b<<" "<<c<<endl;
    this->a=a;
    this->b=b;
    this->c=c;
    material=m;
    Vec3f::Cross3(normal,b-a,c-a);
    normal.Normalize();
    boundingBox = new BoundingBox(a, a);
    boundingBox->Extend(b);
    boundingBox->Extend(c);
}

void Triangle::insertIntoGrid(Grid* g, Matrix* m)
{
    if (m) {
		this->Object3D::insertIntoGrid(g, m);
		return;
	}
    //g->insertMatrix(this, m);
    // cout << "tri insertIntoGrid" << endl;
    Vec3f vec[3];
    vec[0] = a;
    vec[1] = b;
    vec[2] = c;
    if (m)
    {
        m->Transform(vec[0]);
        m->Transform(vec[1]);
        m->Transform(vec[2]);
    }
    BoundingBox *bb = new BoundingBox(vec[0], vec[0]);
    bb->Extend(vec[1]);
    bb->Extend(vec[2]);

    BoundingBox *gb = g->getBoundingBox();
    Vec3f minimum = bb->getMin() - gb->getMin();
    Vec3f maximum = bb->getMax() - gb->getMin() + Vec3f(1e-4,1e-4,1e-4);

    Vec3f block = g->getBoundingBox()->getMax() - g->getBoundingBox()->getMin();
    Vec3f gridSize = g->getGirdSize();
    block.Divide(gridSize.x(), gridSize.y(), gridSize.z());
    minimum.Divide(block.x(), block.y(), block.z());
    maximum.Divide(block.x(), block.y(), block.z());

    //maximum += Vec3f(EPLISON, EPLISON, EPLISON);
    //minimum -= Vec3f(EPLISON, EPLISON, EPLISON);

    for (int i = max(0.f, (float)floor(minimum.x())); i < min(1.f * gridSize.x(), (float)ceil(maximum.x())); i++)
    {
        for (int j = max(0.f, (float)floor(minimum.y())); j < min(1.f * gridSize.y(), (float)ceil(maximum.y())); j++)
        {
            for (int k = max(0.f, (float)floor(minimum.z())); k < min(1.f * gridSize.z(), (float)ceil(maximum.z())); k++)
            {
                //cout<<1<<endl;
                g->insertInto(i, j, k, this);
            }
        }
    }
    delete bb;
}