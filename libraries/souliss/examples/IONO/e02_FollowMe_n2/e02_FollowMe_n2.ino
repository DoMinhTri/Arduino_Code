/**************************************************************************
    Souliss - Hello World
    
    Sample sketch to use all the input and outputs of the IONO Ethernet board
    configure the network IP parameters as per your needs and use SoulissApp
    to control the board.

    Get started in the wiki
    https://github.com/souliss/souliss/wiki
    
***************************************************************************/

// Let the IDE point to the Souliss framework
#include "SoulissFramework.h"

#include "bconf/IonoEthernet.h"             // Define the board type

// Define the network configuration
uint8_t ip_address[4]  = {192, 168, 1, 78};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {192, 168, 1, 1};

// Include framework code and libraries
#include <SPI.h>
#include <EEPROM.h>

/*** All configuration includes should be above this line ***/ 
#include "Souliss.h"

// Each logic is associated to a slot of the memory
#define LIGHT1                  0           
#define LIGHT2                  1           
#define LIGHT3                  2           
#define LIGHT4                  3           
#define LIGHT5                  4   
#define LIGHT6                  5
#define LIGHT_NO                6
#define PWMLOGIC                6
#define PWMOUT                  7   

U8 button_pressed=1;

void setup()
{   
    // Init the board
    InitIono();
    
    // Define logics for the node
    Set_T11(LIGHT1);
    Set_T11(LIGHT2);
    Set_T11(LIGHT3);
    Set_T11(LIGHT4); 
    Set_T11(LIGHT5); 
    Set_T11(LIGHT6); 
    Set_T19(PWMLOGIC);   
    
    // Setup the network configuration
    //
    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);
}

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_510ms() {   // Scan the input values every 510 ms
        
            // Check if a button has been pressed
            button_pressed=1;
        
            if(DigIn(DI1, Souliss_T1n_ToggleCmd, LIGHT1));          // Read inputs from DI1
            else if(DigIn(DI2, Souliss_T1n_ToggleCmd, LIGHT2));     // Read inputs from DI2
            else if(DigIn(DI3, Souliss_T1n_ToggleCmd, LIGHT3));     // Read inputs from DI3
            else if(DigIn(DI4, Souliss_T1n_ToggleCmd, LIGHT4));     // Read inputs from DI4
            else if(DigIn(DI5, Souliss_T1n_ToggleCmd, LIGHT5));     // Read inputs from DI5
            else if(DigIn(DI6, Souliss_T1n_ToggleCmd, LIGHT6));     // Read inputs from DI6
            else button_pressed=0;
        }

        FAST_210ms() {   // Process logics and outputs
 
            Logic_T11(LIGHT1);                                  // Execute the logic for Relay 1
            Logic_T11(LIGHT2);                                  // Execute the logic for Relay 2
            Logic_T11(LIGHT3);                                  // Execute the logic for Relay 3
            Logic_T11(LIGHT4);                                  // Execute the logic for Relay 4
            Logic_T11(LIGHT5);                                  // Execute the logic for Relay 5
            Logic_T11(LIGHT6);                                  // Execute the logic for Relay 6
            
            DigOut(DO1, Souliss_T1n_Coil, LIGHT1);              // Drive the Relay 1
            DigOut(DO2, Souliss_T1n_Coil, LIGHT2);              // Drive the Relay 2
            DigOut(DO3, Souliss_T1n_Coil, LIGHT3);              // Drive the Relay 3
            DigOut(DO4, Souliss_T1n_Coil, LIGHT4);              // Drive the Relay 4
            DigOut(DO5, Souliss_T1n_Coil, LIGHT5);              // Drive the Relay 5
            DigOut(DO6, Souliss_T1n_Coil, LIGHT6);              // Drive the Relay 6
  
            // If a button has been pressed
            if(button_pressed)
            {       
                U8 i, cmd[LIGHT_NO];
                
                // Define the command to align the status of the twin node
                for(i=LIGHT1;i<LIGHT_NO;i++)
                    if(mOutput(i))
                        cmd[i]=Souliss_T1n_OnCmd;
                    else
                        cmd[i]=Souliss_T1n_OffCmd;
                
                // Sync the remote node with the actual status of this node
                SendData(77, LIGHT1, cmd, LIGHT_NO);
            
                // Reset    
                button_pressed=0;                       
            }  
        }

		FAST_10ms()  {

            Logic_T19(PWMLOGIC);                                // Execute the logic for PWM Output
            analogWrite(AO1, mOutput(PWMOUT));			
		}
        
        // Here we process all communication with other nodes
        // and user interface
        FAST_PeerComms();    
    
        FAST_21110ms() {    // Process the timers in about 2 seconds 

            // Timed values are based on number of execution of the timers, as example 
            // if the timer is set at 20 cycle it will need 40 seconds to exipre.
            Timer_T11(LIGHT1);
            Timer_T11(LIGHT2);  
            Timer_T11(LIGHT3);
            Timer_T11(LIGHT4);
            Timer_T11(LIGHT5);
            Timer_T11(LIGHT6);  
            Timer_T19(PWMLOGIC);
        }		
    }
    
    // Scheduler for lower execution rates
    EXECUTESLOW() { 
        UPDATESLOW();

        SLOW_10s() {  // Process the timer every 10 seconds           
  
        }     
    }
} 