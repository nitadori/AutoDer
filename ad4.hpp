#pragma once

#include <cmath>

template <typename T>
struct AD4{
	typedef T BaseType;

	T d0;
	T d1;
	T d2;
	T d3;

	//  Basic four operators
	AD4 operator+(const AD4 &rhs) const {
		T dd0 = d0 + rhs.d0;
		T dd1 = d1 + rhs.d1;
		T dd2 = d2 + rhs.d2;
		T dd3 = d3 + rhs.d3;

		return {dd0, dd1, dd2, dd3};
	}
	AD4 operator-(const AD4 &rhs) const {
		T dd0 = d0 - rhs.d0;
		T dd1 = d1 - rhs.d1;
		T dd2 = d2 - rhs.d2;
		T dd3 = d3 - rhs.d3;

		return {dd0, dd1, dd2, dd3};
	}

	// can be  either of
	//   scalar * scalar -> scalar
	//   scalar * vector -> vector
	//   vector * vector -> scalar
	// where
	//   scalar = AD4<double>
	//   vector = AD4<Vec3<double>>
	template <typename RHS>
	auto operator*(const RHS &rhs) const -> AD4<decltype(d0 * rhs.d0)> {
		auto dd0 = d0 * rhs.d0;
		auto dd1 = d0 * rhs.d1 + d1 * rhs.d0;
		auto dd2 = d0 * rhs.d2 + 2 * (d1 * rhs.d1) + d2 * rhs.d0;
		auto dd3 = d0 * rhs.d3 + d3 * rhs.d0 + 
			3 * (d1 * rhs.d2 + d2 * rhs.d1);

		return {dd0, dd1, dd2, dd3};
	}

	AD4 operator/(const AD4 &rhs) const {
		const AD4 &x = *this;
		const AD4 &y = rhs;

#if 0
		T yinv  = T(1) / y.d0;
		T yinv2 = yinv  * yinv;
		T yinv3 = yinv2 * yinv;
		T yinv4 = yinv2 * yinv2;

		T tt1 = x.d1*y.d0 - x.d0*y.d1;
		T tt2 = x.d2*y.d0 - x.d0*y.d2;
		T tt3 = x.d3*y.d0 - x.d0*y.d3;
		
		T dd0 = x.d0 * yinv;
		T dd1 = tt1 * yinv2;
		// T dd2 = tt2 * yinv2 - 2*y.d1*tt1 * yinv3;
		T dd2 = (y.d0 * tt2  - 2*y.d1 * tt1) * yinv3;

		T dd3 = tt3 * y.d0*y.d0 - 3 * (
		        tt2 * y.d0*y.d1 +
				tt1 * (y.d0*y.d2 - 2*y.d1*y.d1));
		dd3 *= yinv4;

		return {dd0, dd1, dd2, dd3};
#else
		AD4 z;
		T yinv  = T(1) / y.d0;
		z.d0 = x.d0 * yinv;
		z.d1 = (x.d1 - y.d1 * z.d0) * yinv;
		z.d2 = (x.d2 - T(2) * y.d1 * z.d1 - y.d2 * z.d0) * yinv;
		z.d3 = (x.d3 - T(3) * (y.d1 * z.d2 + y.d2 * z.d1) - y.d3 * z.d0) * yinv;

		return z;
#endif

	}

	// Please specialize
	void print() const;
	AD4 set_rand();
};
