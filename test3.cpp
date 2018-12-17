#include <cstdio>
#include <cstdlib>
#include "ad3.hpp"

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

	return 0;
}
