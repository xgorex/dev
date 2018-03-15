

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#include <stdint.h>


//char *kernel_vecadd = "__kernel void vecadd(__global int *A, __global int *B, __global int *C)  {int idx = get_global_id(0); //int idy = get_global_id(1); C[idx] = A[idx] + B[idx];}";
char *kernel_vecadd = "__kernel void vecadd(__global int *A, __global int *B, __global int *C)  {int sum=0; int width=5; int row = get_global_id(0); int col=get_global_id(1); for(int i=0;i<width;i++){ sum += A[(row*width)+i] * B[(width*i)+col];    }   C[(row*width)+col] = sum;}";



#pragma OPENCL EXTENSION cl_intel_printf : enable
char *kernel_gemm_v=
"__kernel void gemm_v(                   "
"             __global int *A,            "
"             __global int *B,            "
"             __global int *C            "
"             ,int size           "
"              ){                         "
"   int sum=0;                            "
"   int width=size;                          "
"   int row = get_global_id(1);           "
"   int col = get_global_id(0);           "

"   for(int i=0;i<width;i++){              "
"        sum += A[(row*width)+i] * B[(width*i)+col]; "
"   }                                     "
"                                         "
"   C[(row*width)+col] = sum;      "
""  
"}                                        ";


//////////////////const char* programSource = “__kernel void vecadd(__global int *A, __global int *C) {int idx = get_global_id(0); C[idx] = A[idx];}”;

int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
        return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
                              ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}


