/*
 * GLCD.c
 *
 * Created: 4/6/2023 12:47:49 PM
 * Author : luka
 */ 


#define F_CPU 1000000UL			/* Define CPU clock Freq 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include "KS0108.h"
#include "font5x8.h"

void pulse_drawer(unsigned char duty);
void Clear_pulse(void);

unsigned char old_duty=0;

int main(void)
{
		unsigned char duty=0;
		
		GLCD_Initalize();
		GLCD_ClearScreen();
		GLCD_GoTo(0,0);
		GLCD_WriteString("Duty=");
		
		
		GLCD_GoTo(64,0);
		GLCD_WriteString("Freq=");



	while(1)
	{
		
		for(;duty<=100;duty+=10)
		{
			GLCD_GoTo(30,0);
			GLCD_into_string(duty);
			GLCD_WriteString("%");
			if (duty != old_duty)
			{
				pulse_drawer(duty);

			}	
		}
		
		
		
					
		
	
		
	}
}

void pulse_drawer(unsigned char duty)
{
	old_duty=duty;
	Clear_pulse();
	unsigned char x=0;
	unsigned char y=0;
	if (duty>0 && duty< 100 )
	{
		
		 y=50;
		unsigned char high_pulse=(float)duty/(float)3.125;
		unsigned char peruid =32;
		
		while(x<128)
		{
			
			for(y=y;y>30;y--)
			{
				GLCD_SetPixel(x,y);
			}
		
			for(x=x;x<high_pulse;x++)
			{
				GLCD_SetPixel(x,y);
			}
			high_pulse+=32;
			for(y=y;y<50;y++)
			{
				GLCD_SetPixel(x,y);
			}
			for(x=x;x<peruid;x++)
			{
				GLCD_SetPixel(x,y);
			}
			peruid+=32;
		}
		x=0;
	}
	
	if (duty == 100)
	{
		x=0;
		y=30;
		for(;x<128;x++)
		{
			GLCD_SetPixel(x,y);
		}
	}
	if (duty == 0)
	{
		x=0;
		y=50;
		for(;x<128;x++)
		{
			GLCD_SetPixel(x,y);
		}
	}
	
}
void Clear_pulse(void)
{
	unsigned char x=0;
	for (x=3;x<7;x++)
	{
		GLCD_GoTo(0,x);
		GLCD_WriteString("                       ");
	}
	
}