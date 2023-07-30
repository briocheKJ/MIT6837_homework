#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.hpp"

class Ray;
class Hit;

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f Shade
      (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
      const Vec3f &lightColor) const = 0;
  virtual void glSetMaterial(void) const = 0;

  Vec3f getTransparentColor(){return transparentColor;}
  Vec3f getReflectiveColor(){return reflectiveColor;}
  float getIndexOfRefraction(){return indexOfRefraction;}

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f transparentColor,reflectiveColor;
  float indexOfRefraction;
};

class PhongMaterial: public Material
{
public:
    //PhongMaterial(const Vec3f &diffuseColor):Material(diffuseColor){}
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, 
                  float exponent, const Vec3f &reflectiveColor, 
                  const Vec3f &transparentColor, float indexOfRefraction)
    :Material(diffuseColor){
        this->specularColor=specularColor;
        this->exponent=exponent;
        this->transparentColor=transparentColor;
        this->reflectiveColor=reflectiveColor;
        this->indexOfRefraction=indexOfRefraction;
    }
    virtual Vec3f Shade
        (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
        const Vec3f &lightColor) const;
    virtual void glSetMaterial(void) const override;

    Vec3f getSpecularColor()const{return specularColor;}
protected:
    Vec3f specularColor;
    float exponent;
    
};

// ====================================================================
// ====================================================================

#endif