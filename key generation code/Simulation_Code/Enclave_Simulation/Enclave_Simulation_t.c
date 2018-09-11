#include "Enclave_Simulation_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */

#include <errno.h>
#include <string.h> /* for memcpy etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)


typedef struct ms_generate_key_t {
	unsigned int ms_retval;
	int ms_key_size_in_bytes;
} ms_generate_key_t;

typedef struct ms_transmit_key_t {
	unsigned char* ms_keys;
	int ms_key_size_in_bytes;
	int ms_char_size;
} ms_transmit_key_t;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#endif

static sgx_status_t SGX_CDECL sgx_generate_key(void* pms)
{
	ms_generate_key_t* ms = SGX_CAST(ms_generate_key_t*, pms);
	sgx_status_t status = SGX_SUCCESS;

	CHECK_REF_POINTER(pms, sizeof(ms_generate_key_t));

	ms->ms_retval = generate_key(ms->ms_key_size_in_bytes);


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv;} ecall_table[1];
} g_ecall_table = {
	1,
	{
		{(void*)(uintptr_t)sgx_generate_key, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[1][1];
} g_dyn_entry_table = {
	1,
	{
		{0, },
	}
};


sgx_status_t SGX_CDECL transmit_key(unsigned char* keys, int key_size_in_bytes, int char_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_keys = key_size_in_bytes * char_size;

	ms_transmit_key_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_transmit_key_t);
	void *__tmp = NULL;

	ocalloc_size += (keys != NULL && sgx_is_within_enclave(keys, _len_keys)) ? _len_keys : 0;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_transmit_key_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_transmit_key_t));

	if (keys != NULL && sgx_is_within_enclave(keys, _len_keys)) {
		ms->ms_keys = (unsigned char*)__tmp;
		__tmp = (void *)((size_t)__tmp + _len_keys);
		memcpy(ms->ms_keys, keys, _len_keys);
	} else if (keys == NULL) {
		ms->ms_keys = NULL;
	} else {
		sgx_ocfree();
		return SGX_ERROR_INVALID_PARAMETER;
	}
	
	ms->ms_key_size_in_bytes = key_size_in_bytes;
	ms->ms_char_size = char_size;
	status = sgx_ocall(0, ms);


	sgx_ocfree();
	return status;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
