#include "ram.h"

/* From http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format */

char *tobitstr(size_t size, uint64_t data, char dir)
{
	uint64_t i, j;
	char *str = NULL;

	str = calloc(size, 1);
	if(str == NULL)
	{
		return NULL;
	}
	if(dir == LITTLEEND)
	{
		for(i = (pow(2,size - 1)), j = 0; i > 0; i >>= 1, j++)
		{
			if(j % 4 == 0 && j != 0)
			{
				strcat(str, " ");
			}
			strcat(str, ((data & i) == i) ? "1" : "0");
		}
	}
	else if(dir == BIGEND)
	{
		for(i = 1, j = 0; (i < (pow(2,size)) - 1 && i != 0); i <<= 1, j++)
		{
			if(j % 4 == 0 && j != 0)
			{
				strcat(str, " ");
			}
			strcat(str, ((data & i) == i) ? "1" : "0");
		}
	}
	return str;	
}

uint16_t toaddr(char *str)
{
	return (uint16_t) strtol(str, NULL, 0);
}

uint8_t todata(char *str)
{
	return (uint8_t) (strtol(str, NULL, 0) % 16);
}

/*
unsigned int toreg(char *str)
{
	if(!strncmp(str, "a", INSTLEN - 1))
	{
		return A;
	}
	else if(!strncmp(str, "mem.lo", INSTLEN - 1))
	{
		return M1;
	}
	else if(!strncmp(str, "mem.midlo", INSTLEN - 1))
	{
		return M2;
	}
	else if(!strncmp(str, "mem.midhi", INSTLEN - 1))
	{
		return M3;
	}
	else if(!strncmp(str, "mem.hi", INSTLEN - 1))
	{

		return M4;
	}
	return 0;
}
*/
