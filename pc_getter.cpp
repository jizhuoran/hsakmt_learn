#include "pc_getter.hpp"

#include <iostream>
#include <unistd.h>
#include <map>
#include <dlfcn.h>

#define AMDDBGAPI_CHECK(condition) \
  do { \
    amd_dbgapi_status_t error = condition; \
    if(error != AMD_DBGAPI_STATUS_SUCCESS) { \
      std::cerr << "This is a error for AMDDBGAPI "<< error << " in " << __LINE__ << " in " << __FILE__ << std::endl;\
      exit(1); \
    } \
  } while (0)
  

struct amd_dbgapi_client_process_s {
	pid_t pid;
};

static std::map<uint64_t, void*> handle2address;

static amd_dbgapi_callbacks_t dbgapi_callbacks = {
  /* allocate_memory.  */
  .allocate_memory = malloc,

  /* deallocate_memory.  */
  .deallocate_memory = free,

  /* get_os_pid.  */
  .get_os_pid =
      [] (amd_dbgapi_client_process_id_t client_process_id, pid_t *pid) {
        *pid = getpid ();
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* enable_notify_shared_library callback.  */
  .enable_notify_shared_library =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          const char *library_name, amd_dbgapi_shared_library_id_t library_id,
          amd_dbgapi_shared_library_state_t *library_state) {
        /* If the debug agent is loaded, then the ROCR is already loaded.   */
		void* address = dlopen(library_name, RTLD_NOW);
        *library_state = AMD_DBGAPI_SHARED_LIBRARY_STATE_LOADED;
		handle2address[library_id.handle] = address;
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* disable_notify_shared_library callback.  */
  .disable_notify_shared_library =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t library_id) {
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* get_symbol_address callback.  */
  .get_symbol_address =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t library_id, const char *symbol_name,
          amd_dbgapi_global_address_t *address) {
        *address = reinterpret_cast<amd_dbgapi_global_address_t> (
            dlsym (handle2address[library_id.handle], symbol_name));
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* set_breakpoint callback.  */
  .insert_breakpoint =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_shared_library_id_t shared_library_id,
          amd_dbgapi_global_address_t address,
          amd_dbgapi_breakpoint_id_t breakpoint_id) {
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* remove_breakpoint callback.  */
  .remove_breakpoint =
      [] (amd_dbgapi_client_process_id_t client_process_id,
          amd_dbgapi_breakpoint_id_t breakpoint_id) {
        return AMD_DBGAPI_STATUS_SUCCESS;
      },

  /* log_message callback.  */
  .log_message =
      [] (amd_dbgapi_log_level_t level, const char *message) {
        std::cout << "rocm-dbgapi: " << message << std::endl;
      }
};

PCGetter::PCGetter() {
    AMDDBGAPI_CHECK(amd_dbgapi_initialize (&dbgapi_callbacks));
}

#define DEBUG

uint64_t PCGetter::stop_and_get_pc_offset() {

#ifdef DEBUG
    return 0x80;
#endif
    amd_dbgapi_process_id_t process_id;
    struct amd_dbgapi_client_process_s user_process_id {getpid()};
    AMDDBGAPI_CHECK(amd_dbgapi_process_attach(&user_process_id, &process_id));

    size_t wave_count;
    amd_dbgapi_wave_id_t* waves;
    AMDDBGAPI_CHECK(amd_dbgapi_process_wave_list(
        AMD_DBGAPI_PROCESS_NONE, &wave_count, &waves, NULL));

    AMDDBGAPI_CHECK(amd_dbgapi_wave_stop (waves[0]));

    amd_dbgapi_dispatch_id_t dispatcher;
    AMDDBGAPI_CHECK(amd_dbgapi_wave_get_info (
        waves[0], AMD_DBGAPI_WAVE_INFO_DISPATCH, 
        sizeof(amd_dbgapi_dispatch_id_t), &dispatcher));

    amd_dbgapi_global_address_t entry;
    AMDDBGAPI_CHECK(amd_dbgapi_dispatch_get_info (
        dispatcher, AMD_DBGAPI_DISPATCH_INFO_KERNEL_CODE_ENTRY_ADDRESS,
        sizeof(amd_dbgapi_global_address_t), &entry));

    amd_dbgapi_global_address_t PC;
    AMDDBGAPI_CHECK(amd_dbgapi_wave_get_info (
        waves[0], AMD_DBGAPI_WAVE_INFO_PC, 
        sizeof(amd_dbgapi_global_address_t), &PC));

    free(waves);

#ifdef DEBUG
    std::cout << "LOG: the num of wave is " << wave_count << std::endl;
    std::cout << "LOG: dispatch_id is " << dispatcher.handle << std::endl;
    std::cout << "LOG: entry is " << std::hex << entry << std::endl;
    std::cout << "LOG: PC is " << std::hex << PC << std::endl;
    std::cout << "PRINT: offset is " << std::hex << PC - entry << std::endl;
#endif //DEBUG
	
    return PC - entry;
}