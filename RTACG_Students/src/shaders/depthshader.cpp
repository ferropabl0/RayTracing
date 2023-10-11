#include "depthshader.h"
#include "../core/utils.h"
#include<cmath>

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), maxDist(maxDist_), color(color_)
{ }

Vector3D DepthShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    float c_i;

    if (Utils::getClosestIntersection(r, objList, its)) {

        c_i = std::max(1.0-(double)(r.maxT/maxDist), 0.0);

        return Vector3D(0.0, c_i, 0.0);
    }

    return bgColor;
}
