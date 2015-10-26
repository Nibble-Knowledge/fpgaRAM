/*
* Nibble Knowlege
* ------------------------------------------------------
* FileName: vm4.h.c
* Description: Contains the definitions and constants used thoughout
*	       the VM
* Original Creator: Bailey Tye
* Last Editor:      Bailey Tye
* Date of Last Edit: 03/10/15
* Date of Creation:  01/10/15
*/

#ifndef _VM4_H_
#define _VM4_H_

#define _POSIX_TIMERS
#define __USE_POSIX199309

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#define VERSTR "VM4 v0.3"
#define BOOTBIN "test.bin"
#define IOMEM 3
#define BOOTMEM 1024
#define RESERVEMEM (1 + IOMEM + BOOTMEM)
#define NOERROR 0
#define MEMALLOCERROR 1
#define UNKNOWNINSTRUCTIONERROR 2
#define UNKOWNERROR -1
#define MEMADDRSIZE 16
#define MEMSIZE 65536
#define BIGEND -1
#define LITTLEEND 1
#define MEMMODSIZE 4

#define PLATEND LITTLEEND

#define NUMREG 4
#define PC 0
#define A 1
#define MEM 2
#define STAT 3

#define HLT 0
#define LOD 1
#define STR 2
#define ADD 3
#define NOP 4
#define NND 5
#define JMP 6
#define CXA 7

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


#define P0 4
#define P1 17
#define P2 27
#define P3 22

#define P4 18
#define P5 23
#define P6 24
#define P7 25

#define P8 12
#define P9 16
#define P10 20
#define P11 21
#define CLKPIN 5


//Main loop modes
#define FILEMODE 1
#define USERMODE 0

// Structure of a nibble

typedef struct _nibble{
	uint8_t data : 4;
} nibble;

// Global Variables

extern uint16_t regPC;
extern nibble   regA;
extern uint16_t regMEM;
extern nibble	regSTAT;

extern nibble*	MAINMEM;

extern int  mem_fd;
extern void *gpio_map;
// I/O access
extern volatile unsigned *gpio;


//--------------------Prototypes----------------------//


// inst.c Prototypes
// ------------------------------------------------//


//Decodes the OP code and Calls the appropriate instruction
int decode(const char* op_code, uint16_t address);

//Halt Instruction
void hlt(void);

//Load Instruction
void lod(uint16_t);

//Store Instruction
void str(uint16_t);

//Add Instruction
void add(uint16_t);

//No Operation Instruction
void nop(void);

//NAND Instruction
void nnd(uint16_t);

//Carry XOR into Accumulator
void cxa(void);

//Jump Instruction
void jmp(uint16_t);

//Prints registers to screen
void printReg(void);

//Sets the IO fields in MEM to the GPIOs
void setIOMem(int mode);

//Inits all GPIOs to outputs
void initGPIOs(void);



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



//main.c Prototypes
// ------------------------------------------------//

//Superloop
int mainloop(void);

//Shuts down the virtual machine
int shutdown(int);


void waitForPeriod(long firstTime,struct timespec gettime_now,long period );

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


#endif
