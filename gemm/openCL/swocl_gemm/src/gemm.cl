
__kernel void gemm_v(                   
             __global int * restrict A,          
             __global int * restrict  B,         
             __global int * restrict  C         
             ,int size           
              ){                        
   int sum=0;                            
   int width=size;                          
   int row = get_global_id(1);           
   int col = get_global_id(0);           

   for(int i=0;i<width;i++){              
        sum += A[(row*width)+i] * B[(width*i)+col]; 
   }                                     
                                         
   C[(row*width)+col] = sum;      
  
}                                        

