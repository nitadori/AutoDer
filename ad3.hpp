#pragma once

#include <cmath>

template <typename T>
struct AD3{
	typedef T BaseType;

	T d0;
	T d1;
	T d2;

#if 0 // can cause a terrible bug
	operator T() const {
		return d0;
	}
#endif

	//  Basic four operators
	AD3 operator+(const AD3 &rhs) const {
		T dd0 = d0 + rhs.d0;
		T dd1 = d1 + rhs.d1;
		T dd2 = d2 + rhs.d2;

		return {dd0, dd1, dd2};
	}
	AD3 operator-(const AD3 &rhs) const {
		T dd0 = d0 - rhs.d0;
		T dd1 = d1 - rhs.d1;
		T dd2 = d2 - rhs.d2;

		return {dd0, dd1, dd2};
	}

	using DotpType = decltype(d0 * d1);
	AD3<DotpType> operator*(const AD3 &rhs) const {
		DotpType dd0 = d0 * rhs.d0;
		DotpType dd1 = d0 * rhs.d1 + rhs.d0 * d1;
		DotpType dd2 = d0 * rhs.d2 + 2 * (d1 * rhs.d1) + d2 * rhs.d0;

		return {dd0, dd1, dd2};

	}
	AD3 operator/(const AD3 &rhs) const {
		const AD3 &x = *this;
		const AD3 &y = rhs;

		T yinv  = T(1) / y.d0;
		T yinv2 = yinv  * yinv;
		T yinv3 = yinv2 * yinv;

		T tt1 = x.d1*y.d0 - x.d0*y.d1;
		T tt2 = x.d2*y.d0 - x.d0*y.d2;
		
		T dd0 = x.d0 * yinv;
		T dd1 = tt1 * yinv2;
		// T dd2 = tt2 * yinv2 - 2*y.d1*tt1 * yinv3;
		T dd2 = (y.d0 * tt2  - 2*y.d1 * tt1) * yinv3;

		return {dd0, dd1, dd2};
	}

	// Scalar-AD product
	friend AD3 operator*(const T &lhs, const AD3 &rhs){
		T dd0 = lhs * rhs.d0;
		T dd1 = lhs * rhs.d1;
		T dd2 = lhs * rhs.d2;

		return {dd0, dd1, dd2};
	}
	AD3 operator*(const T &rhs) const {
		T dd0 = d0 * rhs;
		T dd1 = d1 * rhs;
		T dd2 = d2 * rhs;

		return {dd0, dd1, dd2};
	}
	template<template<typename> class VEC>
	friend AD3<VEC<T>> operator*(const AD3<T> &s, const AD3<VEC<T>> &v){
		auto dd0 = s.d0 * v.d0;
		auto dd1 = s.d0 * v.d1 + s.d1 * v.d0;
		auto dd2 = s.d0 * v.d2 + T(2)*(s.d1 * v.d1) + s.d2 * v.d0;

		return {dd0, dd1, dd2};
	}

	// Overwriting operators
	const AD3 &operator+=(const AD3 &rhs) {
		AD3 tmp = *this + rhs;
		*this = tmp;
		return *this;
	}
	const AD3 &operator-=(const AD3 &rhs) {
		AD3 tmp = *this - rhs;
		*this = tmp;
		return *this;
	}
	const AD3 &operator*=(const AD3 &rhs) {
		AD3 tmp = *this * rhs;
		*this = tmp;
		return *this;
	}
	const AD3 &operator/=(const AD3 &rhs) {
		AD3 tmp = *this / rhs;
		*this = tmp;
		return *this;
	}

	// Primitive functions
	AD3<DotpType> sqr() const {
		DotpType dd0 = d0 * d0;
		DotpType dd1 = 2 * (d0 * d1);
		DotpType dd2 = 2 * (d0 * d2 + d1 * d1);

		return {dd0, dd1};
	}
	AD3 inv() const {
		T xinv = T(1) / d0;
		T y0 = xinv;
		T y1 = -(d1 * y0) * xinv;
		T y2 = (-d2*y0 - T(2)*d1*y1) * xinv;

		return {y0, y1, y2};
	}
	AD3 rsqrt() const {
#ifdef FAST_RSQRT
		T y0   = rsqrt(d0);
		T xinv = t1 * t1;
#else
		T xinv = T(1) / d0;
		T y0   = std::sqrt(xinv);
#endif
		T y1 = (T(-1)/2) * xinv * d1*y0;
		T y2 = (T(-1)/2) * xinv * (d2 * y0 + T(3)*d1*y1);

		return {y0, y1, y2};
	}

	// Please specialize
	void print() const;
	AD3 set_rand();

};
