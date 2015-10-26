/*
* Nibble Knowlege
* ------------------------------------------------------
* FileName: fileIO.c
* Description: Contains file IO functions
*
* Original Creator:  Colton Schmidt
* Date of Creation:  06/10/15
* Last Editor:       Colton Schmidt
* Date of Last Edit: 06/10/15
*/

//Includes
#include "ram.h"


int readBin(const char* fileName, uint16_t memLocation){

	unsigned char buffer = 0;
	nibble temp2;
	nibble temp;
	int counter = 0;

	FILE *binFilePtr;

	printf("Reading file...\n");

	//Open the Bin File
	binFilePtr = fopen(fileName, "rb");

	if(!binFilePtr){
		printf("Unable to open .bin file\n");
		return 1;
	}

	while(1){



		//Read a nibble at a time
		fread(&buffer, sizeof(buffer), 1, binFilePtr);


		temp.data = buffer;

		//Check for end of File
		if(feof(binFilePtr))
			break;

		//Store in Mem
		temp2.data = (buffer >> 4);
		writeMem( temp2, memLocation );
		memLocation += 1;
		writeMem(temp, memLocation);
		memLocation++;
		counter++; counter++;
	}

	fclose(binFilePtr);

	printf("Finished reading file\n");
	return counter/5;

}
