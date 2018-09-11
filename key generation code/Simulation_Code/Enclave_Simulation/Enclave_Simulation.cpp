#include "Enclave_Simulation_t.h"
#include "sgx_trts.h"
#include <string.h>
#include <new>



using namespace std;



unsigned int generate_key(int key_size_in_bytes)
{
	unsigned char  * keys ;
	sgx_status_t status;
	int array_size = 0;
	
	keys = (unsigned char *)(malloc(sizeof(unsigned char) * key_size_in_bytes + 1));
	if (keys == nullptr)
		return 0;
	keys[key_size_in_bytes] = '\0';
	status = sgx_read_rand(keys, key_size_in_bytes);

	if (status != SGX_SUCCESS) return 0;
	
	//Ocall 
	//in next phase of the project - have to ovid sending the key outside of the enclave
	transmit_key(keys, key_size_in_bytes+1, sizeof(char));
	free(keys);
	return 1;
}


