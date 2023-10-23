#include "globalshader.h"
#include "../core/utils.h"
#include "../core/hemisphericalsampler.h"
#include <math.h>

GlobalShader::GlobalShader() :
    hitColor(Vector3D(1, 0, 0))
{ }

GlobalShader::GlobalShader(Vector3D hitColor_, Vector3D bgColor_) :
    Shader(bgColor_), hitColor(hitColor_)
{ }

Vector3D GlobalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<PointLightSource>& lsList) const
{
    Intersection its;
    Vector3D L_o = Vector3D(0.0), L_ind = Vector3D(0.0), a_t = (0.1, 0.1, 0.1), L_i, w_i, w_o, w_r, w_t, neg_n, k_d, w_j, L_i_n, L_i_r;
    Ray wi, refract, sec, refl;
    double root, sin2alpha, u_t;
    int max_depth = 3, n;


    if (Utils::getClosestIntersection(r, objList, its)) {       // Checking closest intersection with camera ray

        w_o = (r.o - its.itsPoint).normalized();    // dir from intersection to camera pos

        if (its.shape->getMaterial().hasSpecular()) {           // MIRROR
            w_r = its.normal * (2 * dot(its.normal, w_o)) - w_o;    // refl direction
            wi = Ray(its.itsPoint, w_r, r.depth + 1);               // refl ray
            L_o = computeColor(wi, objList, lsList);                // color of the intersection point of the refl ray
        }
        else if (its.shape->getMaterial().hasTransmission()) {   // TRANSMISSIVE
            if (dot(r.d, its.normal) < 0) {      // From u1 to u2
                u_t = 1.1;
                neg_n = -its.normal;
                sin2alpha = (1.0 - pow(dot(r.d, its.normal), 2));
                root = 1.0 - (pow(u_t, 2) * sin2alpha);
                if (root < 0)
                {
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

        if (its.shape->getMaterial().hasDiffuseOrGlossy()) { // PHONG
            for (int s = 0; s < lsList.size(); s++) {               // Looping through light sources
                w_i = (lsList[s].getPosition() - its.itsPoint).normalized();    // dir from intersection point to light source

                L_i = lsList[s].getIntensity(its.itsPoint);     // intensity of the intersection point for each source

                wi = Ray(its.itsPoint, w_i, 0, Epsilon, (its.itsPoint - lsList[s].getPosition()).length());     // ray limited from intersection point to light source
                if (Utils::hasIntersection(wi, objList) == false) {
                    L_o += (L_i * its.shape->getMaterial().getReflectance(its.normal, w_o, w_i));
                }
            }

            if (r.depth == 0) {     // First bounce
                HemisphericalSampler sampler = HemisphericalSampler();
                n = 30;             // Number of random samples from 
                for (int i = 0; i < n; i++) {
                    w_j = sampler.getSample(its.normal);    // random direction of the normal hemisphere
                    sec = Ray(its.itsPoint, w_j, r.depth + 1);  // secondary ray
                    L_i = computeColor(sec, objList, lsList);   // calling computeColor again, with an incremented depth
                    L_ind += (L_i * its.shape->getMaterial().getReflectance(its.normal, w_j, w_o)); // Sum of indirect light
                }
                L_ind /= (2 * M_PI * n);
                L_o += L_ind;
            }
            else if (r.depth == max_depth) {    // Last bounce
                k_d = its.shape->getMaterial().getDiffuseCoefficient();
                L_ind = a_t * k_d;      // Indirect component
                L_o += L_ind;
                 
            } else if (r.depth < max_depth) {   // Other bounces
                w_r = its.normal * (2 * dot(its.normal, w_o)) - w_o;    // Perfect reflected direction
                refl = Ray(its.itsPoint, w_r, r.depth + 1);
                L_i_r = computeColor(refl, objList, lsList);            // Li(reflected, w_o)
                sec = Ray(its.itsPoint,its.normal, r.depth + 1);
                L_i_n = computeColor(sec, objList, lsList);             // Li(normal, w_o)
                L_ind = (L_i_n * its.shape->getMaterial().getReflectance(its.normal, its.normal, w_o)) + (L_i_r * its.shape->getMaterial().getReflectance(its.normal, w_r, w_o));   // Formula
                L_ind /= (4 * M_PI);
                L_o += L_ind;
            }
        }
        return L_o;
    }

    return bgColor;
}
