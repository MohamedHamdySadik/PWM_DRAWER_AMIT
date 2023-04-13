/*
 * traning1.c
 *
 * Created: 3/4/2023 7:37:07 PM
 * Author : luka
 */ 
#define F_CPU 1000000UL			/* Define CPU Frequency e.g. here 8MHz */
#include <util/delay.h>			/* Include Delay header file */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std.h"
#include "ICU.h"
#include "KS0108.h"
#include "font5x8.h"

void pulse_drawer(unsigned char duty);
void Clear_pulse(void);


uint16 old_duty=0;
uint16 g_edgeCount = 0;
uint16 g_timeHigh = 0;//T1
uint16 g_timePeriod = 0;//T2
uint16 g_timePeriodPlusHigh = 0;//T3
 int main(void)
 {
			 uint32 dutyCycle = 0;
			/* Create configuration structure for ICU driver */
			Icu_ConfigType Icu_Config = {F_CPU_CLOCK,RISING};

			/* Enable Global Interrupt I-Bit */
			SREG |= (1<<7);

			/* Initialize both the LCD and ICU driver */
			GLCD_Initalize();
			GLCD_ClearScreen();
			Icu_init(&Icu_Config);
			GLCD_GoTo(0,0);
			GLCD_WriteString("Duty=");
			GLCD_GoTo(55,0);
			GLCD_WriteString("Freq=");
			while(1)
			{
				if(g_edgeCount >= 4)
				{
					g_edgeCount = 0;
					/* calculate the dutyCycle */
					dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
					//			dutyCycle = ((float)g_timeHigh/g_timePeriod) * 100;
					/* display the dutyCycle on LCD screen */
					GLCD_GoTo(30,0);
					GLCD_into_string(dutyCycle);
					GLCD_WriteString("% ");
					GLCD_GoTo(75,0);
					switch (Icu_Config.clock)
					{
					case 0:
					break;
					case 1:
					GLCD_into_string(F_CPU/(g_timePeriod));
					break;
					case 2:
					GLCD_into_string(F_CPU/(g_timePeriod*8));
					break;
					case 3:
					GLCD_into_string(F_CPU/(g_timePeriod*64));
					break;
					case 4:
					GLCD_into_string(F_CPU/(g_timePeriod*256));
					break;
					case 5:
					GLCD_into_string(F_CPU/(g_timePeriod*1024));
					break;
					
					}
					GLCD_WriteString("Hz");
					if (dutyCycle != old_duty)
						{
							pulse_drawer(dutyCycle);
						}
					//_delay_ms(1000);
					SREG |= (1<<7);

				}
			}
 }
 
 

void APP_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = Icu_getInputCaptureValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = Icu_getInputCaptureValue();
		/* Clear the timer counter register to start measurements again */
		Icu_clearTimerValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
		SREG &= ~(1<<7); /* disable interrupts */

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

