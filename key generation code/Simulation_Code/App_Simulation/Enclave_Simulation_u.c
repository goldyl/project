#include "Enclave_Simulation_u.h"
#include <errno.h>

typedef struct ms_generate_key_t {
	unsigned int ms_retval;
	int ms_key_size_in_bytes;
} ms_generate_key_t;

typedef struct ms_transmit_key_t {
	unsigned char* ms_keys;
	int ms_key_size_in_bytes;
	int ms_char_size;
} ms_transmit_key_t;

static sgx_status_t SGX_CDECL Enclave_Simulation_transmit_key(void* pms)
{
	ms_transmit_key_t* ms = SGX_CAST(ms_transmit_key_t*, pms);
	transmit_key(ms->ms_keys, ms->ms_key_size_in_bytes, ms->ms_char_size);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[1];
} ocall_table_Enclave_Simulation = {
	1,
	{
		(void*)(uintptr_t)Enclave_Simulation_transmit_key,
	}
};

sgx_status_t generate_key(sgx_enclave_id_t eid, unsigned int* retval, int key_size_in_bytes)
{
	sgx_status_t status;
	ms_generate_key_t ms;
	ms.ms_key_size_in_bytes = key_size_in_bytes;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave_Simulation, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

