#include "globalshader.h"
#include "../core/utils.h"


GlobalShader::GlobalShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

GlobalShader::GlobalShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    Vector3D L_o = Vector3D(0.0), L_i, w_i, w_o, w_r, w_t, neg_n;
    Ray wi, refract;
    double root, sin2alpha, u_t;


    if (Utils::getClosestIntersection(r, objList, its)) {       // Checking closest intersection with camera ray

        w_o = (r.o - its.itsPoint).normalized();    // dir from intersection to origin

        if (its.shape->getMaterial().hasSpecular()) {
            w_r = its.normal * (2 * dot(its.normal, w_o)) - w_o;
            wi = Ray(its.itsPoint, w_r, r.depth + 1);
            L_o = computeColor(wi, objList, lsList);
        }
        else if (its.shape->getMaterial().hasTransmission()) {
            if (dot(r.d, its.normal) < 0) {      // From u1 to u2
                u_t = 1.1;
                neg_n = -its.normal;
                sin2alpha = (1.0 - pow(dot(r.d, its.normal), 2));
                root = 1.0 - (pow(u_t, 2) * sin2alpha);
                if (root < 0)
                {
                    //std::cout << "ray Mirror" << '\n';
                    w_r = its.normal * (2 * dot(its.normal, w_o)) - w_o;
                    wi = Ray(its.itsPoint, w_r, r.depth + 1);
                    L_o = computeColor(wi, objList, lsList);
                }
                else {
                    w_t = (neg_n * (sqrt(root) - (u_t * dot(r.d, neg_n)))) + (r.d * u_t);
                    refract = Ray(its.itsPoint, w_t, r.depth + 1);
                    L_o = computeColor(refract, objList, lsList);
                }
            }
            else {          // From u2 to u1, getting out of the sphere
                u_t = 1.0 / 1.1;
                sin2alpha = (1.0 - pow(dot(r.d, -its.normal), 2));
                root = 1.0 - (pow(u_t, 2) * sin2alpha);
                if (root < 0)
                {
                    std::cout << "w_t to Mirror" << '\n';
                    w_r = its.normal * (2 * dot(its.normal, r.d)) - r.d;
                    wi = Ray(its.itsPoint, w_r, r.depth + 1);
                    L_o = computeColor(wi, objList, lsList);
                }
                else {
                    w_t = (its.normal * (sqrt(root) - u_t * (dot(r.d, its.normal)))) + (r.d * u_t);
                    refract = Ray(its.itsPoint, w_t, r.depth + 1);
                    L_o = computeColor(refract, objList, lsList);
                }
            }


        }

        for (int s = 0; s < lsList.size(); s++) {               // Looping through light sources

            if (its.shape->getMaterial().hasDiffuseOrGlossy()) {
                w_i = (lsList[s].getPosition() - its.itsPoint).normalized();

                L_i = lsList[s].getIntensity(its.itsPoint);

                wi = Ray(its.itsPoint, w_i, 0, Epsilon, (its.itsPoint - lsList[s].getPosition()).length());
                if (Utils::hasIntersection(wi, objList) == false) {
                    L_o += (L_i * its.shape->getMaterial().getReflectance(its.normal, w_o, w_i));
                }
            }


        }

        return L_o;
    }

    return bgColor;
}
