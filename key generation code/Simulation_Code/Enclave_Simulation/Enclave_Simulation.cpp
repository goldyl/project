#include "Enclave_Simulation_t.h"
#include "sgx_trts.h"
#include <string.h>
#include <new>

using namespace std;



unsigned int generate_key(size_t key_size_in_bytes)
{
	unsigned char  * keys ;
	sgx_status_t status;
	int array_size = 0;
	
	keys = new (nothrow) unsigned char [key_size_in_bytes];
	if (keys == nullptr)
		return 0;
	
	status = sgx_read_rand(keys, key_size_in_bytes);
	
	array_size = sizeof(keys);

	if (status != SGX_SUCCESS) return 0;

	return 1;

}
