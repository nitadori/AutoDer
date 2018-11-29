#include <cstdio>
#include <cstdlib>
#include "ad2.hpp"

int main(){
	srand48(20181130);

	using Ad2 = AD2<double>;
	Ad2 x = {drand48()*drand48(), drand48()*drand48()};
	Ad2 y0 = x.rsqrtCubed();
	Ad2 y1 = x.inv() * x.rsqrt();
	Ad2 tmp = x.rsqrt();
	Ad2 y2 = tmp * tmp * tmp;

	y0.print();
	y1.print();
	y2.print();

	return 0;
}
