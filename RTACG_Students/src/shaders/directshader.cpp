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
    Vector3D L_o = Vector3D(0.0), L_i, w_i, w_o, w_r;
    Ray wi;
    //int isVisible = 0;

    if (Utils::getClosestIntersection(r, objList, its)) {       // Checking closest intersection with camera ray

        w_o = (r.o - its.itsPoint).normalized();

        for (int s = 0; s < lsList.size(); s++) {               // Looping through light sources
            
            if (its.shape->getMaterial().hasSpecular()) {
                w_r = its.normal * (2 * dot(its.normal, w_o)) - w_o;
                wi = Ray(its.itsPoint, w_r, r.depth + 1);
                L_o = computeColor(wi, objList, lsList);
            }
            else if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
                w_i = (lsList[s].getPosition() - its.itsPoint).normalized();

                L_i = lsList[s].getIntensity(its.itsPoint);

                wi = Ray(its.itsPoint, w_i, 0, Epsilon, (its.itsPoint - lsList[s].getPosition()).length());
                if (Utils::hasIntersection(wi, objList) == false) {
                    L_o += (L_i * its.shape->getMaterial().getReflectance(its.normal, w_o, w_i));
                }
            }
            else if (its.shape->getMaterial().hasTransmission()) {

            }
                
        }

        return L_o;
    }

    return bgColor;
}
