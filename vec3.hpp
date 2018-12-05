#pragma once 

template <typename T>
struct Vec3{
	typedef T BaseType;

    T x, y, z;

    friend Vec3 operator*(const T &s, const Vec3 &v){
        return {s*v.x, s*v.y, s*v.z};
    }

	// Dot product
    T operator*(const Vec3 &rhs) const {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }

    Vec3 &operator+=(const Vec3 &rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    Vec3 operator-(const Vec3 &rhs) const {
        return {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z
        };
    }

    Vec3 operator+(const Vec3 &rhs) const {
        return {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z
        };
    }

};
