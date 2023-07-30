#include "rayTracer.hpp"
#include "vectors.hpp"
#include "Object3D/group.hpp"
#include "hit.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "material.hpp"
#include "input_parser.hpp"
#include "scene_parser.hpp"
#include "material.hpp"
#include "rayTree.hpp"
#include "Object3D/grid.hpp"
//./raytracer -input in/scene2_05_inside_sphere.txt -size 200 200 -output output2_05.tga -shade_back
Vec3f RayTracer::traceRay(Ray &r, float tmin, int bounces, float weight, float indexOfRefraction, Hit &h) const
{
    Vec3f color=scene->getBackgroundColor();
    Object3D *group=scene->getGroup();
    if(InputParser::visualize_grid)
    {
        group = this->grid;
    }
    if(group->intersect(r, h, tmin))
            {
                //std::cout << i << ' ' << j << std::endl;
                //if(i>5)exit(0);
                // cout << h.getMaterial()->getDiffuseColor() << endl;
                //cout<<h.getT()<<endl;
                float index_t = h.getMaterial()->getIndexOfRefraction();
                if(fabs(indexOfRefraction-1)>0.001)
                        index_t=1;
                if(h.getNormal().Dot3(r.getDirection()) >= 0)
                {
                    if(shade_back)
                    {
                        
                        Vec3f normal = h.getNormal();
                        normal.Negate();
                        h.set(h.getT(), h.getMaterial(), normal, r);
                    }
                }
                color = h.getMaterial()->getDiffuseColor();
                Vec3f::Mult(color,color,scene->getAmbientLight());
                int numLights=scene->getNumLights();
                for(int k=0;k<numLights;k++)
                {
                    Light* light=scene->getLight(k);
                    Vec3f lightColor, dir;
                    float distance;
                    light->getIllumination(h.getIntersectionPoint(),dir,lightColor,distance);
                    // Vec3f I;
                    // Vec3f::Mult(I,lightColor,h.getMaterial()->getDiffuseColor());
                    // color+=I*max(0.0f,dir.Dot3(h.getNormal()));
                    if(InputParser::shadows)
                    {
                        Ray ray_to_light(h.getIntersectionPoint(),dir);
                        Hit shadow_hit(EPSILON, nullptr, Vec3f(0, 0, 0));
                        scene->getGroup()->intersect(ray_to_light, shadow_hit, EPSILON);
                        if(shadow_hit.getMaterial()==nullptr||shadow_hit.getT()>distance)
                            RayTree::AddShadowSegment(ray_to_light, RayTracer::EPSILON, distance);
                        else
                        {
                            RayTree::AddShadowSegment(ray_to_light, 0, shadow_hit.getT());
                            continue;
                            //cout<<shadow_hit.getT()<<endl;
                        } 
                    }
                    color += h.getMaterial()->Shade(r, h, dir, lightColor);
                }
                //cout<<h.getNormal()<<endl;
                if (bounces < max_bounces && weight >= cutoff_weight)
                {
                    Vec3f r_color=h.getMaterial()->getReflectiveColor();
                    //cout<<h.getMaterial()->getReflectiveColor().Length()<<endl;
                    //assert(r_color.Length()<=1.01);
                    if (r_color.Length()>0)
                    {
                        Ray reflectionRay(h.getIntersectionPoint(), mirrorDirection(h.getNormal(), r.getDirection()));
                        Hit reflection_hit(INFINITY, nullptr, Vec3f(0, 0, 0));
                        //Vec3f _color=color;
                        color += weight * r_color * traceRay(reflectionRay, RayTracer::EPSILON, bounces + 1, weight*r_color.Length()/sqrt(3), 
                            indexOfRefraction, reflection_hit);
                        //cout<<"reflect: "<<color-_color<<endl;;
                        RayTree::AddReflectedSegment(reflectionRay, 0, reflection_hit.getT());
                    }

                    Vec3f t_color=h.getMaterial()->getTransparentColor();
                    //assert(r_color.Length()<=1.01);
                    if (t_color.Length()>0)
                    {
                        Vec3f transmitted_direction;
                        if(transmittedDirection(h.getNormal(), -1 * r.getDirection(),indexOfRefraction, index_t,transmitted_direction))
                        {
                            Ray transparentRay(h.getIntersectionPoint(), transmitted_direction);
                            Hit transparent_hit(INFINITY, nullptr, Vec3f(0, 0, 0));
                            //Vec3f _color=color;
                            color += weight * t_color * traceRay(transparentRay, RayTracer::EPSILON, bounces + 1, weight*t_color.Length()/sqrt(3), 
                                index_t, transparent_hit);
                            //cout<<"transparent: "<<color-_color<<endl;;
                            RayTree::AddTransmittedSegment(transparentRay, 0, transparent_hit.getT());
                        }
                    }
                }
            }
    //cout<<color<<" "<<weight<<" "<<bounces<<" "<<indexOfRefraction<<endl;
    return color;
}

Vec3f RayTracer::mirrorDirection(const Vec3f& normal, const Vec3f& incoming) const
{
	Vec3f mirriorDir = incoming - 2 * incoming.Dot3(normal) * normal;
	mirriorDir.Normalize();
	return mirriorDir;
}

bool RayTracer::transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t,Vec3f &transmitted) const
{

    float cosAlpha = incoming.Dot3(normal);
    Vec3f horizon = -cosAlpha * normal + incoming; //入射光水平分量(反向)，长度等于sin alpha
    float sinAlpha = horizon.Length();
    float sinBeta = sinAlpha * index_i / index_t;

    if (abs(sinBeta) > 1) // 全反射
        return false;

    float cosBeta = sqrt(1 - sinBeta * sinBeta);
    horizon.Normalize();//单位水平向量（反向）
    if (cosAlpha > 0) // 外部入射
        transmitted = -cosBeta * normal - sinBeta * horizon;
    else //内部入射
        transmitted = cosBeta * normal - sinBeta * horizon;

    return true;
}