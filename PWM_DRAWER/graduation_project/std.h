/*
 * std.h
 *
 * Created: 3/4/2023 7:37:55 PM
 *  Author: luka
 */ 


#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <stdlib.h>
#define setBit(reg,bit) reg|=(1<<bit)
#define clearBit(reg,bit) reg&=(~(1<<bit))
#define toggleBit(reg,bit) reg^=(1<<bit)
#define readBit(reg,bit) (1&(reg>>bit))
typedef unsigned char uint8;
typedef char Int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

typedef enum bin{
	false=0,
	true=1
}bool;



#endif /* STD_H_ */