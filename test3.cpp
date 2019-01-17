#include <cstdio>
#include <cstdlib>
#include "ad3.hpp"
#include "vec3.hpp"

static double drand53(){
	return sqrt(drand48() * drand48());
}

template<>
void AD3<double>::print() const{
	printf("%e (%A) %e (%A) %e (%A) \n", d0, d0, d1, d1, d2, d2);
}

template<>
AD3<double> AD3<double>::set_rand(){
	d0 = drand53();
	d1 = drand53();
	d2 = drand53();

	return *this;
}

int main(){
	srand48(20181130);

	using AD = AD3<double>;
	const AD x = AD().set_rand();
	const AD y = AD().set_rand();

	x.print();
	(x/y).print();
	(y*(x/y)).print();
	puts("");
	(x/y).print();
	(x*y.inv()).print();
	puts("");
	x.inv().print();
	(x.rsqrt() * x.rsqrt()).print();
	puts("");
	auto t = x.rsqrt();
	auto t3 = t * t * t;
	t3.print();
	x.rsqrtCubed().print();

	puts("");

	puts("Vec3 tests");

	Vec3<AD> u, v;
	u = {AD().set_rand(), AD().set_rand(), AD().set_rand()};
	v = {AD().set_rand(), AD().set_rand(), AD().set_rand()};

	(u.x*v.x + u.y*v.y + u.z*v.z).print();

	(u * v).print();

	AD3<Vec3<double>> uu = {
		{u.x.d0, u.y.d0, u.z.d0}, 
		{u.x.d1, u.y.d1, u.z.d1}, 
		{u.x.d2, u.y.d2, u.z.d2},
	}; 
	AD3<Vec3<double>> vv = {
		{v.x.d0, v.y.d0, v.z.d0}, 
		{v.x.d1, v.y.d1, v.z.d1},
		{v.x.d2, v.y.d2, v.z.d2},
	}; 

	(uu * vv).print();

	puts("");
	((0.25 * u) * (4.0 * v)).print();
	((0.25 *uu) * (4.0 *vv)).print();


	return 0;
}
