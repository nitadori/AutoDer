#include <cstdio>
#include <cstdlib>
#include "ad2.hpp"
#include "vec3.hpp"
#include "sfinae.hpp"

template<>
void AD2<double>::print() const{
	printf("%e (%A) %e (%A)\n", d0, d0, d1, d1);
}

template<>
AD2<double> AD2<double>::set_rand(){
	d0 = sqrt(drand48() * drand48());
	d1 = sqrt(drand48() * drand48());

	return *this;
}

int main(){
	srand48(20181130);

	using AD = AD2<double>;
	const AD x = AD().set_rand();
	AD y0 = x.rsqrtCubed();
	AD y1 = x.inv() * x.rsqrt();
	AD tmp = x.rsqrt();
	AD y2 = tmp * tmp * tmp;

	y0.print();
	y1.print();
	y2.print();
	
	puts("");

	const AD z = AD().set_rand();
	(x/z).print();
	(x * z.inv()).print();

	puts("");
	x.print();
	(0.25 * x * 4.0).print();

	puts("");

	puts("Vec3 tests");

	Vec3<AD> u, v;
	u = {AD().set_rand(), AD().set_rand(), AD().set_rand()};
	v = {AD().set_rand(), AD().set_rand(), AD().set_rand()};

	(u.x*v.x + u.y*v.y + u.z*v.z).print();

	(u * v).print();

	AD2<Vec3<double>> uu = {
		{u.x.d0, u.y.d0, u.z.d0}, 
		{u.x.d1, u.y.d1, u.z.d1}}; 
	AD2<Vec3<double>> vv = {
		{v.x.d0, v.y.d0, v.z.d0}, 
		{v.x.d1, v.y.d1, v.z.d1}}; 

	(uu * vv).print();

	puts("");
	((0.25 * u) * (4.0 * v)).print();
	((0.25 *uu) * (4.0 *vv)).print();


	return 0;
}
