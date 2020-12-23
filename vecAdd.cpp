/*

Sources: http://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/

*/

// openCL headers

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <hsakmt.h>
#include <amd-dbgapi.h>



#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_SOURCE_SIZE (0x100000)

#include <execinfo.h>
void print_trace(void) {
    char **strings;
    size_t i, size;
    enum Constexpr { MAX_SIZE = 1024 };
    void *array[MAX_SIZE];
    size = backtrace(array, MAX_SIZE);
    strings = backtrace_symbols(array, size);
    for (i = 0; i < size; i++)
        printf("%s\n", strings[i]);
    puts("");
    free(strings);
}


void* allocate_memory(size_t byte_size) {
	printf("************ Call allocate_memory ! \n");
	return malloc(byte_size);
}

void deallocate_memory(void *data) {
	printf("************ Call deallocate_memory ! \n");
	return;
}

amd_dbgapi_status_t get_os_pid (
		amd_dbgapi_client_process_id_t client_process_id,
		amd_dbgapi_os_process_id_t *os_pid) {
	*os_pid = getpid();
	printf("************ Call get_os_pid ! \n");
	return AMD_DBGAPI_STATUS_SUCCESS;
}

amd_dbgapi_status_t enable_notify_shared_library (
      amd_dbgapi_client_process_id_t client_process_id,
      const char *shared_library_name,
      amd_dbgapi_shared_library_id_t shared_library_id,
      amd_dbgapi_shared_library_state_t *shared_library_state) {
	
	print_trace();
	printf("************ Call enable_notify_shared_library ! \n");
	return AMD_DBGAPI_STATUS_ERROR;
}

amd_dbgapi_status_t disable_notify_shared_library (
      amd_dbgapi_client_process_id_t client_process_id,
      amd_dbgapi_shared_library_id_t shared_library_id) {
	print_trace();
	printf("************ Call disable_notify_shared_library ! \n");
	return AMD_DBGAPI_STATUS_ERROR;
}

amd_dbgapi_status_t get_symbol_address (
      amd_dbgapi_client_process_id_t client_process_id,
      amd_dbgapi_shared_library_id_t shared_library_id,
      const char *symbol_name, amd_dbgapi_global_address_t *address) {
	printf("************ Call get_symbol_address ! \n");
	return AMD_DBGAPI_STATUS_ERROR;
}

amd_dbgapi_status_t insert_breakpoint (
      amd_dbgapi_client_process_id_t client_process_id,
      amd_dbgapi_shared_library_id_t shared_library_id,
      amd_dbgapi_global_address_t address,
      amd_dbgapi_breakpoint_id_t breakpoint_id) {
	printf("************ Call insert_breakpoint ! \n");
	return AMD_DBGAPI_STATUS_ERROR;
}

amd_dbgapi_status_t remove_breakpoint (
      amd_dbgapi_client_process_id_t client_process_id,
      amd_dbgapi_breakpoint_id_t breakpoint_id) {
	printf("************ Call remove_breakpoint ! \n");
	return AMD_DBGAPI_STATUS_ERROR;
}

void log_message (amd_dbgapi_log_level_t level, const char *message) {
	printf("************ Call log_message ! \n");
	return;
}

struct amd_dbgapi_client_process_s
{
	pid_t pid;
};


