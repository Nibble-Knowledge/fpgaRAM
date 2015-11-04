
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
	puts("Usage:\n ~pm lower upper : prints memor from lower to upper");
	puts("~rm : resets memory");
	puts("~in : import file into memory");
	puts("~rm : writes current MEM to the RAM chip");
	puts("~rr : reads current RAM into MEM");
	puts("~q : quit");
	
	//setup
	setup_io();
	initGPIOs();
	initMem();

	//UNCOMMENT FOR PI AS RAM
	//pthread_t tid;
	//pthread_create(&tid, NULL, memThread, NULL);



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
		else if(!strcmp(op_code, "~wr")){
			writeRAMChip();
		}
		else if(!strcmp(op_code, "~rr")){
			readRAMChip();
		}


	}

}


//Used for pi as RAM
void* memThread(void* input){

	while(1){
		if(GET_GPIO(WE)){
			readDataFromMem(readAddress());
		}
		else
			writeDataToMem(readAddress());
	}


}


void readRAMChip(void){
	freeMem();
	initMem();
	
	printf("WARNING: Memory being overwritten!\n\nReading RAM chip...");
	
	//Set address lines and WE to output
	OUT_GPIO(WE);
	OUT_GPIO(A0);
	OUT_GPIO(A1);
	OUT_GPIO(A2);
	OUT_GPIO(A3);
	OUT_GPIO(A4);
	OUT_GPIO(A5);
	OUT_GPIO(A6);
	OUT_GPIO(A7);
	OUT_GPIO(A8);
	OUT_GPIO(A9);
	OUT_GPIO(A10);
	OUT_GPIO(A11);
	OUT_GPIO(A12);
	OUT_GPIO(A13);
	OUT_GPIO(A14);
	OUT_GPIO(A15);
	
	//Set data lines to in
	INP_GPIO(D0);
	INP_GPIO(D1);
	INP_GPIO(D2);
	INP_GPIO(D3);
	
	//SET TO READ
	GPIO_SET = 1 << WE;
	
	nibble temp;
	
	for(int  i = 0; i < 65536; i++){
		if((i % 1000) == 0)
			printf(".");
		
		setAddressLines(i);
		
		sleep(0.1);
		
		
		//Read Data and put in MEM
		if(GET_GPIO(D0))
			temp.data |= 0x1;
		else
			temp.data &= 0xFFFE;
		if(GET_GPIO(D1))
			temp.data |= 0x2;
		else
			temp.data &= 0xFFFD;
		if(GET_GPIO(D2))
			temp.data |= 0x4;
		else
			temp.data &= 0xFFFB;
		if(GET_GPIO(D3))
			temp.data |= 0x8;
		else
			temp.data &= 0xFFF7;
			
		MAINMEM[i] = temp;
		
		sleep(0.1);
			
	}
	
	puts("\nFinished reading ram chip into MEM");
	
	
}


void writeRAMChip(void){
	printf("Writing current MEM to RAM...");
	
	//Set address lines and WE to output
	OUT_GPIO(WE);
	OUT_GPIO(A0);
	OUT_GPIO(A1);
	OUT_GPIO(A2);
	OUT_GPIO(A3);
	OUT_GPIO(A4);
	OUT_GPIO(A5);
	OUT_GPIO(A6);
	OUT_GPIO(A7);
	OUT_GPIO(A8);
	OUT_GPIO(A9);
	OUT_GPIO(A10);
	OUT_GPIO(A11);
	OUT_GPIO(A12);
	OUT_GPIO(A13);
	OUT_GPIO(A14);
	OUT_GPIO(A15);
	OUT_GPIO(D0);
	OUT_GPIO(D1);
	OUT_GPIO(D2);
	OUT_GPIO(D3);
	
	nibble temp;
	
	for(int i = 0; i < 65536; i++){
		if((i % 1000) == 0)
			printf(".");
		
		//Set ouputs
		setAddressLines(i);
		temp = MAINMEM[i];
		setDataLines(temp);
		sleep(0.1);
		
		//Write output
		GPIO_CLR = 1 << WE;
		sleep(0.1);
		GPIO_SET = 1 << WE;
		
	}
	
	puts("\nFinished writing MEM to RAM");
	
}


void setDataLines(nibble data){
	
	if(data.data & 0x8)
		GPIO_SET = 1 << D3;
	else
		GPIO_CLR = 1 << D3;
		
	if(data.data & 0x4)
		GPIO_SET = 1 << D2;
	else
		GPIO_CLR = 1 << D2;
		
	if(data.data & 0x2)
		GPIO_SET = 1 << D1;
	else
		GPIO_CLR = 1 << D1;
		
	if(data.data & 0x1)
		GPIO_SET = 1 << D0;
	else
		GPIO_CLR = 1 << D0;
}


void setAddressLines(int i){
	
	//Write address to pins
	if(i & 0x1){
		GPIO_SET = 1 << A0;
	}
	else{
		GPIO_CLR = 1 << A0;
	}
	if(i & 0x2){
		GPIO_SET = 1 << A1;	
	}
	else{
		GPIO_CLR = 1 << A1;
	}
	if(i & 0x4){
		GPIO_SET = 1 << A2;	
	}
	else{
		GPIO_CLR = 1 << A2;
	}
	if(i & 0x8){
		GPIO_SET = 1 << A3;	
	}
	else{
		GPIO_CLR = 1 << A3;
	}
	if(i & 0x10){
		GPIO_SET = 1 << A4;	
	}
	else{
		GPIO_CLR = 1 << A4;
	}
	if(i & 0x20){
		GPIO_SET = 1 << A5;	
	}
	else{
		GPIO_CLR = 1 << A5;
	}
	if(i & 0x40){
		GPIO_SET = 1 << A6;	
	}
	else{
		GPIO_CLR = 1 << A6;
	}
	if(i & 0x80){
		GPIO_SET = 1 << A7;	
	}
	else{
		GPIO_CLR = 1 << A7;
	}
	if(i & 0x100){
		GPIO_SET = 1 << A8;	
	}
	else{
		GPIO_CLR = 1 << A8;
	}
	if(i & 0x200){
		GPIO_SET = 1 << A9;	
	}
	else{
		GPIO_CLR = 1 << A9;
	}
	if(i & 0x400){
		GPIO_SET = 1 << A10;	
	}
	else{
		GPIO_CLR = 1 << A10;
	}
	if(i & 0x800){
		GPIO_SET = 1 << A11;	
	}
	else{
		GPIO_CLR = 1 << A11;
	}
	if(i & 0x1000){
		GPIO_SET = 1 << A12;	
	}
	else{
		GPIO_CLR = 1 << A12;
	}
	if(i & 0x2000){
		GPIO_SET = 1 << A13;	
	}
	else{
		GPIO_CLR = 1 << A13;
	}
	if(i & 0x4000){
		GPIO_SET = 1 << A14;	
	}
	else{
		GPIO_CLR = 1 << A14;
	}
	if(i & 0x8000){
		GPIO_SET = 1 << A15;	
	}
	else{
		GPIO_CLR = 1 << A15;
	}
}