int main(){

   cl_int status;
   cl_uint numPlatforms;

   cl_platform_id *platform; 

   //get number of platforms
   status = clGetPlatformIDs(0, NULL, &numPlatforms);

   if(CL_SUCCESS == status){
      printf("platforms counter:%d\n",numPlatforms);
      platform =(cl_platform_id*) malloc(numPlatforms * sizeof(cl_platform_id));
   }
   status = clGetPlatformIDs(numPlatforms, platform, 0);

   if(CL_SUCCESS == status){
      printf("Got platforms ID\n");
   }

   //get platform infor: NAME
   size_t namePlatformCount;

   status = clGetPlatformInfo(
            platform[0],
            CL_PLATFORM_NAME,
            0,
            NULL,
            &namePlatformCount
         );

   char *namePlatform;
   if(CL_SUCCESS == status){
      namePlatform = (char*)malloc(namePlatformCount * sizeof(char));
   }
   
   status = clGetPlatformInfo(
            platform[0],
            CL_PLATFORM_NAME,
            namePlatformCount,
            namePlatform,
            NULL
         );
   if(CL_SUCCESS == status){
      printf("Platform name:%s\n",namePlatform);
   }

   //get number of devices
   cl_uint numDevices;
   cl_device_id *device;
   status = clGetDeviceIDs(platform[0],CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);

   if(CL_SUCCESS == status){
      printf("Devices count:%d\n",numDevices);
      device = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
   }


   status = clGetDeviceIDs(platform[0],CL_DEVICE_TYPE_CPU, numDevices, device, NULL);
   if(CL_SUCCESS == status){
      printf("Got devices ID\n");
   }

   size_t infoDeviceSize;
   status = clGetDeviceInfo(
            device[0],
            CL_DEVICE_TYPE,
            0,
            NULL,
            &infoDeviceSize
         ); 

   //cl_device_type *paramValue;
   cl_device_type paramValue;
   if(CL_SUCCESS == status){
      printf("[DEV] Got device type size:%zd\n",infoDeviceSize);
      //paramValue = malloc(infoDeviceSize * sizeof(cl_device_type));
   }

   status = clGetDeviceInfo(
            device[0],
            CL_DEVICE_TYPE,
            infoDeviceSize,
            &paramValue,
            NULL
         ); 

   if(CL_SUCCESS == status){
      switch(paramValue){
         case CL_DEVICE_TYPE_CPU:
            printf("[DEV] Device type: CPU\n");
            break;
         case CL_DEVICE_TYPE_GPU:   
            printf("[DEV] Device type: GPU\n");
            break;
         default:   
            printf("[DEV] NO DEVICES QUERED !!!\n");
      }
   }   

      //query maximum compute units on device
   cl_uint paramValue_MAX_CUs;
   status = clGetDeviceInfo(
            device[0],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            infoDeviceSize,
            &paramValue_MAX_CUs,
            NULL
         ); 

   if(CL_SUCCESS == status){
      printf("[DEV] Max compute units:%d\n",paramValue_MAX_CUs);
   }   

     //query maximum WORK ITEM DIMENTION for device
   cl_uint paramValue_WID;
   status = clGetDeviceInfo(
            device[0],
            CL_DEVICE_MAX_COMPUTE_UNITS,
            infoDeviceSize,
            &paramValue_WID,
            NULL
         ); 

   if(CL_SUCCESS == status){
      printf("[DEV] Max work items dimention:%d\n",paramValue_WID);
   }

     //query maximum WORK ITEM SIZES for device
   size_t paramValue_WIS;
   //paramValue_WIS = (void*)malloc(2*sizeof(size_t*));


   status = clGetDeviceInfo(
            device[0],
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            infoDeviceSize,
            &paramValue_WIS,
            NULL
         ); 

   if(CL_SUCCESS == status){
      printf("[DEV] Max work items size:%zd\n",paramValue_WIS);
   }else{
     if(CL_INVALID_DEVICE == status)printf("CL_INVALID_DEVICE\n");
     if(CL_INVALID_VALUE == status)printf("CL_INVALID_VALUE\n");
     if(CL_OUT_OF_RESOURCES == status)printf("CL_OUT_OF_RESOURCES\n");
     if(CL_OUT_OF_HOST_MEMORY == status)printf("CL_OUT_OF_HOST_MEMORY\n");

     return 2;
   
   }

   //CREATE CONTEXT FOR FIRST POOL OF DEVICES
   cl_context context; 
   context = clCreateContext(
           NULL,
           numDevices,
           device,
           NULL,
           NULL,
           &status
         );

   cl_command_queue cmdQ;
   cmdQ = clCreateCommandQueue(
            context,
            device[0],
            0,
            &status
         );


   int elements = 500;
   //size_t datasize = elements * sizeof(int);

   int sysBuffA[elements][elements];

   for(int i =0; i<elements;i++)
      for(int y =0; y<elements;y++)
         sysBuffA[i][y]=i+y;

   cl_mem bufferA;
   bufferA = clCreateBuffer(
            context,
            CL_MEM_READ_ONLY,
            elements * elements * sizeof(int),
            NULL,
            &status
         );

   status = clEnqueueWriteBuffer(
            cmdQ,
            bufferA,
            CL_TRUE,
            0,
            elements * elements * sizeof(int),
            sysBuffA,
            0,
            NULL,
            NULL
         );


   int sysBuffB[elements][elements];

   for(int i =0; i<elements;i++)
      for(int y =0; y<elements;y++)
         sysBuffB[i][y]=i+y;

   cl_mem bufferB;
   bufferB = clCreateBuffer(
            context,
            CL_MEM_READ_ONLY,
            elements * elements * sizeof(int),
            NULL,
            &status
         );

   status = clEnqueueWriteBuffer(
            cmdQ,
            bufferB,
            CL_TRUE,
            0,
            elements * elements * sizeof(int),
            sysBuffB,
            0,
            NULL,
            NULL
         );

   int sysBuffC[elements][elements];

   cl_mem bufferC;
   bufferC = clCreateBuffer(
            context,
            CL_MEM_WRITE_ONLY,
            elements * elements * sizeof(int),
            NULL,
            &status
         );
    // Create a program with source code
   
   printf("[CL] Create programm from kernel\n"); 
    cl_program kernel_source = clCreateProgramWithSource(
            context,
            1,
            (const char**)&kernel_gemm_v,
            NULL,
            &status
         ); 

 // Build (compile) the program for the device
 
    printf("[CL] Build programm\n");
    status = clBuildProgram(
            kernel_source,
            0,
            NULL,
            NULL,
            NULL,
            NULL
          );


    // Create the vector addition kernel
    //
    printf("[CL] Create kernel\n");
     cl_kernel kernel;
     kernel = clCreateKernel(
            kernel_source,
            "gemm_v",
            &status
          );

   status = clSetKernelArg(
            kernel,
            0,
            sizeof(cl_mem),
            (void*)&bufferA
          );
   status = clSetKernelArg(
            kernel,
            1,
            sizeof(cl_mem),
            (void*)&bufferB
          );
   status = clSetKernelArg(
            kernel,
            2,
            sizeof(cl_mem),
            (void*)&bufferC
          );
   status = clSetKernelArg(
            kernel,
            3,
            sizeof(int),
            (void*)&elements 
          );

   //EXECUTION

   // Define an index space (global work size) of work
   //     // items for execution. A workgroup size (local work size)
   //         // is not required, but can be used.
   size_t globalWorkSize[2] = {elements,elements};

   struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);

   status = clEnqueueNDRangeKernel(
               cmdQ,
               kernel,
               2,
               NULL,
               globalWorkSize,
               NULL,
               0,
               NULL,
               NULL
            );

      clock_gettime(CLOCK_MONOTONIC, &end);
   uint64_t timeElapsed = timespecDiff(&end, &start);
   int timeElapsed_sec = timeElapsed / 1000000000; //get just seconds
   printf("[DEV]Time spent in compute:\n [ns]:%ld\n  [s]:%d\n",timeElapsed,timeElapsed_sec);



   printf("[DEV] Compute DONE.\n");

   clEnqueueReadBuffer(
               cmdQ,
               bufferC,
               CL_TRUE,
               0,
               elements * elements * sizeof(int),
               sysBuffC,
               0,
               NULL,
               NULL);


   FILE *mfile;
   mfile = fopen("sysBuffC.dump","w");

   for(int h=0; h<elements;h++){
      for(int w=0; w<elements;w++){
         //printf("Tablica C[%d][%d]=%d\n",h,w,sysBuffC[h][w]);
         fprintf(mfile,"%d\n",sysBuffC[h][w]);
      }   
   }

   fclose(mfile);

   free(platform); 
   free(device);

   printf("[ALL] DONE.\n");
   return 0;
}
