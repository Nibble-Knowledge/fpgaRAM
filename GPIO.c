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
	INP_GPIO(CLKPIN);
	OUT_GPIO(CLKPIN);
	INP_GPIO(P0);
	OUT_GPIO(P0);
	INP_GPIO(P1);
	OUT_GPIO(P1);
	INP_GPIO(P2);
	OUT_GPIO(P2);
	INP_GPIO(P3);
	OUT_GPIO(P3);
	INP_GPIO(P4);
	OUT_GPIO(P4);
	INP_GPIO(P5);
	OUT_GPIO(P5);
	INP_GPIO(P6);
	OUT_GPIO(P6);
	INP_GPIO(P7);
	OUT_GPIO(P7);
	INP_GPIO(P8);
	OUT_GPIO(P8);
	INP_GPIO(P9);
	OUT_GPIO(P9);
	INP_GPIO(P10);
	OUT_GPIO(P10);
	INP_GPIO(P11);
	OUT_GPIO(P11);

	//Turn them all off
	GPIO_CLR = 1 << CLKPIN;
	GPIO_CLR = 1 << P0;
	GPIO_CLR = 1 << P1;
	GPIO_CLR = 1 << P2;
	GPIO_CLR = 1 << P3;
	GPIO_CLR = 1 << P4;
	GPIO_CLR = 1 << P5;
	GPIO_CLR = 1 << P6;
	GPIO_CLR = 1 << P7;
	GPIO_CLR = 1 << P8;
	GPIO_CLR = 1 << P9;
	GPIO_CLR = 1 << P10;
	GPIO_CLR = 1 << P11;
}


