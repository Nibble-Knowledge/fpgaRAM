/*
* Nibble Knowlege
* ------------------------------------------------------
* FileName: ram.h
* Description: Contains the definitions and constants used thoughout
*	       the RAM
* Original Creator: Bailey Tye
* Last Editor:      Bailey Tye
* Date of Last Edit: 25/10/15
* Date of Creation:  25/10/15
*/

#ifndef _RAM_H_
#define _RAM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <math.h>

#define IOMEM 3
#define BOOTMEM 1024
#define RESERVEMEM (1 + IOMEM + BOOTMEM)
#define MEMADDRSIZE 16
#define MEMSIZE 65536
#define BIGEND -1
#define LITTLEEND 1
#define MEMMODSIZE 4

#define PLATEND LITTLEEND


//Stuff for GPIOs
#define BCM2708_PERI_BASE 0x3F000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock


#define A0 2
#define A1 3
#define A2 4
#define A3 17
#define A4 27
#define A5 22
#define A6 10
#define A7 9
#define A8 11
#define A9 5
#define A10 6
#define A11 13
#define A12 19
#define A13 26
#define A14 21
#define A15 20

#define WE 25

#define D0 14
#define D1 15
#define D2 18
#define D3 23



// Structure of a nibble

typedef struct _nibble{
	uint8_t data : 4;
} nibble;

// Global Variables

//extern static nibble*	MAINMEM;

extern int  mem_fd;
extern void *gpio_map;

// I/O access
extern volatile unsigned *gpio;



// mem.c Prototypes
// ------------------------------------------------//

//Initialize Memory
int initMem(void);


nibble readMem(uint16_t);

//Stores data into memory
void writeMem(nibble data, uint16_t address);

//Prints all of Memory
void printMem(uint16_t, uint16_t);

//Frees Memory allocation
void freeMem(void);

//Sets data table values for 1 to 15
void setBoot(void);



//util.c Prototypes
//-------------------------------------------------//


char *tobitstr(size_t,uint64_t,char);


//fileIO.c Prototypes
//------------------------------------------------//

//Reads file into memory
int readBin(const char*, uint16_t);


//GPIO.c Prototypes
//-----------------------------------------------//

void setup_io(void);

void initGPIOs(void);

int readWE(void);

uint16_t readAddress(void);

void readDataFromMem(uint16_t);

void writeDataToMem(uint16_t);

void* memThread(void*);

#endif
