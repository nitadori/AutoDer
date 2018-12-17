#pragma once

#include <cmath>

template <typename T>
struct AD2{
	typedef T BaseType;

	T d0;
	T d1;
	
#if 0
	operator T() const {
		return d0;
	}
#endif

	//  Basic four operators
	AD2 operator+(const AD2 &rhs) const {
		T dd0 = d0 + rhs.d0;
		T dd1 = d1 + rhs.d1;

		return {dd0, dd1};
	}
	AD2 operator-(const AD2 &rhs) const {
		T dd0 = d0 - rhs.d0;
		T dd1 = d1 - rhs.d1;

		return {dd0, dd1};
	}
#if 0
	AD2 operator*(const AD2 &rhs) const {
		T dd0 = d0 * rhs.d0;
		T dd1 = d0 * rhs.d1 + rhs.d0 * d1;

		return {dd0, dd1};
	}
#else
	// typedef decltype(d0 * d1) DotpType;
	using DotpType = decltype(d0 * d1);
	AD2<DotpType> operator*(const AD2 &rhs) const {
		DotpType dd0 = d0 * rhs.d0;
		DotpType dd1 = d0 * rhs.d1 + rhs.d0 * d1;

		return {dd0, dd1};
	}
#endif
	AD2 operator/(const AD2 &rhs) const {
		T ginv  = T(1) / rhs.d0;
		T ginv2 = ginv * ginv;

		T dd0 = d0 * ginv;
		T dd1 = (rhs.d0 * d1 - d0 * rhs.d1) * ginv2;

		return {dd0, dd1};
	}

	// Scalar-AD product
	friend AD2 operator*(const T &lhs, const AD2 &rhs){
		T dd0 = lhs * rhs.d0;
		T dd1 = lhs * rhs.d1;

		return {dd0, dd1};
	}
	AD2 operator*(const T &rhs) const {
		T dd0 = d0 * rhs;
		T dd1 = d1 * rhs;

		return {dd0, dd1};
	}

	template<template<typename> class VEC>
	friend AD2<VEC<T>> operator*(const AD2<T> &s, const AD2<VEC<T>> &v){
		auto dd0 = s.d0 * v.d0;
		auto dd1 = s.d0 * v.d1 + s.d1 * v.d0;

		return {dd0, dd1};
	}
	

	// Overwriting operators
	const AD2 &operator+=(const AD2 &rhs) {
		AD2 tmp = *this + rhs;
		*this = tmp;
		return *this;
	}
	const AD2 &operator-=(const AD2 &rhs) {
		AD2 tmp = *this - rhs;
		*this = tmp;
		return *this;
	}
	const AD2 &operator*=(const AD2 &rhs) {
		AD2 tmp = *this * rhs;
		*this = tmp;
		return *this;
	}
	const AD2 &operator/=(const AD2 &rhs) {
		AD2 tmp = *this / rhs;
		*this = tmp;
		return *this;
	}

	// Primitive functions
	AD2<DotpType> sqr() const {
		DotpType dd0 = d0 * d0;
		DotpType dd1 = 2 * (d0 * d1);

		return {dd0, dd1};
	}
	AD2 inv() const {
		T finv  = T(1) / d0;
		T finv2 = finv * finv;

		T dd0 = finv;
		T dd1 = -d1 * finv2;

		return {dd0, dd1};
	}
	AD2 rsqrt() const {
#ifdef FAST_RSQRT
		T t1 = rsqrt(d0);
		T t2 = t1 * t1;
#else
		T t2 = T(1) / d0;
		T t1 = std::sqrt(t2);
#endif
		T t3 = t1 * t2;

		T dd0 = t1;
		T dd1 = (T(-1)/T(2)) * (d1 * t3);

		return {dd0, dd1};
	}
	AD2 rsqrtCubed() const {
#ifdef FAST_RSQRT
		T t1 = rsqrt(d0);
		T t2 = t1 * t1;
#else
		T t2 = T(1) / d0;
		T t1 = std::sqrt(t2);
#endif
		T t3 = t1 * t2;
		T t5 = t2 * t3;

		T dd0 = t3;
		T dd1 = (T(-3)/T(2)) * (d1 * t5);

		return {dd0, dd1};
	}

	// Please specialize
	void print() const;
	AD2 set_rand();
};
