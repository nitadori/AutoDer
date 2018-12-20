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

	// can be  either of
	//   scalar * scalar -> scalar
	//   scalar * vector -> vector
	//   vector * vector -> scalar
	// where
	//   scalar = AD3<double>
	//   vector = AD3<Vec3<double>>
	template <typename RHS>
	auto operator*(const RHS &rhs) const -> AD3<decltype(d0 * rhs.d0)> {
		auto dd0 = d0 * rhs.d0;
		auto dd1 = d0 * rhs.d1 + d1 * rhs.d0;
		auto dd2 = d0 * rhs.d2 + 2 * (d1 * rhs.d1) + d2 * rhs.d0;

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
	auto sqr() const -> AD3<decltype(d0 * d0)> {
		auto dd0 = d0 * d0;
		auto dd1 = 2 * (d0 * d1);
		auto dd2 = 2 * (d0 * d2 + d1 * d1);

		return {dd0, dd1, dd2};
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
#if 0
	AD3 rsqrtCubed() const {
#ifdef FAST_RSQRT
		T t1 = rsqrt(d0);
		T t2 = t1 * t1;
#else
		T t2 = T(1) / d0;
		T t1 = std::sqrt(t2);
#endif
		T y0   = t1 * t2;
		T cc   = (T(-3)/T(2)) * y0;
		T xinv = t2;
		T y1   = cc * d1 * xinv;
		T y2   = (cc * d2 - (T(5)/T(2)) * d1 * y1) * xinv;


		return {y0, y1, y2};
	}
#else
	AD3 rsqrtCubed() const {
#ifdef FAST_RSQRT
		T y0   = rsqrt(d0);
		T xinv = t1 * t1;
#else
		T xinv = T(1) / d0;
		T y0   = std::sqrt(xinv);
#endif
		y0 *= xinv;

		T cc = T(-3)/T(2) * y0;
		T y1 = cc *  d1* xinv; 
		T y2 = (cc * d2 + (T(-5)/T(2))*d1*y1) * xinv;

		return {y0, y1, y2};
	}
#endif

	// Please specialize
	void print() const;
	AD3 set_rand();

};
