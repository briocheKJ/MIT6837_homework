#include "orthographicCamera.hpp"

OrthographicCamera::OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
{
    this->center = center;
    this->direction = direction; 
    (this->direction).Normalize();
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    Vec3f::Cross3(up, horizon, direction);
    this->up = up; this->up.Normalize();
    this->size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    horizon.Normalize();
    Vec3f o = center + (point.x() - 0.5) * horizon * size + (point.y() - 0.5) * up * size;
    Ray ray(o, direction);
    return ray;
}