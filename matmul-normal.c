#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//ergasia gia to ma8hma parallhlos programatismos H examhnou 
/* 
1. DESMEFSH PINAKON
2. ARXIKOPOISH
3. FOSRTIO - LOAD
4. ELENXOS 
5. APOSESMESH PINAKON - DEDOMENON
*/
void get_walltime(double *wct);


int main(){
    double ts = 0;		
	double te = 0;		
	double mflop = 0;
    float *arrayA,*arrayB,*arrayC;
    
//desmeush pinakwn a b c kai elegxos   
    
arrayA =(float*)malloc(N * N * sizeof(float));
if(arrayA==NULL){
    
    return 1;
}
arrayB =(float*)malloc(N * N * sizeof(float));

if(arrayB==NULL){
    free(arrayA);
    return 1;
}
//o pinakas c,einai o pinakas pou apo8hkeuoume to apotelesma a * b 
arrayC =(float*)malloc(N * N * sizeof(float));

if(arrayC==NULL){
    free(arrayA);
    free(arrayB);
    return 1;
}
 
 float *pa=arrayA,*pb=arrayB,*pc=arrayC; //8etoume tous pointers na deixnoun sthn arxh tou pinaka 
 for(int i=0;i<N*N;i++){ //arxikopoioume tous pinakes 
     *pa=2.0;
      pa++;
     *pb=3.0;
      pb++;
     *pc=0.0;
      pc++;
 }
 pa=arrayA, pb=arrayB, pc=arrayC; //8etoume tous pointers xana na deixoun thn arxh twn pinakwn
 //pa to xrhsimopoioume gia rows kai to pb gia collums
  get_walltime(&ts);
  for(int k=0;k<N;k++){
		pb = arrayB;			
		for(int j=0;j<N;j++){
			pa = arrayA + k * N;	//
			for(int i=0;i<N;i++){
				*pc += (*pa) * (*pb);	
				pa++;			//auxanoume ton pointer gia ton pinaka a
				pb++;			//auxanoume ton pointer gia ton pinaka b

			} 

			pc++;				//auxanoume ton pointer gia na deixei sto epomeno stoixeio tou pinaka c

		} 
	} 
    get_walltime(&te);
    
    for(int i=0;i<N*N;i++){
		if(arrayC[i]!= 3.0 * 2.0 * N){
			printf("%f\n", arrayC[i]);
			printf("error\n");
			break;

		} 
		pc++;
	} 
	
	
	mflop = (N*N*N)/((te-ts) * 1e6);
	printf("Xronos: %f,  MFLOPS/s: %f\n", te-ts, mflop);
	//apodesmeush pinakwn
	free(arrayA);
	free(arrayB);
	free(arrayC);
  
    return 0;
}
void get_walltime(double *wct) {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	*wct = (double) (tp.tv_sec + tp.tv_usec/1000000.0);

}
