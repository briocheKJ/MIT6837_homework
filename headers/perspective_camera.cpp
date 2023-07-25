#include "perspective_camera.hpp"

PerspectiveCamera::PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float angle)
{
    this->center = center;
    this->direction = direction; 
    (this->direction).Normalize();
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    Vec3f::Cross3(up, horizon, direction);
    this->up = up; this->up.Normalize();
    this->angle = angle;
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f horizon;
    Vec3f::Cross3(horizon, direction, up);
    horizon.Normalize();
    float dis_to_screen = 1.0 / tan(angle / 2);
    Vec3f r_dir = direction * dis_to_screen + (point.x() - 0.5) * horizon * 2  + (point.y() - 0.5) * up * 2;
    r_dir.Normalize();
    Ray ray(center, r_dir);
    return ray;
}