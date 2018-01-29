/*
 * SPI_Message_OS.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: nargiz.rzayeva
 */

#include "blackfin.h"
#include "SPI_Message_OS.h"
#include "stdio.h"

static volatile unsigned SPI_tokenSent = 0;
static volatile bool SPI_interfaceInUse = true;



extern volatile bool ready;
extern volatile bool message1Finished;
extern volatile bool message2Finished;
extern volatile bool clearFinished;

volatile int message1Counter = 0;
volatile int message2Counter = 0;
volatile int initCounter = 0;
volatile int toggleCounter = 0;

extern unsigned short int initializeLCD[];

extern short int hexMessage1[255];
extern short int hexMessage2[255];

void SPI_initialize_Thread()
{
	if(!((*pSPI_STAT & 0x0008) == 0x0008))
	{
		if(toggleCounter == 0)
		{
			*pSPI_TDBR = initializeLCD[initCounter];
			printf("Case 1\n");
			toggleCounter++;
		}
		else if(toggleCounter == 1)
		{
			printf("Case 2\n");
			*pSPI_TDBR = initializeLCD[initCounter] & 0xFEFF;
			toggleCounter++;
		}
		else if(toggleCounter == 2)
		{
			*pSPI_TDBR = initializeLCD[initCounter];
			if(initCounter == 5)
			{
				printf("Initialized\n");
				initCounter = 0;
				ready = true;
			}
			else
			{
				initCounter++;
			}
			toggleCounter = 0;
		}
		else
		{
			printf("Do nothing!\n");
		}
	}
}


void SPI_Message1_Thread()
{
	if(ready && ((*pSPI_STAT & 0x0008) != 0x0008))
	{
		switch(toggleCounter){
		case 0:
			toggleCounter++;
			*pSPI_TDBR = hexMessage1[message1Counter];
		break;
		case 1:
			toggleCounter++;
			*pSPI_TDBR = hexMessage1[message1Counter] & 0xFEFF;
		break;
		case 2:
			toggleCounter = 0;
			*pSPI_TDBR = hexMessage1[message1Counter];
			message1Counter++;
			if(hexMessage1[message1Counter] == '\0')
			{
				message1Counter = 0;
				message1Finished = true;
			}
		break;
		}
	}

}

void SPI_Message2_Thread()
{
	if(ready && ((*pSPI_STAT & 0x0008) != 0x0008))
	{
		switch(toggleCounter){
		case 0:
			toggleCounter++;
			*pSPI_TDBR = hexMessage2[message2Counter];
		break;
		case 1:
			toggleCounter++;
			*pSPI_TDBR = hexMessage2[message2Counter] & 0xFEFF;
		break;
		case 2:
			toggleCounter = 0;
			*pSPI_TDBR = hexMessage2[message2Counter];
			message2Counter++;
			if(hexMessage2[message2Counter] == '\0')
			{
				message2Counter = 0;
				message2Finished = true;
			}
		break;
		}
	}
}

void SPI_Clear_Thread()
{
	if(!((*pSPI_STAT & 0x0008) == 0x0008))
	{
		//printf("help\n");
		//printf("toggleCounter = %d\n", toggleCounter);

		if(toggleCounter == 0)
		{
			*pSPI_TDBR = initializeLCD[5];
			printf("Case 1\n");
			toggleCounter++;
		}
		else if(toggleCounter == 1)
		{
			printf("Case 2\n");
			*pSPI_TDBR = initializeLCD[5] & 0xFEFF;
			toggleCounter++;
		}
		else if(toggleCounter == 2)
		{
			*pSPI_TDBR = initializeLCD[5];
			if(initCounter == 5)
			{
				printf("Clear finished\n");
				initCounter = 0;
				clearFinished = true;
			}
			else
			{
				initCounter++;
			}
			toggleCounter = 0;
		}
		else
		{
			printf("Do nothing!\n");
		}
	}
}



