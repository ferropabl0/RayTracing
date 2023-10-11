#include "phong.h"

#include <iostream>

Phong::Phong() :
	d_coeff(0.5), s_coeff(0.5), alpha(0.5)
{ }
Phong::Phong(const double d_coeff_, const double s_coeff_, const double alpha_):
	d_coeff(d_coeff_), s_coeff(s_coeff_), alpha(alpha_)
{ }

Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
	Vector3D wr = n * (2 * dot(n, wi)) - wi;
	Vector3D refl = (d_coeff * dot(wi, n)) + (s_coeff * pow(dot(wo, wr), alpha));

	return refl;
}

bool Phong::hasSpecular() const {
	return true;
}
bool Phong::hasTransmission() const {
	return true;
}
bool Phong::hasDiffuseOrGlossy() const {
	return true;
}

double Phong::getIndexOfRefraction() const {
	return 0.5;
}
