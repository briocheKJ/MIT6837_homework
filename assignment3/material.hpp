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

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

class PhongMaterial: public Material
{
public:
    PhongMaterial(const Vec3f &diffuseColor, 
        const Vec3f &specularColor, float exponent)
        :Material(diffuseColor){
          this->specularColor=specularColor;
          this->exponent=exponent;
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