#include "ad2.hpp"
#include "ad3.hpp"
#include "ad4.hpp"
#include "vec3.hpp"

// Scalar-AD product, e.g. (double * AD2<Vec3<double>>)
template <typename T>
inline AD2<T> operator*(const typename T::BaseType &s, const AD2<T> &rhs){
	auto dd0 = s * rhs.d0;
	auto dd1 = s * rhs.d1;

	return {dd0, dd1};
}
template <typename T>
inline AD3<T> operator*(const typename T::BaseType &s, const AD3<T> &rhs){
	auto dd0 = s * rhs.d0;
	auto dd1 = s * rhs.d1;
	auto dd2 = s * rhs.d2;

	return {dd0, dd1, dd2};
}
template <typename T>
inline AD4<T> operator*(const typename T::BaseType &s, const AD4<T> &rhs){
	auto dd0 = s * rhs.d0;
	auto dd1 = s * rhs.d1;
	auto dd2 = s * rhs.d2;
	auto dd3 = s * rhs.d3;

	return {dd0, dd1, dd2, dd3};
}

// Scalar-Vector product, e.g. (double * Vec3<AD2<double>>)
// SFINAE for T::BaseType
template <typename T>
inline Vec3<T> operator*(const typename T::BaseType &s, const Vec3<T> &v){
        return {s*v.x, s*v.y, s*v.z};
}

