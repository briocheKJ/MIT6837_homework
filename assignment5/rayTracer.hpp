#pragma once
#include "vectors.hpp"
class SceneParser;
class Ray;
class Hit;
class Grid;

class RayTracer
{
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, bool shade_back, Grid* grid)
        : scene(s), max_bounces(max_bounces), cutoff_weight(cutoff_weight), shadows(shadows), shade_back(shade_back),grid(grid){}
    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;
    Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const;
	bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t,Vec3f &transmitted) const;
    
public:
    SceneParser* scene;
    int max_bounces;
    float cutoff_weight;
    bool shadows;
    bool shade_back;
    const float EPSILON=1e-4;
    Grid* grid;
};