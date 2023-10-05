#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

NormalShader::NormalShader(Vector3D hitColor_, Vector3D bgColor_) :
    bgColor(bgColor_), hitColor(hitColor_)
{ }

Vector3D NormalShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
    Intersection its; 
    Vector3D c_i;

    if (Utils::getClosestIntersection(r, objList, its)) {

        c_i = (its.normal + (1.0,1.0,1.0))/2.0;

        return c_i;
    }

    return Vector3D(0.0,0.0,0.0);
}
