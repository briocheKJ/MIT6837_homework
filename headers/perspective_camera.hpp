#pragma once

#include "camera.hpp"
#include "vectors.hpp"

const float P_CAMERA_T_MIN=0;

class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const{return P_CAMERA_T_MIN;}

public:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float angle;
};