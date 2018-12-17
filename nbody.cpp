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

__attribute__ ((noinline))
void calc_acc_jrk(const int N, Particle * __restrict__ p){
	for(int i=0; i<N; i++){
		vec acc = {0,}, jrk = {0,};
		for(int j=0; j<N; j++){
			if(j == i) continue;
			vec dr = p[j].pos - p[i].pos;
			vec dv = p[j].vel - p[i].vel;

			double r2 = dr * dr;
			double rv = dr * dv;

			double ri1 = 1.0 / sqrt(r2);
			double ri2 = ri1 * ri1;
			double mri3 = p[j].mass * ri2 * ri1;
			double alpha = (-3.0 * rv) * ri2;

			acc += mri3 * (dr);
			jrk += mri3 * (dv + alpha * dr);
		}
		p[i].acc = acc;
		p[i].jrk = jrk;
	}
}

template <typename P>
__attribute__ ((noinline))
void calc_acc(const int N, P * __restrict__ p){
	for(int i=0; i<N; i++){
		decltype(p->acc) acc = {{0,},};
		for(int j=0; j<N; j++){
			if(j == i) continue;
			const auto dr = p[j].pos - p[i].pos;
#if 1
			auto r2 = dr * dr;
#else
			auto r2 = dr.sqr();
#endif
			auto ri1 = r2.rsqrt();
			auto ri2 = ri1 * ri1;
			auto mri3 = p[j].mass * (ri2 * ri1);
			acc += mri3 * (dr);
		}
		p[i].acc = acc;
	}
}

int main(){
	constexpr int N = 5;
	static Particle p[N];
	static AD_Particle ap[N];

	srand48(20181209);

	for(int i=0; i<N; i++){
		p[i].init();
	}

	for(int i=0; i<N; i++){
		ap[i].mass = p[i].mass;
		ap[i].pos = {p[i].pos, p[i].vel};
	}

	calc_acc_jrk(N, p);

	calc_acc(N, ap);

	for(int i=0; i<N; i++){
		const char *fmt = "(%e %e %e), (%e %e %e)\n";
		// fmt = "(%A %A %A), (%A %A %A)\n";
		printf(fmt,
				p[i].acc.x, p[i].acc.y, p[i].acc.z,
				p[i].jrk.x, p[i].jrk.y, p[i].jrk.z);
		printf(fmt,
				ap[i].acc.d0.x, ap[i].acc.d0.y, ap[i].acc.d0.z,
				ap[i].acc.d1.x, ap[i].acc.d1.y, ap[i].acc.d1.z);
		puts("");
	}


	return 0;
}
