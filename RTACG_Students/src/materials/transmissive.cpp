#include "transmissive.h"

#include <iostream>


Transmissive::Transmissive() :
	u_t(1.1)
{ }

Vector3D Transmissive::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {
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

double Phong::getIndexOfRefraction() const {
	return 0.5;
}
