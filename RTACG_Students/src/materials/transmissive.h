#ifndef TRANSMISSIVE
#define TRANSMISSIVE

#include "material.h"


class Transmissive : public Material
{
public:
    Transmissive(const double u_t_, const Vector3D s_coeff_);

    virtual Vector3D getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const;

    virtual bool hasSpecular() const;
    virtual bool hasTransmission() const;
    virtual bool hasDiffuseOrGlossy() const;
    virtual double getIndexOfRefraction() const;

private:
    double u_t;
    Vector3D s_coeff;
};


#endif // PHONG