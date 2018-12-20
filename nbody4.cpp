#include <cstdio>
#include <cstdlib>
#include "ad4.hpp"
#include "vec3.hpp"
#include "sfinae.hpp"

static double drand53(){
	return sqrt(drand48() * drand48());
}

template<>
void AD4<double>::print() const{
	printf("%A %A %A %A\n", d0, d1, d2, d3);
}

template<>
AD4<double> AD4<double>::set_rand(){
	d0 = drand53();
	d1 = drand53();
	d2 = drand53();
	d3 = drand53();

	return *this;
}

using vec = Vec3<double>;

struct Particle{
	double mass;
	vec pos;
	vec vel;
	vec acc;
	vec jrk;
	vec snp;
	vec crk;

	void init(){
		mass = drand53();
		pos  = {drand53(), drand53(), drand53()};
		vel  = {drand53(), drand53(), drand53()};
		acc  = {0,};
		jrk  = {0,};
		snp  = {0,};
		crk  = {0,};
	}
};

struct AD_Particle{
	double mass;
	AD4<Vec3<double>> pos;
	AD4<Vec3<double>> acc;
};

template <typename P>
__attribute__ ((noinline))
void calc_acc(const int N, P * __restrict__ p){
	for(int i=0; i<N; i++){
		decltype(p->acc) acc = {{0,},};
		for(int j=0; j<N; j++){
			if(j == i) continue;
			auto dr = p[j].pos - p[i].pos;
#if 0
			auto r2 = dr * dr;
#else
			auto r2 = dr.sqr();
#endif
#if 0
			auto ri1 = r2.rsqrt();
			auto ri2 = ri1 * ri1;
			auto mri3 = p[j].mass * (ri2 * ri1);
#else
			auto mri3 = p[j].mass * r2.rsqrtCubed();
#endif
			acc += mri3 * dr;
		}
		p[i].acc = acc;
	}
}

__attribute__ ((noinline))
void calc_acc_jrk_snp_crk(const int N, Particle * __restrict__ p){
	for(int i=0; i<N; i++){
		vec acc = {0,}, jrk = {0,}, snp = {0,}, crk = {0,};
		for(int j=0; j<N; j++){
			if(j == i) continue;
			vec dr = p[j].pos - p[i].pos;
			vec dv = p[j].vel - p[i].vel;
			vec da = p[j].acc - p[i].acc;
			vec dj = p[j].jrk - p[i].jrk;

			double r2 = dr * dr;
			double rv = dr * dv;
			double v2 = dv * dv;
			double ra = dr * da;
			double va = dv * da;
			double rj = dr * dj;

			double ri1 = 1.0 / sqrt(r2);
			double ri2 = ri1 * ri1;
			double mri3 = p[j].mass * ri2 * ri1;
			double alpha = rv * ri2;
			double beta  = (v2 + ra) * ri2 + alpha * alpha;
			double gamma = (3.0*va + rj)*ri2 + alpha*(3.0*beta - 4.0*alpha*alpha);


#if 0
			const vec aij = mri3 * dr;
			const vec jij = mri3 * dv + (-3.0*alpha) * aij;
			const vec sij = mri3 * da + (-6.0*alpha) * jij + (-3.0*beta) * aij;
			const vec cij = mri3 * dj + (-9.0*alpha) * sij + (-9.0*beta) * jij + (-3.0*gamma) * aij;


			acc += aij;
			jrk += jij;
			snp += sij;
			crk += cij;
#else
			acc += mri3 * dr;
			vec tmp1 = dv + (-3.0*alpha) * dr;
			jrk += mri3 * tmp1;
			vec tmp2 = da + (-6.0*alpha) * tmp1 + (-3.0*beta) * dr;
			snp += mri3 * tmp2;
			vec tmp3 = dj + (-9.0*alpha) * tmp2 + (-9.0*beta) * tmp1 + (-3.0*gamma) * dr;
			crk += mri3 * tmp3;
#endif
		}

		p[i].acc = acc;
		p[i].jrk = jrk;
		p[i].snp = snp;
		p[i].crk = crk;
	}
}

int main(){
	srand48(20181221);

	constexpr int N = 5;
	static Particle p[N];
	static AD_Particle ap[N];

	for(int i=0; i<N; i++){
		p[i].init();
	}

	calc_acc_jrk_snp_crk(N, p);
	calc_acc_jrk_snp_crk(N, p);

	for(int i=0; i<N; i++){
		ap[i].mass = p[i].mass;
		ap[i].pos = {p[i].pos, p[i].vel, p[i].acc, p[i].jrk};
	}

	calc_acc(N, ap);

	for(int i=0; i<N; i++){
		const char *fmt = "(%e %e %e), (%e %e %e), (%e %e %e), (%e %e %e)\n";
		// fmt = "(%A %A %A), (%A %A %A)\n";
		printf(fmt,
				p[i].acc.x, p[i].acc.y, p[i].acc.z,
				p[i].jrk.x, p[i].jrk.y, p[i].jrk.z,
				p[i].snp.x, p[i].snp.y, p[i].snp.z,
				p[i].crk.x, p[i].crk.y, p[i].crk.z);
		printf(fmt,
				ap[i].acc.d0.x, ap[i].acc.d0.y, ap[i].acc.d0.z,
				ap[i].acc.d1.x, ap[i].acc.d1.y, ap[i].acc.d1.z,
				ap[i].acc.d2.x, ap[i].acc.d2.y, ap[i].acc.d2.z,
				ap[i].acc.d3.x, ap[i].acc.d3.y, ap[i].acc.d3.z);
		puts("");
	}

	return 0;
}
