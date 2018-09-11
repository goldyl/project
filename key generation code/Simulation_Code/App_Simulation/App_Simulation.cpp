#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <bitset>
#include <iostream>
#include <string.h>
#include "SerialClass.h"
#include "sgx_urts.h"
#include "Enclave_Simulation_u.h"

using namespace std;

#define ENCLAVE_FILE _T("Enclave_Simulation.signed.dll")
#define MAX_BUF_LEN 100
#define CYPER_KEY_SIZE	32
#define PATH_TO_SERIAL "..\\serial.exe \""
// Convert a char to a binary string representation of that char value
#define DIGITS					8
#define BASE					2
#define KEY_LENGTH_IN_BYTES		(CYPER_KEY_SIZE/8)
#define PERIOD					1000
#define PORT					"COM6"

/*function: my_itob

function returns an array (size 8)  of chars of the inputs binary value
MSB will be located in index 0
*/
char* my_itob(int number)
{
	int bits_index = DIGITS - 1;
	char* binary_string = (char*)malloc(sizeof(char)*DIGITS);

	for (int i = 0; i < DIGITS; i++)
		binary_string[i] = '0';

	printf("\n%d\t binary:", number);

	while (number > 0) {
		binary_string[bits_index--] = (number & 1) + '0';
		number = (number >> 1);
	}

	for (int i = 0; i < DIGITS; i++)
		printf("%c", binary_string[i]);
	printf("\n");

	return binary_string;
}



int _tmain(int argc, _TCHAR* argv[])
{
	sgx_status_t sgx_stat = SGX_SUCCESS;
	sgx_enclave_id_t eid = 0;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
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

	printf("Please enter any character to close window");
	getchar();

		// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
	{
		return -1;
	}
	return 0;
	}

//untrusted function

/*function: transmit_key

function transmit given key to serial port
this is an untrusted function

*/
void transmit_key(unsigned char* keys, int key_size_in_bytes, int char_size) {
	int number;
	char			binary_keys[KEY_LENGTH_IN_BYTES][DIGITS + 1] = { { '\0' } };

	printf("keys: %s\n", keys);
	for (int i = 0; keys[i] != '\0'; i++)
	{
		printf("keys[%d]", i);
		char* itob_res = my_itob((int)keys[i]);
		strncpy(binary_keys[i], itob_res, DIGITS);
		free(itob_res);
	}

	printf("connecting to port %s\n", PORT);

	Serial* SP = new Serial(PORT);    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	int write_result = 0;
	char write_data[2] = { '\0' };

	if (SP->IsConnected())
	{

		//start new key - transmit 0xA5 to indicate start of new key transmition
		char start_new_key_str[DIGITS + 1] = { '\0' };
		strncpy(start_new_key_str, my_itob(0xA5), DIGITS);

		printf("\n******** transmit start new key ******** \n 0xA5\n");
		for (int j = 0; j < DIGITS; j++) //transmit MSB first
		{
			write_data[0] = start_new_key_str[j];
			write_result = SP->WriteData(write_data, 1);
			printf("%s ", write_data);

			Sleep(PERIOD);
		}

		//key transmition
		for (int i = 0; i < key_size_in_bytes-1; i++)
		{
			printf("\n******** transmit byte number %d ******** \n char: %c\t int:%d\n", i, keys[i], keys[i]);
			for (int j = 0; j < DIGITS; j++) //transmit MSB first
			{
				write_data[0] = binary_keys[i][j];
				write_result = SP->WriteData(write_data, 1); 
				printf("%s ", write_data);

				Sleep(PERIOD);
			}
		}
	}
}
