#include <cstdio>
#include <cmath>

template <typename T>
struct AD2{
	T d0;
	T d1;
	
	operator T() const {
		return d0;
	}

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
	AD2 operator*(const AD2 &rhs) const {
		T dd0 = d0 * rhs.d0;
		T dd1 = d0 * rhs.d1 + rhs.d0 * d1;

		return {dd0, dd1};
	}
	AD2 operator/(const AD2 &rhs) const {
		T ginv  = T(1) / rhs.d0;
		T ginv2 = ginv * ginv;

		T dd0 = d0 * ginv;
		T dd1 = (rhs.d0 * d1 - d0 * rhs.d1) * ginv2;

		return {dd0, dd1};
	}

	// Overwriting operators
	const AD2 &operator+=(const AD2 &rhs) {
		AD2 tmp = *this + rhs;
		*this = tmp;
	}
	const AD2 &operator-=(const AD2 &rhs) {
		AD2 tmp = *this - rhs;
		*this = tmp;
	}
	const AD2 &operator*=(const AD2 &rhs) {
		AD2 tmp = *this * rhs;
		*this = tmp;
	}
	const AD2 &operator/=(const AD2 &rhs) {
		AD2 tmp = *this / rhs;
		*this = tmp;
	}

	// Primitive functions
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

	int print(
			FILE *fp = stdout,
			const char *fmt = "%A, %A\n"
			) const {
		int ret = fprintf(fp, fmt, d0, d1);
		return ret;
	}
};