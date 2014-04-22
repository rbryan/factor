#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u128 __attribute__((mode(TI)));
typedef unsigned int u64 __attribute__((mode(DI)));



int world_size;
int world_rank;
char proc_name[MPI_MAX_PROCESSOR_NAME];
int proc_name_len;
u128 randfactor;

inline u128 my_abs(__int128_t a){
	if(a<0) return (u128)-1*a;
	return (u128) a;
}

inline u128 f(u128 i, u128 n){
//	return (i*i+1)%n;
	return (i*i+randfactor)%n;
}

u128 gcd(u128 a, u128 b){
	if(b==0) return a;
	else
		return gcd(b,a%b);
}

void print128( u128 p){
	
	printf("%llu\n",(u64)(p>>64));
	printf("%llu",(u64)(p));
}

void prho(u128 n){
	u128 a;
	u128 b;
	u128 p=1;
	srand(time(NULL)*world_rank);
//	#pragma omp parallel
	for(;;){
		randfactor = ((u128)rand())<<96 | ((u128)rand())<<64 | ((u128)rand())<<32 | ((u128)rand());
		a = rand()%n;
		b = a;
		u64 count=0;

		do{
			a = f(a,n);
			b = f(f(b,n),n);
			p = gcd(my_abs((__int128_t)b-(__int128_t)a),n);	
			count ++;
			if(p>1) break;
		}while(b != a);

		if(p!=n && p > 1){
			//break;
			printf("World Rank:\t%d\tProcessor:\t%s\n",world_rank,proc_name);
			print128(p);
			print128(p/n);
		}
	}
//	return p;

}


int main(int argc, char **argv){

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	MPI_Get_processor_name(proc_name,&proc_name_len);

	u128 a =  56919724113ULL;
		a*=100000000000ULL;
		a+= 41955564803ULL;//5691972411341955564803 
		a=73874852881ULL*77048849363ULL;
		a=45433413203ULL*191244791ULL;
	print128(a);
	prho(a);
}
