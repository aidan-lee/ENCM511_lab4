/*************************************************************************************
* AUTO-GENERATED COMMENT - DO NOT MODIFY 
* Author: nargiz.rzayeva 
* Date: Fri 2017/12/08 at 10:18:57 AM 
* File Type: TTCOS Main File
*************************************************************************************/

// This is an example TT_COS main( ) function with threads.
// Use this as a template for your own TT_COS main( ) for the various Threads

#include "Lab4uTTCOS_uTTCOSg2017_main.h"
#include "SPI_Message_OS.h"
#include "stdio.h"


//#warning  "This timing MACRO may need adjusting	to get one second flashes on your board"
#undef  EXECUTE_EVERY_SECOND
#define EXECUTE_EVERY_SECOND 		(5 * uTTCOSg_GetTickFrequency( )) // ADJUST ME


#include <stdio.h>

enum {USE_TIMER_OS = 1, USE_AUDIO_OS = 2, DEFAULT_OS = 2, INIT_BOTH_OS_USE_AUDIO_OS = 3};
extern int whichOS;

// TODO "Add C++ prototypes for each new thread you add to the system"
#if defined(__ADSPBF533__)
//#warning  "Read class notes on why this prototype does not have the correct format"
void BlackfinBF533_uTTCOSg_Audio_Rx_Tx_Task(void); 

#elif defined(__ADSP21469__)
extern "C" void SHARC21469_uTTCOSg_Audio_Rx_Tx_Task(void);
#else
	#error "Unknown processor in Audio thread"
#endif	

#warning "Fix these TODO's, then delete warning and TODO lines	
	// TODO  The call to uTTCOSg_AudioTalkThrough will cause a link error because of a missing file
	// TODO  You will need to add the file "BlackfinBF533_uTTCOSg_AudioPremptive_Task.cpp" 
	// TODO      to the project src directory. This file can be found in 
	// TODO            "C:\__CCES\AudioCapture_Files\"
	
	// TODO  The calls to uTTCOSg and GPIO functions will cause link errors until you add their libraries into the project
	// TODO       To add these libraries -- Right click on project name, select Properties, Setting, Library
	// TODO           Select "Add Library" then "Workspace"
	// TODO           "File libraries can be found at "C:\__CCES\Libraries\"

void initSPI()
{

	*pSPI_BAUD = 0x8000;
	*pSPI_FLG |= 0x0020;
	//*pSPI_TDBR = 0x0000;
	*pSPI_CTL = 0x5101;
	//*pSPI_STAT = 0x0000;
	ssync();
}

void charToInt(char* message, short int *hexMessage)
{
	int i = 0;
	while(message[i] != '\0')
	{

		hexMessage[i] = 0x0500 | (unsigned short int) message[i];
		i++;
	}
}

volatile bool initializiationFinished = false;
volatile bool message1Finished = false;
volatile bool message2Finished = false;


short int hexMessage1[255];
short int hexMessage2[255];

//char message1[] = "Hello World!";
//char message2[] = "Goodbye World!";
char message1[] ="What is the                             meaning of life?";
char message2[] = "Psst, it's 42.";

volatile bool ready = false;
volatile bool clearFinished = false;

unsigned short int initializeLCD[] = {0x0101, 0x0130, 0x0130, 0x013C, 0x010F, 0x0101};

