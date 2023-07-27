#pragma once

#include "vectors.hpp"
#include "ray.hpp"

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

const float O_CAMERA_T_MIN=-1e9;

class OrthographicCamera: public Camera
{
public:
    OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const{return O_CAMERA_T_MIN;}

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

public:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float size;
};

const float P_CAMERA_T_MIN=0;

class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const{return P_CAMERA_T_MIN;}

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

public:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float angle;
};