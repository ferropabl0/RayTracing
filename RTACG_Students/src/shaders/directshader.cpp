#include "directshader.h"
#include "../core/utils.h"
#include "../materials/phong.h"


DirectShader::DirectShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

DirectShader::DirectShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D DirectShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    Vector3D L_o = Vector3D(0.0), L_i, w_i;
    Phong ph = Phong(0.3, 0.4, 0.5);

    for (int s = 0; s < lsList.size(); s++) {
        if (Utils::getClosestIntersection(r, objList, its)) {
            L_i = lsList[s].getIntensity(its.itsPoint);
            w_i = its.itsPoint - lsList[s].getPosition();
            L_o += L_i * ph.getReflectance(its.normal, r.d, w_i);
        }
    }

    return L_o;
}
