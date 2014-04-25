#include <gmp.h>
#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


int world_size;
int world_rank;
char proc_name[MPI_MAX_PROCESSOR_NAME];
int proc_name_len;
mpz_t randfactor;


void  f(mpz_t out,mpz_t i, mpz_t n){
//	return (i*i+1)%n;
	mpz_mul(out,i,i);
	mpz_add(out,out,randfactor);
	mpz_mod(out,out,n); //maybe problem?
}

void prho(mpz_t n){
	mpz_t tmp;
	mpz_t a;
	mpz_t b;
	mpz_t p;
	mpz_init(a);
	mpz_init(b);
	mpz_init(p);
	mpz_init(tmp);
	mpz_set_str(p,"1",10);
	gmp_randstate_t r_state;
	gmp_randinit_default(r_state);
	gmp_randseed_ui(r_state,time(NULL)*world_rank);
//	srand(time(NULL)*world_rank);
//	#pragma omp parallel
	for(;;){
		mpz_urandomb(randfactor,r_state,mpz_sizeinbase(n,10));
		mpz_urandomb(randfactor,r_state,mpz_sizeinbase(n,10));
		mpz_mod(a,a,n);;
		mpz_set(b,a);

		do{
			f(a,a,n);
			f(b,b,n);
			f(b,b,n);
			mpz_sub(tmp,b,a);
			mpz_abs(tmp,tmp); //may cause problems
			mpz_gcd(p,tmp,n);
			if(mpz_cmp_ui(p,1) > 0) break;
		}while(mpz_cmp(b,a)!=0);

		if(mpz_cmp(p,n) != 0 && mpz_cmp_ui(p,1) > 0){
			//break;
			printf("World Rank:\t%d\tProcessor:\t%s\n",world_rank,proc_name);
			gmp_printf("%Zd\n",p);
			mpz_t out;
			mpz_init(out);
			mpz_div(out,n,p);
			gmp_printf("%Zd\n",out);
		}
	}
//	return p;

}


int main(int argc, char **argv){

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&world_size);
	MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
	MPI_Get_processor_name(proc_name,&proc_name_len);

	mpz_t n;
	mpz_init(n);
	mpz_set_str(n,"5691972411341955564803",10);
	mpz_set_str(n,"430960000765005008079109423163567",10);
	//5691972411341955564803 
	prho(n);
}
