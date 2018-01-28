#include "Enclave_Simulation_u.h"
#include <errno.h>

typedef struct ms_generate_key_t {
	unsigned int ms_retval;
	size_t ms_key_size_in_bytes;
} ms_generate_key_t;

static const struct {
	size_t nr_ocall;
	void * func_addr[1];
} ocall_table_Enclave_Simulation = {
	0,
	{ NULL },
};

sgx_status_t generate_key(sgx_enclave_id_t eid, unsigned int* retval, size_t key_size_in_bytes)
{
	sgx_status_t status;
	ms_generate_key_t ms;
	ms.ms_key_size_in_bytes = key_size_in_bytes;
	status = sgx_ecall(eid, 0, &ocall_table_Enclave_Simulation, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