static amd_dbgapi_callbacks_t dbgapi_callbacks = {
  /* allocate_memory.  */
  .allocate_memory = malloc,

  /* deallocate_memory.  */
  .deallocate_memory = free,

  /* get_os_pid.  */
  .get_os_pid =
      [] (amd_dbgapi_client_process_id_t client_process_id, pid_t *pid) {
		printf("************ Call get_os_pid ! \n");
        *pid = getpid ();
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* enable_notify_shared_library callback.  */
  .enable_notify_shared_library =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          const char *library_name, amd_dbgapi_shared_library_id_t library_id,
          amd_dbgapi_shared_library_state_t *library_state) {
        /* If the debug agent is loaded, then the ROCR is already loaded.   */
		printf("************ Call enable_notify_shared_library ! \n");

		library_id.handle = (uint64_t)dlopen(library_name, RTLD_NOW);
        *library_state = AMD_DBGAPI_SHARED_LIBRARY_STATE_LOADED;
		std::cout << "The loaded address is " << (void*) library_id.handle << std::endl;
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* disable_notify_shared_library callback.  */
  .disable_notify_shared_library =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t library_id) {
		printf("************ Call disable_notify_shared_library ! \n");
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* get_symbol_address callback.  */
  .get_symbol_address =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t library_id, const char *symbol_name,
          amd_dbgapi_global_address_t *address) {
		std::cout << "Call get_symbol_address" << std::endl;
        *address = reinterpret_cast<amd_dbgapi_global_address_t> (
            dlsym (RTLD_DEFAULT, symbol_name));
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* set_breakpoint callback.  */
  .insert_breakpoint =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t shared_library_id,
          amd_dbgapi_global_address_t address,
          amd_dbgapi_breakpoint_id_t breakpoint_id) {
		printf("************ Call insert_breakpoint ! \n");
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* remove_breakpoint callback.  */
  .remove_breakpoint =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_breakpoint_id_t breakpoint_id) {
		printf("************ Call remove_breakpoint ! \n");
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* log_message callback.  */
  .log_message =
      [] (amd_dbgapi_log_level_t level, const char *message) {
		printf("************ Call log_message ! \n");
        std::cout << "rocm-dbgapi: " << message << std::endl;
      }
};

int main(int argc, char ** argv) {

	int SIZE = 1024*1024;

	// Allocate memories for input arrays and output array.
	float *A = (float*)malloc(sizeof(float)*SIZE);
	float *B = (float*)malloc(sizeof(float)*SIZE);

	// Output
	float *C = (float*)malloc(sizeof(float)*SIZE);
	
	
	// Initialize values for array members.
	int i = 0;
	for (i=0; i<SIZE; ++i) {
		A[i] = i+1;
		B[i] = (i+1)*2;
	}

	// Load kernel from file vecAddKernel.cl

	FILE *kernelFile;
	char *kernelSource;
	size_t kernelSize;

	kernelFile = fopen("vecAddKernel.cl", "r");

	if (!kernelFile) {

		fprintf(stderr, "No file named vecAddKernel.cl was found\n");

		exit(-1);

	}
	kernelSource = (char*)malloc(MAX_SOURCE_SIZE);
	kernelSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
	fclose(kernelFile);

	// Getting platform and device information
	cl_platform_id platformId = NULL;
	cl_device_id deviceID = NULL;
	cl_uint retNumDevices;
	cl_uint retNumPlatforms;
	cl_int ret = clGetPlatformIDs(1, &platformId, &retNumPlatforms);
	ret = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, &retNumDevices);

	// Creating context.
	cl_context context = clCreateContext(NULL, 1, &deviceID, NULL, NULL,  &ret);


	// Creating command queue
	cl_command_queue commandQueue = clCreateCommandQueue(context, deviceID, 0, &ret);

	// HsaVersionInfo VersionInfo;
	// HSAKMT_STATUS kmt_ret = hsaKmtGetVersion(&VersionInfo);
	// printf("The return value of hsaKmtGetVersion is %d \n", kmt_ret);
	
	// printf("VersionInfo %u %u \n", VersionInfo.KernelInterfaceMajorVersion, VersionInfo.KernelInterfaceMinorVersion);

	// HSAuint32 Major, Minor;
	// kmt_ret = hsaKmtGetKernelDebugTrapVersionInfo(
    // &Major, &Minor);
	// printf("Major Minor %u %u \n", Major, Minor);
	// printf("The return value of hsaKmtGetKernelDebugTrapVersionInfo is %d \n", kmt_ret);


	// HsaNodeProperties NodeProperties;
	// kmt_ret = hsaKmtGetNodeProperties(1, &NodeProperties);
	// printf("The return value of hsaKmtGetNodeProperties is %d \n", kmt_ret);
	
	// printf("NodeProperties %lu \n", NodeProperties.DebugProperties.Value);



	// kmt_ret = hsaKmtEnableDebugTrap(1, INVALID_QUEUEID);
	// printf("The return value of hsaKmtEnableDebugTrap is %d \n", kmt_ret);

	// kmt_ret = hsaKmtDbgRegister(1);
	// printf("The return value of hsaKmtDbgRegister is %d \n", kmt_ret);

//	kmt_ret = hsaKmtDisableDebugTrap(1);
//	printf("The return value of hsaKmtDisableDebugTrap is %d \n", kmt_ret);






	// Memory buffers for each array
	cl_mem aMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &ret);
	cl_mem bMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &ret);
	cl_mem cMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(float), NULL, &ret);


	// Copy lists to memory buffers
	ret = clEnqueueWriteBuffer(commandQueue, aMemObj, CL_TRUE, 0, SIZE * sizeof(float), A, 0, NULL, NULL);;
	ret = clEnqueueWriteBuffer(commandQueue, bMemObj, CL_TRUE, 0, SIZE * sizeof(float), B, 0, NULL, NULL);

	// Create program from kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, (const size_t *)&kernelSize, &ret);	

	// Build program
	ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	// Create kernel
	cl_kernel kernel = clCreateKernel(program, "addVectors", &ret);


	// Set arguments for kernel
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&aMemObj);	
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bMemObj);	
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&cMemObj);	

	// kmt_ret = hsaKmtSetWaveLaunchMode(1, HSA_DBG_WAVE_LAUNCH_MODE_HALT);
	// printf("The return value of hsaKmtSetWaveLaunchMode is %d \n", kmt_ret);


	HsaDbgWaveMsgAMDGen2 wavemsggen2;
	
	HsaDbgWaveMessageAMD wavemsg;
	wavemsg.WaveMsgInfoGen2 = wavemsggen2;

	HsaDbgWaveMessage msg;
	void* memoryva = malloc(1024 * 1024 * 1024);
	msg.MemoryVA = memoryva;
	msg.DbgWaveMsg = wavemsg;




	fflush(stdout);
	printf("Before clEnqueueNDRangeKernel \n");
	fflush(stdout);
	// Execute the kernel
	size_t globalItemSize = SIZE;
	size_t localItemSize = 64; // globalItemSize has to be a multiple of localItemSize. 1024/64 = 16 
	ret = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);	
	fflush(stdout);
	printf("After clEnqueueNDRangeKernel \n");
	fflush(stdout);


	printf("Before half!!! \n");
	
	// amd_dbgapi_callbacks_s callback;

	// callback.allocate_memory = &allocate_memory;
	// callback.deallocate_memory = &deallocate_memory;
	// callback.get_os_pid = &get_os_pid;
	// callback.enable_notify_shared_library = &enable_notify_shared_library;
	// callback.disable_notify_shared_library = &disable_notify_shared_library;
	// callback.get_symbol_address = &get_symbol_address;
	// callback.insert_breakpoint = &insert_breakpoint;
	// callback.remove_breakpoint = &remove_breakpoint;
	// callback.log_message = &log_message;

	
	amd_dbgapi_status_t dbg_ret = amd_dbgapi_initialize (&dbgapi_callbacks);
	printf("The return value of amd_dbgapi_initialize is %d \n", dbg_ret);

	amd_dbgapi_process_id_t process_id;
	struct amd_dbgapi_client_process_s user_process_id;
	user_process_id.pid = getpid();

	dbg_ret = amd_dbgapi_process_attach (&user_process_id, &process_id);
	printf("The return value of amd_dbgapi_process_attach is %d \n", dbg_ret);


	size_t wave_count;
	amd_dbgapi_wave_id_t* waves;
	dbg_ret = amd_dbgapi_process_wave_list (
    	AMD_DBGAPI_PROCESS_NONE, &wave_count, &waves, NULL);

	printf("The return value of amd_dbgapi_process_wave_list is %d and the value is %lu \n", dbg_ret, wave_count);


	amd_dbgapi_global_address_t value;
	dbg_ret = amd_dbgapi_wave_get_info (
		AMD_DBGAPI_WAVE_NONE,
        AMD_DBGAPI_WAVE_INFO_PC, 
		sizeof(amd_dbgapi_global_address_t),
        &value);

	printf("The return value of amd_dbgapi_wave_get_info is %d and the value is %lu \n", dbg_ret, value);

	

	// hsaKmtDbgWavefrontControl(1, HSA_DBG_WAVEOP_KILL, HSA_DBG_WAVEMODE_BROADCAST_PROCESS, 1, &msg);

	printf("We halt it!!! \n");
	// kmt_ret = hsaKmtQueueSuspend(
    //    -1 ,/*HSAuint32    Pid,*/
    //    1 ,/*HSAuint32    NumQueues,*/
    //    NULL,/*HSA_QUEUEID *Queues,*/
    //    0,/*HSAuint32    GracePeriod,*/
    //    2);/*This is true QueueID */

	// kmt_ret = hsaKmtQueueResume(-1, 1, NULL, 2);
	// printf("The return value is %d \n", kmt_ret);


	// kmt_ret = hsaKmtSetWaveLaunchMode(1, HSA_DBG_WAVE_LAUNCH_MODE_NORMAL);
	// printf("The return value of hsaKmtSetWaveLaunchMode is %d \n", kmt_ret);

	// Read from device back to host.
	ret = clEnqueueReadBuffer(commandQueue, cMemObj, CL_TRUE, 0, SIZE * sizeof(float), C, 0, NULL, NULL);
	printf("So no this line!!!");

	// Write result
	/*
	for (i=0; i<SIZE; ++i) {

		printf("%f + %f = %f\n", A[i], B[i], C[i]);

	}
	*/

	// Test if correct answer
	for (i=0; i<SIZE; ++i) {
		if (C[i] != 1024*(A[i] + B[i])) {
			printf("Something didn't work correctly! Failed test. %d: %f, %f, %f\n", i, C[i], A[i], B[i]);
			break;
		}
	}
	if (i == SIZE) {
		printf("Everything seems to work fine! \n");
	}

	
	// kmt_ret = hsaKmtDbgUnregister(1);
	// printf("The return value of hsaKmtDbgUnregister is %d \n", kmt_ret);


	// Clean up, release memory.
	ret = clFlush(commandQueue);
	ret = clFinish(commandQueue);
	ret = clReleaseCommandQueue(commandQueue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(aMemObj);
	ret = clReleaseMemObject(bMemObj);
	ret = clReleaseMemObject(cMemObj);
	ret = clReleaseContext(context);
	free(A);
	free(B);
	free(C);

	return 0;

	}