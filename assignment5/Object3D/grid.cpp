#include "grid.hpp"
#include "../rayTree.hpp"
#include "../hit.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define EPLISON 0.00001


void Grid::insertInto(int i, int j, int k, Object3D *obj)
{
    int id = getID(i, j, k);
    insertInto(id, obj);
}

void Grid::insertInto(int id, Object3D *obj)
{
    isOpaque[id] = true;
    object3ds[id].push_back(obj);
    // object3ds[id] = obj;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    //cout << "grid intersect" << endl;
    MarchingInfo mi;
    
    int cnt=0;
    initializeRayMarch(mi, r, tmin);
    bool flag=false;
    while(1)
    {
        cnt++;

        if (mi.i < 0 || mi.i >= nx) break;
        if (mi.j < 0 || mi.j >= ny) break;
        if (mi.k < 0 || mi.k >= nz) break;
        
        int id = getID(mi.i, mi.j, mi.k);
        bool temp = isOpaque[id];

        Vec3f block = boundingBox->getMax() - boundingBox->getMin();
        block.Divide(nx, ny, nz);
        Vec3f line = block * 0.5;

        static Vec3f points[] = {Vec3f(1, -1, -1), Vec3f(1, 1, -1), Vec3f(-1, 1, -1), Vec3f(-1, -1, -1),
                                 Vec3f(1, -1, 1), Vec3f(1, 1, 1), Vec3f(-1, 1, 1), Vec3f(-1, -1, 1)};
        static int index[6][4] = {{0, 1, 5, 4}, {2, 3, 7, 6}, {1, 2, 6, 5}, {3, 0, 4, 7}, {4, 5, 6, 7}, {3, 2, 1, 0}};
        static Vec3f nor[] = {Vec3f(1, 0, 0), Vec3f(-1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, -1, 0),
                              Vec3f(0, 0, 1), Vec3f(0, 0, -1)};

        Vec3f centerpoint = boundingBox->getMin() + (Vec3f(mi.i, mi.j, mi.k) + Vec3f(0.5, 0.5, 0.5)) * block;
        // 碰撞网格
        for (int face = 0; face < 6; face++)
        {
            vector<Vec3f> faces;
            for (int i = 0; i < 4; i++)
            {
                Vec3f t = points[index[face][i]];
                t = t * line + centerpoint;
                faces.push_back(t);
            }
            Vec3f normal = nor[face];
            RayTree::AddHitCellFace(faces[0], faces[1], faces[2], faces[3], normal, countColor[cnt%11]);
        }

        // 碰撞面

        int hitFace = mi.nor.Dot3(Vec3f(1, 3, 5));
        if (hitFace < 0)
            hitFace = -hitFace;
        else
            hitFace -= 1;
        hitFace = max(min(hitFace, 5), 0);
        vector<Vec3f> faces;
        for (int i = 0; i < 4; i++)
        {
           Vec3f t = points[index[hitFace][i]];
            t = t * line + centerpoint;
            faces.push_back(t);
        }
        RayTree::AddEnteredFace(faces[0], faces[1], faces[2], faces[3], mi.nor, countColor[cnt%11]);
        if (temp&&!flag)
        {
            int size = object3ds[id].size();
            size = min(size, 10);
            h.set(200, countColor[size], mi.nor, r);
            flag=true;
        }
        mi.nextCell();
    }
    return flag;
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{
    Vec3f t1 = (boundingBox->getMax() - r.getOrigin());
    t1.Divide(r.getDirection().x(), r.getDirection().y(), r.getDirection().z());
    Vec3f t2 = (boundingBox->getMin() - r.getOrigin());
    t2.Divide(r.getDirection().x(), r.getDirection().y(), r.getDirection().z());

    float txmin = min(t1.x(), t2.x());
    float txmax = max(t1.x(), t2.x());
    float tymin = min(t1.y(), t2.y());
    float tymax = max(t1.y(), t2.y());
    float tzmin = min(t1.z(), t2.z());
    float tzmax = max(t1.z(), t2.z());

    float allMin = max(txmin, max(tymin, tzmin));
    float allMax = min(txmax, min(tymax, tzmax));
    //cout << "allMin Max: " << allMin << ' ' << allMax << endl;
    if (allMin > allMax)
    {
        // 没有相交
    }

    Vec3f block = boundingBox->getMax() - boundingBox->getMin();
    block.Divide(nx, ny, nz);
    //cout << "tmin allMin: " << tmin << ' ' << allMin << endl;
    //cout << "block:" << block << endl;
    mi.tmin = max(tmin, allMin);
    //cout << ".pointAtParameter(mi.tmin):" << r.pointAtParameter(mi.tmin) << endl;
    Vec3f pos = (r.pointAtParameter(mi.tmin) - boundingBox->getMin());
    //cout << "before pos:" << pos << endl;
    pos.Divide(block.x(), block.y(), block.z());
    //cout << "after pos:" << pos << endl;
    if(pos.x() < EPLISON)
    {
        pos += Vec3f(EPLISON, 0, 0);
    }
    if(pos.x() > nx - EPLISON)
    {
        pos -= Vec3f(EPLISON, 0, 0);
    }
    if(pos.y() < EPLISON)
    {
        pos += Vec3f(0, EPLISON, 0);
    }
    if(pos.y() > ny - EPLISON)
    {
        pos -= Vec3f(0, EPLISON, 0);
    }
    if(pos.z() < EPLISON)
    {
        pos += Vec3f( 0, 0,EPLISON);
    }
    if(pos.z() > nz - EPLISON)
    {
        pos -= Vec3f(0, 0, EPLISON);
    }


    mi.sign_x = r.getDirection().x() > 0;
    mi.sign_y = r.getDirection().y() > 0;
    mi.sign_z = r.getDirection().z() > 0;

    mi.pos = Vec3f(floor(pos.x()), floor(pos.y()), floor(pos.z()));
    mi.rs = Vec3f(mi.sign_x, mi.sign_y, mi.sign_z) * 2.0f - Vec3f(1, 1, 1);

    Vec3f vel = r.getDirection();
    vel.Divide(block.x(), block.y(), block.z());
    vel.Normalize();

    // 防止平行
    if (vel.x() == 0)
    {
        vel += Vec3f(0.0000001, 0, 0) * mi.rs.x();
    }
    if (vel.y() == 0)
    {
        vel += Vec3f(0, 0.0000001, 0) * mi.rs.y();
    }
    if (vel.z() == 0)
    {
        vel += Vec3f(0, 0, 0.0000001) * mi.rs.z();
    }

    mi.ri = Vec3f(1, 1, 1);
    mi.ri.Divide(vel.x(), vel.y(), vel.z());

    mi.dis = (mi.pos - pos + Vec3f(0.5, 0.5, 0.5) + 0.5 * mi.rs) * mi.ri;
    {
        Vec3f normal = Vec3f(0, 0, 0);
        if (txmin + 0.0001 > allMin)
            normal += !mi.sign_x ? Vec3f(1, 0, 0) : Vec3f(-1, 0, 0);
        if (tymin + 0.0001 > allMin)
            normal += !mi.sign_y ? Vec3f(0, 1, 0) : Vec3f(0, -1, 0);
        if (tzmin + 0.0001 > allMin)
            normal += !mi.sign_z ? Vec3f(0, 0, 1) : Vec3f(0, 0, -1);
        mi.nor = normal;
    }
    mi.i = mi.pos.x();
    mi.j = mi.pos.y();
    mi.k = mi.pos.z();
    // mi.updateijk();
}

void Grid::paint()
{
    // cout << "grid paint" << endl;
    this->material->glSetMaterial();

    static Vec3f points[] = {Vec3f(1, -1, -1), Vec3f(1, 1, -1), Vec3f(-1, 1, -1), Vec3f(-1, -1, -1),
                             Vec3f(1, -1, 1), Vec3f(1, 1, 1), Vec3f(-1, 1, 1), Vec3f(-1, -1, 1)};
    static int index[6][4] = {{0, 1, 5, 4}, {2, 3, 7, 6}, {1, 2, 6, 5}, {3, 0, 4, 7}, {4, 5, 6, 7}, {3, 2, 1, 0}};
    static Vec3f norms[] = {Vec3f(1, 0, 0), Vec3f(-1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, -1, 0),
                            Vec3f(0, 0, 1), Vec3f(0, 0, -1)};

    // cout << boundingBox << endl;
    Vec3f r = boundingBox->getMax() - boundingBox->getMin();
    Vec3f block(r);
    block.Divide(nx, ny, nz);
    Vec3f line = block * 0.5;

    for (int i = 0; i < nx; i++)
    {
        for (int j = 0; j < ny; j++)
        {
            for (int k = 0; k < nz; k++)
            {
                Vec3f gridCenter = boundingBox->getMin() + (Vec3f(i, j, k) + Vec3f(0.5, 0.5, 0.5)) * block;
                // int id = (_i * y + _j) * z + _k;
                int id = getID(i, j, k);
                // cout << object3ds[id].size() << '\t';

                if (isOpaque[id])
                {
                    int size = object3ds[id].size();
                    size = min(size, 10);
                    
                    countColor[size]->glSetMaterial();
                    // indicatesMaterial[size - 1]->glSetMaterial();
                    glBegin(GL_QUADS);
                    for (int face = 0; face < 6; face++)
                    {
                        //cout << i << ' ' << j << ' ' << k << " " << face << endl;
                        glNormal3f(norms[face].x(), norms[face].y(), norms[face].z());
                        // cout << norms[face] << endl;
                        for (int ii = 0; ii < 4; ii++)
                        {
                            Vec3f t = points[index[face][ii]];
                            t = t * line + gridCenter;
                            // cout << t << ' ';
                            glVertex3f(t.x(), t.y(), t.z());
                        }
                        // cout << endl;
                    }
                    glEnd();
                }
            }
            // cout << endl;
        }
        // cout << endl;
    }
}

void Grid::insertIntoGrid(Grid *g, Matrix *m)
{
}

void Grid::getIndexFromCoord(Vec3f coord, int &i, int &j, int &k) const {
	Vec3f minPos = boundingBox->getMin();
	Vec3f maxPos = boundingBox->getMax();
	Vec3f dia = maxPos - minPos;
	Vec3f tmp = coord - minPos;
    coord.Set((fabs(tmp[0]) < 1e-5)?coord[0] + 1e-5:coord[0],(fabs(tmp[1]) < 1e-5)?coord[1] + 1e-5:coord[1],(fabs(tmp[2]) < 1e-5)?coord[2] + 1e-5:coord[2]);
    tmp = coord - maxPos;
    coord.Set((fabs(tmp[0]) < 1e-5)?coord[0] - 1e-5:coord[0],(fabs(tmp[1]) < 1e-5)?coord[1] - 1e-5:coord[1],(fabs(tmp[2]) < 1e-5)?coord[2] - 1e-5:coord[2]);
	
	float dx = dia.x() / nx;
	float dy = dia.y() / ny;
	float dz = dia.z() / nz;
	i = (int)floor((coord.x() - minPos.x()) / dx);
	j = (int)floor((coord.y() - minPos.y()) / dy);
	k = (int)floor((coord.z() - minPos.z()) / dz);
}