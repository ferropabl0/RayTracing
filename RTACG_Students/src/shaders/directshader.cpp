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
    Vector3D L_o = Vector3D(0.0), L_i, w_i, w_o;
    Ray wi;
    int isVisible = 0;

    if (Utils::getClosestIntersection(r, objList, its)) {       // Checking closest intersection with camera ray

        for (int s = 2; s < lsList.size(); s++) {               // Looping through light sources
            
            w_i = (its.itsPoint - lsList[s].getPosition()).normalized();
            w_o = (r.o - its.itsPoint).normalized();
            
            L_i = lsList[s].getIntensity(its.itsPoint);

            wi = Ray(its.itsPoint, w_i);        // Creating the incident light ray
            if (Utils::hasIntersection(wi, objList) == false) {
                isVisible = 1;
                L_o += (L_i * its.shape->getMaterial().getReflectance(its.normal, w_o, w_i) * isVisible);
            }
                
        }
    }

    return L_o;
}
