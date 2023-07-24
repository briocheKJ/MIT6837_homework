#pragma once

#include "camera.hpp"
#include "vectors.hpp"

const float O_CAMERA_T_MIN=-1e9;

class OrthographicCamera: public Camera
{
public:
    OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
    virtual Ray generateRay(Vec2f point);
    virtual float getTMin() const{return O_CAMERA_T_MIN;}

public:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float size;
};