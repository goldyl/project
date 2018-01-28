#ifndef ENCLAVE_SIMULATION_T_H__
#define ENCLAVE_SIMULATION_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif


unsigned int generate_key(size_t key_size_in_bytes);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
