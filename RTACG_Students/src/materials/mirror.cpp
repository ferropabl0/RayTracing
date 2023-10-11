#include "mirror.h"

#include <iostream>


Mirror::Mirror(const Vector3D s_coeff_) :
	s_coeff(s_coeff_)
{ }

Vector3D Mirror::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
	Vector3D wr = n * (2 * dot(n, wo)) - wo;
	Vector3D refl = s_coeff * dot(wo, wr);
	return refl;
}

bool Mirror::hasSpecular() const {
	return true;
}
bool Mirror::hasTransmission() const {
	return false;
}
bool Mirror::hasDiffuseOrGlossy() const {
	return false;
}

double Mirror::getIndexOfRefraction() const {
	return 0.5;
}
