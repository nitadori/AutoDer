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
		T dd2 = tt2 * yinv2 - 2*y.d1*tt1 * yinv3;

		return {dd0, dd1, dd2};
	}

	// Please specialize
	void print() const;
	AD3 set_rand();

};
