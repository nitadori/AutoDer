#include <cstdio>
#include <cstdlib>
#include "ad2.hpp"
#include "vec3.hpp"
#include "sfinae.hpp"

static double drand53(){
	return sqrt(drand48() * drand48());
}

template<>
void AD2<double>::print() const{
	printf("%A %A\n", d0, d1);
}

template<>
AD2<double> AD2<double>::set_rand(){
	d0 = sqrt(drand53());
	d1 = sqrt(drand53());

	return *this;
}

using vec = Vec3<double>;

struct Particle{
	double mass;
	vec pos;
	vec vel;
	vec acc;
	vec jrk;

	void init(){
		mass = drand53();
		pos  = {drand53(), drand53(), drand53()};
		vel  = {drand53(), drand53(), drand53()};
		acc  = {0,};
		jrk  = {0,};
	}
};

struct AD_Particle{
	double mass;
	AD2<Vec3<double>> pos;
	AD2<Vec3<double>> acc;
};

int main(){
	constexpr int N = 5;
	static Particle p[N];
	static AD_Particle ap[N];

	for(int i=0; i<N; i++){
		p[i].init();
	}

	for(int i=0; i<N; i++){
		ap[i].mass = p[i].mass;
		ap[i].pos = {p[i].pos, p[i].vel};
	}

	srand48(20181209);

	return 0;
}
