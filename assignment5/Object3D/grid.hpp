#ifndef _GRID_H
#define _GRID_H

#include "object3D.hpp"
#include "../material.hpp"
#include <vector>

struct MarchingInfo
{
    float tmin;
    int i, j, k;
    Vec3f ri;
    Vec3f dis;
    Vec3f nor;
    Vec3f rs;
    Vec3f pos;
    bool sign_x, sign_y, sign_z;

    void Print()
    {
        cout << "MarchInfo:" << endl;
        cout << i << ' ' << j << ' ' << k << endl;
        cout << tmin << endl;
        cout << "ri: " << ri << endl;
        cout << "dis: " << dis << endl;
        cout << "nor: " << nor << endl;
        cout << "rs: " << rs << endl;
        cout << "pos: " << pos << endl;
        cout << sign_x << ' ' << sign_y <<  ' ' << sign_z << endl << endl;
    }

    void nextCell()
    {
        // https://www.shadertoy.com/view/4dfGzs
        // RayTracingStats::IncrementNumGridCellsTraversed();
        Vec3f dire = Vec3f(dis.x() < min(dis.y(), dis.z()),
                           dis.y() < min(dis.x(), dis.z()),
                           dis.z() < min(dis.x(), dis.y()));
        
        // 轴平行
        if (dire.Length() < 0.001 || dire.Length() > 1.1)
        {
            dis -= Vec3f(rand(), rand(), rand()) * 0.001 * Vec3f(abs(ri.x()) < 100, abs(ri.y()) < 100, abs(ri.z()) < 100);
            dire = Vec3f(dis.x() < min(dis.y(), dis.z()), dis.y() < min(dis.x(), dis.z()), dis.z() < min(dis.x(), dis.y()));
        }
        if (dire.Length() < 0.001 || dire.Length() > 1.1)
        {
            dis += Vec3f(rand(), rand(), rand()) * 0.001;
            dire = Vec3f(dis.x() < min(dis.y(), dis.z()), dis.y() < min(dis.x(), dis.z()), dis.z() < min(dis.x(), dis.y()));
        }

        nor = dire * rs * (-1);
        dis += dire * rs * ri;
        pos += dire * rs;
        
        i = pos.x();
        j = pos.y();
        k = pos.z();

    }
};

class Grid : public Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    Grid() {}
    ~Grid() {}

    Grid(BoundingBox *bb, int nx, int ny, int nz)
    {
        this->nx = nx;
        this->ny = ny;
        this->nz = nz;
        object3ds.resize(nx * ny * nz, vector<Object3D*>());
        isOpaque.resize(nx * ny * nz, false);
        boundingBox = bb;
        Vec3f zero=Vec3f(0,0,0);
        material = new PhongMaterial(Vec3f(0.8,0.8,0.8),zero,0,zero,zero,0);
        // countColor.resize(12);
        countColor.push_back(new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(1, 1, 1), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(1, 0, 0), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(1, 1, 0), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0, 1, 0), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0, 1, 1), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0, 0, 1), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(1, 0, 1), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0.5, 0.5, 0.5), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0.8, 0.8, 0.8), Vec3f(0,0,0), 0,zero,zero,0));
        countColor.push_back(new PhongMaterial(Vec3f(0, 0, 0), Vec3f(0,0,0), 0,zero,zero,0));
    }

    Vec3f getGirdSize()
    {
        return Vec3f(nx, ny, nz);
    }

    int getID(int i, int j, int k)
    {
        int id = (i * ny + j) * nz + k;
        return id;
    }

    
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

    void insertInto(int id, Object3D *obj);
    void insertInto(int i, int j, int k, Object3D *obj);

    void getIndexFromCoord(Vec3f coord, int &i, int &j, int &k) const;

private:
    int nx;
    int ny;
    int nz;
    vector<bool> isOpaque;
    vector<vector<Object3D *> > object3ds;
    bool visualize;
    vector<Material*> countColor;
};

#endif