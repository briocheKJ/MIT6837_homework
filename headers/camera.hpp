#pragma once

#include "vectors.hpp"
#include "ray.hpp"

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};