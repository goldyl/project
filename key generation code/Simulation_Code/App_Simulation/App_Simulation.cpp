#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "sgx_urts.h"
#include "Enclave_Simulation_u.h"
#define ENCLAVE_FILE _T("Enclave_Simulation.signed.dll")
#define MAX_BUF_LEN 100
#define CYPER_KEY_SIZE	256
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	sgx_status_t sgx_stat = SGX_SUCCESS;
	sgx_enclave_id_t eid = 0;
	sgx_launch_token_t token ={0};
	int updated = 0;
	char buffer[MAX_BUF_LEN] = "Hello World!";
	int enable_generate_keys = 1;
	unsigned int retval;


	// Create the Enclave with above launch token.
	sgx_stat = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);

	unsigned char key[CYPER_KEY_SIZE / 8] = { 0 };

	if (sgx_stat != SGX_SUCCESS) 
	{
		printf("App: error %#x, failed to create enclave.\n", sgx_stat);
		return -1;
	}


	//cout << "Would you like to generate keys? (1-yes, 0-no)";
	//cin		>> enable_generate_keys;

	if (enable_generate_keys)
	{
		if (SGX_SUCCESS != generate_key(eid, &retval, CYPER_KEY_SIZE / 8))
		{
			cout << "Key generation faild! :(";
			return -1;
		}
		cout << "Key generation Succeeded! :) ";

	}


	// Destroy the enclave when all Enclave calls finished.
	if(SGX_SUCCESS != sgx_destroy_enclave(eid))
	{
		return -1;
	}
	return 0;
}