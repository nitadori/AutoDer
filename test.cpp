#include <cstdio>
#include <cstdlib>
#include "ad2.hpp"

template<>
void AD2<double>::print() const{
	printf("%A %A\n", d0, d1);
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

	return 0;
}
