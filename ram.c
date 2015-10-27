
/*
* Simple RAM for CPU FPGA
* Created by: Bailey Tye
*
*/


#include "ram.h"




int main(void){


	char op_code[100];
	uint16_t instAddr;
	uint16_t topAddr;
	uint16_t baseAddr;
	int run = 1;
	char fileName[100];
	char in[100];

	puts("Initializing memory");
	setup_io();
	initGPIOs();
	initMem();

	pthread_t tid;

	pthread_create(&tid, NULL, memThread, NULL);



	while(run){

		printf("Input: ");
		fgets(in, 99, stdin);
		sscanf(in, "%s %hu %hu", op_code, &instAddr, &topAddr);

		//Process input
		if(!strcmp(op_code, "~q")){
			printf("Shuting down memory...\n");
			run = 0;
		}
		else if(!strcmp(op_code, "~pm")){
			printMem(instAddr, topAddr);
			printf("Current Address: %hu\n", readAddress());
		}
		else if(!strcmp(op_code, "~rm")){
			freeMem();
			initMem();
		}
		else if(!strcmp(op_code, "~in")){
			printf("Enter file name, followed by an address to load at: ");
        	        scanf("%s %hu", fileName, &baseAddr);
			puts("WARNING, make sure the base address is set correctly when assembling file");
			while(getchar()!= '\n');
			if(readBin(fileName, baseAddr) == -1){
                        	puts("Could not open file");
			}
		}


	}

}



void* memThread(void* input){

	while(1){
		if(!GET_GPIO(WE)){
			writeDataToMem(readAddress());
		}
		else
			readDataFromMem(readAddress());
	}


}