void main(void) {
	// Make maxNumberThreads at least 5 larger than the 
	//            number of threads you plan to add
	int numBackgroundThreads = 5;
	int numberYourThreads = 4;
	int maxNumberThreads = numBackgroundThreads + numberYourThreads;  

#if defined(__ADSPBF533__)
#if 1
	uTTCOSg_OS_Init(USE_AUDIO_INTERRUPT, 0, 44000);
//	uTTCOSg_AddPreEmptiveThread(BlackfinBF533_uTTCOSg_Audio_Rx_Tx_Task, NO_DELAY, EVERY_TICK);
#else
	uTTCOSg_OS_Init(USE_TIMER_INTERRUPT, 1000, 0);
#endif
#elif defined(__ADSP21469__)
	uTTCOSg_OS_Init(USE_AUDIO_INTERRUPT, 0, 44000);	// This changes LED pattern to 0xF8 for some reason
//	uTTCOSg_AddPreEmptiveThread(SHARC21469_uTTCOSg_Audio_Rx_Tx_Task, NO_DELAY, EVERY_TICK);
#else
	#error "Unknown processor in OS_Init"
#endif	

#warning "You will need to activate the next linme to get the LED interface to workr//  Init_LED_GPIOInterface();

//  TODO Add calls to activate I/O Threads to flash LEDs.  
//  TODO Test the calls one at a time to check they work indiviudually -- then run all together to do an acceptance test
//	uTTCOSg_AddThread(Faulty_Flash_LED1_If_Then_Else, NO_DELAY, EXECUTE_EVERY_SECOND);
//	uTTCOSg_AddThread(Faulty_Flash_LED3_If_Then_Else, NO_DELAY, EXECUTE_EVERY_SECOND);
//	uTTCOSg_AddThread(Faulty_Flash_LED5_If_Then_Else, NO_DELAY, EXECUTE_EVERY_SECOND);

//  TODO When Flash1, 3 and 5 work -- refactor the code to have STATE machine format
//	uTTCOSg_AddThread(Flash_LED1_StateMachine, NO_DELAY, EXECUTE_EVERY_SECOND);
//	uTTCOSg_AddThread(Flash_LED3_StateMachine, NO_DELAY, EXECUTE_EVERY_SECOND / 4);
//	uTTCOSg_AddThread(Flash_LED5_StateMachine, NO_DELAY, EXECUTE_EVERY_SECOND);


	charToInt(message1, hexMessage1);
	charToInt(message2, hexMessage2);

	uTTCOSg_Start_Scheduler(maxNumberThreads);   //  Start the scheduler timer
	initSPI();
				// Execution time of TT_COS_Dispatch( ) and TT_COS_Update( ) improved by specifiying maxNumberTasks
	while (1) {


		unsigned char initThread = uTTCOSg_AddThread(SPI_initialize_Thread, NO_DELAY, EXECUTE_EVERY_SECOND);
		while(!ready)
		{
			//printf("calling thread\n");
			// Wait, in low power mode, for an interrupt
			// The interrupt service routine calls TTCOS_Update( )
			uTTCOSg_GoToSleep( );

			// Run all the threads in the system according
			// to whether their delays have expired
			uTTCOSg_DispatchThreads( );
		}
		//printf("help\n");
		uTTCOSg_DeleteThread(initThread);

		unsigned char message1Thread = uTTCOSg_AddThread(SPI_Message1_Thread, NO_DELAY, EXECUTE_EVERY_SECOND / 10);
		while(!message1Finished)
		{
			// Wait, in low power mode, for an interrupt
			// The interrupt service routine calls TTCOS_Update( )
			uTTCOSg_GoToSleep( );

			// Run all the threads in the system according
			// to whether their delays have expired
			uTTCOSg_DispatchThreads( );
		}
		uTTCOSg_DeleteThread(message1Thread);

		unsigned char clearThread =uTTCOSg_AddThread(SPI_Clear_Thread, NO_DELAY, EXECUTE_EVERY_SECOND / 10);
		while(!clearFinished)
		{
			// Wait, in low power mode, for an interrupt
			// The interrupt service routine calls TTCOS_Update( )
			uTTCOSg_GoToSleep( );

			// Run all the threads in the system according
			// to whether their delays have expired
			uTTCOSg_DispatchThreads( );
		}
		uTTCOSg_DeleteThread(clearThread);

		unsigned char message2Thread = uTTCOSg_AddThread(SPI_Message2_Thread, NO_DELAY, EXECUTE_EVERY_SECOND / 10);
		while(!message2Finished)
		{
			// Wait, in low power mode, for an interrupt
			// The interrupt service routine calls TTCOS_Update( )
			uTTCOSg_GoToSleep( );

			// Run all the threads in the system according
			// to whether their delays have expired
			uTTCOSg_DispatchThreads( );
		}
		uTTCOSg_DeleteThread(message2Thread);



	}


}

#warning "Remember to fix these TODO's, then delete warning and TODO lines	
