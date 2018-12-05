template <typename T>
struct Vec3{
    T x, y, z;

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

// Scalar-Vector product
template <typename T>
inline Vec3<T> operator*(const typename T::BaseType &s, const Vec3<T> &v){
        return {s*v.x, s*v.y, s*v.z};
}
// Substitution Failure is not An Error
template <typename T>
inline Vec3<T> operator*(const T &s, const Vec3<T> &v){
        return {s*v.x, s*v.y, s*v.z};
}
