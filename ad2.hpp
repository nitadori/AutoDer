#include <cmath>

template <typename T>
struct AD2{
	T d0;
	T d1;

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
		T ginv = T(1) / rhs.d0;
		T ginv2 = ginv * ginv;

		T dd0 = d0 * ginv;
		T dd1 = (rhs.d0 * d1 - d0 * rhs.d1) * ginv2;

		return {dd0, dd1};
	}
};
