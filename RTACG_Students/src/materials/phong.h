#ifndef PHONG
#define PHONG

#include "material.h"


class Phong : public Material
{
public:
    Phong();
    Phong(const Vector3D d_coeff_, const Vector3D s_coeff_, const double alpha_);

    virtual Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;

private:
    
    Vector3D d_coeff;
    Vector3D s_coeff;
    double alpha;
};


#endif // PHONG