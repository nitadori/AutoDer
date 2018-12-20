#include <cstdio>
#include <cstdlib>
#include "ad4.hpp"

static double drand53(){
	return sqrt(drand48() * drand48());
}

template<>
void AD4<double>::print() const{
	printf("%e (%A) %e (%A) %e (%A) %e (%A)\n", d0, d0, d1, d1, d2, d2, d3, d3);
}

template<>
AD4<double> AD4<double>::set_rand(){
	d0 = drand53();
	d1 = drand53();
	d2 = drand53();
	d3 = drand53();

	return *this;
}

int main(){
	srand48(20181219);

	using AD = AD4<double>;
	const AD x = AD().set_rand();
	const AD y = AD().set_rand();

	x.print();
	y.print();
	(x/y).print();

	puts("");

	x.print();
	(y*(x/y)).print();

	puts("");
	(x*x).print();
	x.sqr().print();
	(x/y).print();
	(x*y.inv()).print();

	puts("");

	x.inv().print();
	(x.rsqrt() * x.rsqrt()).print();
	
	return 0;
};
