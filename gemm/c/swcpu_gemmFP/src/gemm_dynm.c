

#define dbgNO 

#include <stdlib.h> 
#include <stdio.h> 
#include <time.h>
#include <stdint.h>


int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
     return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
                   ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}


int initMatrices(int hA, int wA, float **A, float **B, float **C){


   long counter=0;
   float value=0;
   for(int h=0;h<hA;h++)
      for(int w=0;w<wA; w++){
         //value=counter;
         //value=(long)rand() % 20;
         value = h+w;
         A[h][w]=value;
#ifdef dbg
         printf("[I] Init A[%d][%d]=%f--->%f\n",h,w,value,A[h][w]);
#endif
         counter++;
     } 

   counter=0;
   value=0;
   for(int h=0;h<wA;h++)
      for(int w=0;w<hA; w++){
         //memcpy(B+((w+h)*sizeof(int)),&counter,sizeof(int) );
         //value=counter;
         //value = (long)rand() % 20;
         value = h+w;
         B[h][w]= value;
#ifdef dbg
         printf("[I] Init B[%d][%d]=%f--->%f\n",h,w,counter,B[h][w]);
#endif
         counter++;
     } 

   for(int h=0;h<hA;h++)
      for(int w=0;w<hA; w++){
         //memcpy(C+((w+h)*sizeof(int)),&counter,sizeof(int) );
         C[h][w]=0;
#ifdef dbg
         printf("[I]Init C[%d][%d]=0--->%f\n",h,w,C[h][w]);
#endif
     } 
   
   return 0;
};



int checkMatrices(int hA, int wA, long **A,long **B){

   printf("[FI]Checking matrices...");

   for(int h=0;h<hA; h++)
     for(int w=0;w<wA;w++){
         printf("  [I]Check Init A[%d,%d]:%ld\n",h,w,A[h][w]);
     } 
   for(int h=0;h<wA;h++){
     for(int w=0;w<hA; w++)
         printf("  [I]Check Init B[%d,%d]:%ld\n",h,w,B[h][w]);
     } 

   printf("done.\n");
   return 0;
}


int computeMatrices(int hA, int wA, float **a, float **b, float **c){

   float sum=0;
   for(int h=0; h<hA; h++){
      for(int w=0; w<hA; w++){
         sum=0;
         for(int i=0; i<wA; i++){
         
            sum += (a[h][i] * b[i][w]);
#ifdef dbg
            printf("[INTER]<%d> c[%d][%d]=%f * %f; -> %f\n",i,h,w,a[h][i],b[i][w],c[h][w]);
#endif
         } 
         c[h][w] = sum;
      }

   }

   return 0;
}


int verifyCompute(int hA, long **c){

   printf("[FI]Veryfing computation work...\n");

   for(int h=0;h<hA;h++)
      for(int w=0;w<hA; w++){
         printf("  Wyniki:%ld\n",c[h][w]);
     } 

   printf("done\n");
   return 0;
}


int main(){

   struct timespec start, end;
   //matrix A sizes
   int hA = 500;
   int wA = 500;
   //matrix B sizes
   int hB = 500;
   int wB = 500;
   
   int wC, hC;

   if(hA != wB){ 
      printf("[ERR] Martices does not match; wA=%d,hA=%d,wB=%d,hB=%d\n",wA, hA, wB, hB);
      return 0;
   }
   //matrix C sizes
   //wC = hC = hA; 
   wC = wB;
   hC = hA;

   //setup output file
   FILE *mfile;

   //allocate matrices
   float **mA;
   float **mB;
   float **mC;
   
   mA = (float **)malloc(hA * sizeof(float *));
   for(int i=0; i<hA; i++)
      mA[i] = malloc(wA * sizeof(float));

   mB = (float **)malloc(hB * sizeof(float *));
   for(int i=0; i<hB; i++)
      mB[i] = malloc(wB * sizeof(float));

   mC = (float **)malloc(hC * sizeof(float *));
   for(int i=0; i<hC; i++)
      mC[i] = malloc(wC * sizeof(float));


   //start operations
   printf("[FI]Initializing matrices...");
   initMatrices(hA, wA, mA, mB, mC);
   printf("done.\n");
#ifdef dbg
//   checkMatrices(hA, wA, mA, mB);
#endif
   printf("[FI]Computing matrices for H=%d,W=%d...",hA,wA);
   
      clock_gettime(CLOCK_MONOTONIC, &start);
   computeMatrices(hA, wA, mA, mB, mC);
      clock_gettime(CLOCK_MONOTONIC, &end);

   uint64_t timeElapsed = timespecDiff(&end, &start);
   int timeElapsed_sec = timeElapsed / 1000000000; //get just seconds
   printf("done.\n");
   printf("[END]Time spent in compute:\n [ns]:%ld\n  [s]:%d\n",timeElapsed,timeElapsed_sec);

//   verifyCompute(hA,mC);
   mfile = fopen("mC.dump","w");
   for(int h =0; h<hC;h++)
      for(int w=0;w<wC;w++)
         fprintf(mfile,"%f\n",mC[h][w]);

   fclose(mfile);


   //free dynamically allocated matrices
   for(int i=0;i<wA;i++) free(mA[i]);
   free(mA);
   for(int i=0;i<wB;i++) free(mB[i]);
   free(mB);
   for(int i=0;i<wC;i++) free(mC[i]);
   free(mC);

   return 0;
}
