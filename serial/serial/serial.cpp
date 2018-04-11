// serial.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerialClass.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <bitset>
#include <iostream>
#include <string.h>
// Convert a char to a binary string representation of that char value
#define DIGITS					8
#define BASE					2
#define KEY_LENGTH				32
#define KEY_LENGTH_IN_BYTES		(KEY_LENGTH/8)

/*function: itob

function returns an array (size 8)  of chars of the inputs binary value
MSB will be located in index 0
*/
char* itob(int number)
{
	int bits_index = DIGITS - 1;
	char binary_string[DIGITS] = { '0' };

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





// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	int number;
	unsigned char	keys[KEY_LENGTH_IN_BYTES + 1];
	char			binary_keys[KEY_LENGTH_IN_BYTES][DIGITS + 1] = { {'\0'} };
	//string test = "ABCD";
    //const char * testCharArray = test.c_str();

	strcpy((char *)keys, "abcd");
    /*for (int i = 0; keys[i] != '\0'; i++) {
        cout << (int)keys[i] << " ";
    }
    cout << endl;
	*/
	//keys[3] = (char)255;
	//keys[4] = '\0';

	printf("keys: %s\n", keys);
	for (int i = 0; keys[i] != '\0';  i++)
	{
		printf("keys[%d]: , %d, %b\n", i, keys[i], keys[i]);
		strncpy(binary_keys[i], itob((int)keys[i]), DIGITS);
	}

	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("COM9:");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	char outcomingData[2] = "";			// don't forget to pre-allocate memory
											//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;
	int write_result = 0;
	char write_data[2] = { '\0' };

//	while (SP->IsConnected())
//	{
//		readResult = SP->ReadData(incomingData, dataLength);
//		printf("Bytes read: (0 means no data available) %i\n",readResult);
//		incomingData[readResult] = 0;
		
		//start new key - transmit 0xA5 to indicate start of new key transmition
		char start_new_key_str[DIGITS + 1] = { '\0' };
		strncpy(start_new_key_str, itob(0xA5), DIGITS);
		printf("\n******** transmit start new key ******** \n 0xA5\n");
		for (int j = 0; j < DIGITS; j++) //transmit MSB first
		{
			write_data[0] = start_new_key_str[j];
			write_result = SP->WriteData(write_data, 1); //TODO: add error indication
			printf("%s ", write_data);

			Sleep(1000);
		}

		//key transmition
		for (int i = 0; i < KEY_LENGTH_IN_BYTES; i++)
		{
			printf("\n******** transmit byte number %d ******** \n char: %c\t int:%d\n", i, keys[i], keys[i]);
			for (int j = 0; j < DIGITS; j++) //transmit MSB first
			{
				write_data[0] = binary_keys[i][j];
				write_result = SP->WriteData(write_data, 1); //TODO: add error indication
				printf("%s ", write_data);

				Sleep(1000);
			}

		}
	

		
	//}
	return 0;
}

