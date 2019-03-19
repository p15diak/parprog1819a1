
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>
	/* To programma ginetai compile me thn entolh : gcc -Wall -O2 matmul-sse.c -o matmul-sse -DN= 400
	* to N borei na parei opoia timh tou dwsoume kalo 8a einai na diaireite akrivos me to 4 
	* h logikh einai c=a*b
	* arrayD:o pinakas pou apo8hkeuei tis tetrades pou xrhsimopoioume ston upologismo ka8e stoixeiou tou array c */
void get_walltime(double *wct);

int main(int argc, char *argv[]) {
	float *arrayA, *arrayB, *arrayC, *arrayD;
	int a=0, b =0, c=0 , d=0;
	double ts = 0;		
	double te = 0;		
	double mflop = 0;
	// desmeush mnhmhs xrhsimopoiontas thn  posix_memalign kai elegxos
	 a= posix_memalign((void**)&arrayA, 16, N*N*sizeof(float));
    if(a != 0) {//elegxos
        return 1;
    }
     b = posix_memalign((void**)&arrayB, 16, N*N*sizeof(float));
    if(b != 0) {//elegxos
        free(arrayA);
        return 1;
    }
     c = posix_memalign((void**)&arrayC, 16, N*N*sizeof(float));
    if(c != 0) {//elegxos
        free(arrayA);
        free(arrayB);
        return 1;
    }
	 d = posix_memalign((void**)&arrayD, 16, 4*sizeof(float));
    if(d != 0) {//elegxos
        free(arrayA);
        free(arrayB);
		free(arrayC);
        return 1;
    }


	

	int i, j, k;
	//arxikopoihsh twn triwn pinakwn a b c
	float *pa = arrayA, *pb = arrayB, *pc = arrayC;
	for(i=0; i<N*N; i++) {
		*pa = 2.0;
		 pa++;
		*pb = 3.0;
		 pb++;
		*pc = 0.0;
		 pc++;
	}
	
	//arxikopoioume ton pinaka d pou apo8hkeuoume mesa ta apotelesmata tis ka8e tetradas
	float *pd = arrayD;
	for(i=0; i<4; i++) {
		*pd = 0.0;
		pd++;
	}
	
	// 8etoume tous pointers na deixnoun pali sthn arxh twn pinakw
	pa = arrayA; pb = arrayB; pc = arrayC; pd= arrayD;
	
	__m128 *va, *vb= (__m128*)pc, *vd = (__m128*)pd;
	
	
	//Φορτίο
	get_walltime(&ts);
	for(k=0; k<N; k++) {
		vb = (__m128*)pb;
		for(j=0; j<N; j++) {
			va = (__m128*)(arrayA + k*N);
			*vd = _mm_set_ps(0.0, 0.0, 0.0, 0.0);		// arxikopoihsh tou pointer vd se 0 o opoio deixnei ston pinaka d 
			for(i=0; i<N; i+=4) {
				*vd = _mm_add_ps(*vd, _mm_mul_ps(*va, *vb));
				va++; vb++;
			}
			
			pd = arrayD;
			for(i=0; i<4; i++) {
				*pc = *pc + *pd;//ypologismos apotelsematos pc  gia to ekastote stoixeio pou trexei ekeinh thn stigmh
				pd++;
			}
			pc++;
		}
	}
	get_walltime(&te);
	
	// upologismos xronou
	mflop = (N*N*N)/((te-ts) * 1e6);
	printf("Xronos: %f,  MFLOPS/s: %f\n", te-ts, mflop);
	
	// elegxoume ta apotelesmata tou fortiou
  for(i=0;i<N*N;i++){
		if(arrayC[i] != 3.0 * 2.0 * N){
			printf("%f\n", arrayC[i]);
			printf("error\n");
			break;

		} 
	}

	


	//apodesmeush mnhmhs twn pinakwn
	free(arrayA);
	free(arrayB);
	free(arrayC);
	free(arrayD);
	
	return 0;
}

void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}
