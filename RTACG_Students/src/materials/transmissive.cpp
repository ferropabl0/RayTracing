#include "transmissive.h"

#include <iostream>


Transmissive::Transmissive(const double u_t_, const Vector3D s_coeff_) :
	s_coeff(s_coeff_), u_t(u_t_)
{ }

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
	Vector3D wr = n * (2 * dot(n, wo)) - wo;
	Vector3D refl = s_coeff * dot(wo, wr);
	return refl;
}

bool Transmissive::hasSpecular() const {
	return false;
}
bool Transmissive::hasTransmission() const {
	return true;
}
bool Transmissive::hasDiffuseOrGlossy() const {
	return false;
}

double Transmissive::getIndexOfRefraction() const {
	return 0.5;
}
