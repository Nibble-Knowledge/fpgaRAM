#include "ram.h"


int  mem_fd;
void *gpio_map;
// I/O access
volatile unsigned *gpio;


void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
   {
   	printf("can't open /dev/mem \n");
        exit(-1);
   }
   /* mmap GPIO */
   gpio_map = mmap(
   	NULL,             //Any adddress in our space will do
        BLOCK_SIZE,       //Map length
        PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
        MAP_SHARED,       //Shared with other processes
        mem_fd,           //File to map
        GPIO_BASE         //Offset to GPIO peripheral
   );
   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
	printf("mmap error %d\n", (int)gpio_map);//errno also set!
   	exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;

}


void initGPIOs(void){
	INP_GPIO(WE);
	INP_GPIO(A0);
	INP_GPIO(A1);
	INP_GPIO(A2);
	INP_GPIO(A3);
	INP_GPIO(A4);
	INP_GPIO(A5);
	INP_GPIO(A6);
	INP_GPIO(A7);
	INP_GPIO(A8);
	INP_GPIO(A9);
	INP_GPIO(A10);
	INP_GPIO(A11);
	INP_GPIO(A12);
	INP_GPIO(A13);
	INP_GPIO(A14);
	INP_GPIO(A15);
	INP_GPIO(D0);
	INP_GPIO(D1);
	INP_GPIO(D2);
	INP_GPIO(D3);
	
}

int readWE(void){
	if(GET_GPIO(WE))
		return 1;
	else
		return 0;
}

unsigned int readAddress(void){
	unsigned int address = 0;
	if(GET_GPIO(A0))
		address |= 0x1;
	else
		address &= 0xFFFE;
	if(GET_GPIO(A1))
		address |= 0x2;
	else
		address &= 0xFFFD;
	if(GET_GPIO(A2))
		address |= 0x4;
	else
		address &= 0xFFFB;
	if(GET_GPIO(A3))
		address |= 0x8;
	else
		address &= 0xFFF7;
	if(GET_GPIO(A4))
		address |= 0x10;
	else
		address &= 0xFFEF;
	if(GET_GPIO(A5))
		address |= 0x20;
	else
		address &= 0xFFDF;
	if(GET_GPIO(A6))
		address |= 0x40;
	else
		address &= 0xFFBF;
	if(GET_GPIO(A7))
		address |= 0x80;
	else
		address &= 0xFF7F;
	if(GET_GPIO(A8))
		address |= 0x100;
	else
		address &= 0xFEFF;
	if(GET_GPIO(A9))
		address |= 0x200;
	else
		address &= 0xFDFF;
	if(GET_GPIO(A10))
		address |= 0x400;
	else
		address &= 0xFBFF;
	if(GET_GPIO(A11))
		address |= 0x800;
	else
		address &= 0xF7FF;
	if(GET_GPIO(A12))
		address |= 0x1000;
	else
		address &= 0xEFFF;
	if(GET_GPIO(A13))
		address |= 0x2000;
	else
		address &= 0xDFFF;		
	if(GET_GPIO(A14))
		address |= 0x4000;
	else
		address &= 0xBFFF;
	if(GET_GPIO(A15))
		address |= 0x8000;
	else
		address &= 0x7FFF;
	
	return address;
		
}

void writeDataToMem(uint16_t address){
	nibble data;
	INP_GPIO(D0);
	INP_GPIO(D1);
	INP_GPIO(D2);
	INP_GPIO(D3);
	
	if(GET_GPIO(D0))
		data.data |= 0x1;
	else
		data.data &= 0xE;
	if(GET_GPIO(D1))
		data.data |= 0x2;
	else
		data.data &= 0xD;		
	if(GET_GPIO(D2))
		data.data |= 0x4;
	else
		data.data &= 0xB;
	if(GET_GPIO(D3))
		data.data |= 0x8;
	else
		data.data &= 0x7;
	
	writeMem(data, address);
}


void readDataFromMem(uint16_t address){
	nibble data = readMem(uint16_t)
	OUT_GPIO(D0);
	OUT_GPIO(D1);
	OUT_GPIO(D2);
	OUT_GPIO(D3);
	
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

