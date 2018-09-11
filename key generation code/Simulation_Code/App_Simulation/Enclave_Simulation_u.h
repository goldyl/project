#ifndef ENCLAVE_SIMULATION_U_H__
#define ENCLAVE_SIMULATION_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void SGX_UBRIDGE(SGX_NOCONVENTION, transmit_key, (unsigned char* keys, int key_size_in_bytes, int char_size));

sgx_status_t generate_key(sgx_enclave_id_t eid, unsigned int* retval, int key_size_in_bytes);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
