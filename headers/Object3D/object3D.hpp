#pragma once

class Material;
class Ray;
class Hit;

class Object3D 
{
public:
    Material * material;

public:
    virtual ~Object3D(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
};