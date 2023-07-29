#include <iostream>
#include <cmath>

#include "sphere.hpp"
#include "../ray.hpp"
#include "../hit.hpp"
#include "../material.hpp"
#include "../matrix.hpp"
#include "../input_parser.hpp"

#include "GL/gl.h"

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

Vec3f Sphere::getPoint(float phi, float theta) const
{
    Vec3f p(0, -radius, 0);
    Matrix mat=Matrix::MakeYRotation(-M_PI*phi/180) * Matrix::MakeXRotation(-M_PI*theta/180);
    mat.Transform(p);
    return center + p;
}

void Sphere::paint(void) 
{
    Vec3f p1, p2, p3, p4, n;
	glBegin(GL_QUADS);
	material->glSetMaterial();
	int deltaPhi = (int)(180.f / InputParser::phiStep), deltaTheta = (int)(360.f / InputParser::thetaStep);//向下取整
	for (int iPhi = - 90; iPhi < 90; iPhi += deltaPhi)//纬线
	{
		for (int iTheta = 0; iTheta < 360; iTheta += deltaTheta)//经线
		{
			// compute appropriate coordinates & normals
			p1 = getPoint(iPhi, iTheta);
			p2 = getPoint(iPhi, iTheta + deltaTheta);
			p3 = getPoint(iPhi + deltaPhi, iTheta );
			p4 = getPoint(iPhi + deltaPhi, iTheta + deltaTheta);
			// send gl vertex commands
			if (InputParser::gouraud)
			{
				n = p1 - center;n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p1.x(), p1.y(), p1.z());
				n = p2 - center;n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p2.x(), p2.y(), p2.z());
				n = p4 - center;n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p4.x(), p4.y(), p4.z());
				n = p3 - center;n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p3.x(), p3.y(), p3.z());
				
			}
			else
			{
				Vec3f::Cross3(n, p2 - p3, p4 - p1);//对角线一定不平行
				n.Normalize();
				glNormal3f(n.x(), n.y(), n.z());
				glVertex3f(p1.x(), p1.y(), p1.z());
				glVertex3f(p2.x(), p2.y(), p2.z());
				glVertex3f(p4.x(), p4.y(), p4.z());
				glVertex3f(p3.x(), p3.y(), p3.z());
			}
		}
	}
	glEnd();
}