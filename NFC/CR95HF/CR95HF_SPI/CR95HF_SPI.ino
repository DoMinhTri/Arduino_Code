/*****************************************************************************/
/*  Source code demo for application RFID use board Arduino UNO R3 and       */
/*  board CR95HF Easy.                                                       */
/*  Application: Detect a contacless tag standard ISO 15693.                 */
/*  If catch a 15693 contacless tag, display UID of tag on LCD and read data */
/*  in EEPROM from block 0 to block 2 of tag after display data on LCD.      */  
/*  Author:   AT-COM TELECOMMUNICATIONS AUTOMATION CO., Ltd                  */
/*  Date:     10.09.2015                                                     */
/*  Version:  rev1.0                                                         */
/*  Hardware connection:                                                     */
/*             Arduino UNO R3                      CR95HF Easy               */
/*             IRQin : Pin 9                       RST : pin 3               */
/*             SS    : pin 10                      CS  : pin 4               */
/*             MOSI  : pin 11                      SDI : pin 7               */
/*             MISO  : pin 12                      SDO : pin 6               */
/*             SCK   : pin 13                      SCK : pin 5               */
/*                                                                           */              
/*     LCD RS pin to digital pin  5                                         */
/*     LCD Enable pin to digital pin 6                                      */
/*     LCD D4 pin to digital pin 7                                           */
/*     LCD D5 pin to digital pin A2                                           */
/*     LCD D6 pin to digital pin 3                                           */
/*     LCD D7 pin to digital pin A1                                           */
/*****************************************************************************/  
/* Includes -----------------------------------------------------------------*/
#include <SPI.h>
#include <LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(5, 6, 7, A2, 3, A1);    // LCD(RS,E,D4,D5,D6,D7)

const int SSPin = 10;   // Slave Select pin
const int IRQPin = 9;   // Sends wake-up pulse
byte TXBuffer[40];      // Rransmit buffer
byte RXBuffer[40];      // Receive buffer
byte NFCReady = 0;      // Used to track NFC state
byte Data = 0;          // Keep track of memory we read from   
char Data_Eeprom[12];
int  Display=0;
//******************************************************************************************************
void setup() {
    Serial.begin(9600);
    
    lcd.begin(16, 2);
    pinMode(IRQPin, OUTPUT);
    digitalWrite(IRQPin, HIGH); // Wake up pulse
    pinMode(SSPin, OUTPUT);
    digitalWrite(SSPin, HIGH);
//    Serial.begin(9600);
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    
    delay(10);                      // Send a wake up
    digitalWrite(IRQPin, LOW);      // Pulse to put the 
    delayMicroseconds(100);         // CR95HF Easy into SPI
    digitalWrite(IRQPin, HIGH);     // Mode 
    delay(10);
}
//******************************************************************************************************
/* SetProtocol_Command programs the CR95HF for
ISO/IEC 15693 operation.

This requires three steps.
1. send command
2. poll to see if CR95HF has data
3. read the response

If the correct response is received the serial monitor is used
to display successful programming. 
*/
void SetProtocol_Command()
 {
 byte i = 0;
 
// step 1 send the command
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(0x02);  // Set protocol command
  SPI.transfer(0x02);  // Length of data to follow
  SPI.transfer(0x01);  // Code for ISO/IEC 15693
  SPI.transfer(0x0D);  // Wait for SOF, 10% modulation, append CRC
  digitalWrite(SSPin, HIGH);
  delay(1);
 
// step 2, poll for data ready

  digitalWrite(SSPin, LOW);
  while(RXBuffer[0] != 8)
    {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // Bit 3 is set
    }
  digitalWrite(SSPin, HIGH);
  delay(1);

// step 3, read the data
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x02);   // SPI control byte for read         
  RXBuffer[0] = SPI.transfer(0);  // Response code
  RXBuffer[1] = SPI.transfer(0);  // Length of data
  digitalWrite(SSPin, HIGH);

  if ((RXBuffer[0] == 0) & (RXBuffer[1] == 0))  // Is response code good?
    {
      if(Display==0)
        { 
           lcd.clear();
           lcd.setCursor(5, 0);
           lcd.print("AT-COM");
           lcd.setCursor(1, 1);
           lcd.print("CR95HF handled");
        }
    NFCReady = 1; // NFC is ready
    Display=1;
    }
  else
    {
    NFCReady = 0; // NFC not ready
    Display=0;
    }
}
//******************************************************************************************************
/* Inventory_Command chekcs to see if an RF
tag is in range of CR95HF Easy.

This requires three steps.
1. send command
2. poll to see if CR95HF has data
3. read the response

If the correct response is received the serial monitor is used
to display the the RF tag's universal ID.  
*/
void Inventory_Command()
 {
 byte i = 0,j;
 char data_lcd[2];
// step 1 send the command
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(0x04);  // Send Receive CR95HF command
  SPI.transfer(0x03);  // Length of data that follows is 0
  SPI.transfer(0x26);  // Request Flags byte
  SPI.transfer(0x01);  // Inventory Command for ISO/IEC 15693
  SPI.transfer(0x00);  // Mask length for inventory command
  digitalWrite(SSPin, HIGH);
  delay(1);
 
// step 2, poll for data ready
// data is ready when a read byte
// has bit 3 set (ex:  B'0000 1000')

  digitalWrite(SSPin, LOW);
  while(RXBuffer[0] != 8)
    {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // Bit 3 is set
    }
  digitalWrite(SSPin, HIGH);
  delay(1);
  
// step 3, read the data
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x02);             // SPI control byte for read         
  RXBuffer[0] = SPI.transfer(0);  // Response code
  RXBuffer[1] = SPI.transfer(0);  // Length of data
  for (i=0;i<RXBuffer[1];i++)      
    RXBuffer[i+2]=SPI.transfer(0);  // Data
  digitalWrite(SSPin, HIGH);
  delay(1);

  if (RXBuffer[0] == 128)  // Is response code good?
     {
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("UID:");
       for(i=10;i>=4;i--)
         {
           itoa(RXBuffer[i],data_lcd, 16);
           for(j=0;j<=strlen(data_lcd);j++)
             {
                if(data_lcd[j]>='0'&&data_lcd[j]<='9')
                   data_lcd[j]=data_lcd[j];
                else if(data_lcd[j]>='a'&&data_lcd[j]<='z')
                   data_lcd[j]=data_lcd[j]-32;
             }
           lcd.write(data_lcd);
         }  
       NFCReady = 2;
      }
  else
    {
       NFCReady = 0;
    }
 }
//****************************************************************************************************** 
/* Write Memory writes data to a block of memory.  
This code assumes the RF tag has less than 256 blocks
of memory and that the block size is 4 bytes.  The block 
written to increments with each pass and if it exceeds
20 it starts over. Data is also changed with each write.

This requires three steps.
1. send command
2. poll to see if CR95HF has data
3. read the response
*/
void Write_Memory(byte Memory_Block)
 {
 byte i = 0;
    
  Data = Data +4;
  Memory_Block = Memory_Block +1;
  if (Memory_Block > 20)
    Memory_Block = 0;
  
// step 1 send the command
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);   // SPI control byte to send command to CR95HF
  SPI.transfer(0x04);   // Send Receive CR95HF command
  SPI.transfer(0x07);   // Length of data that follows 
//  SPI.transfer(0x08); // Use this length if more than 256 mem blocks 
  SPI.transfer(0x02);   // Request Flags byte
//  SPI.transfer(0x0A); // Request Flags byte if more than 256 memory blocks
  SPI.transfer(0x21);   // Write Single Block command for ISO/IEC 15693
  SPI.transfer(Memory_Block);  // Memory block address
//  SPI.transfer(0x00);  // MSB of mem. address greater than 256 blocks
  SPI.transfer(Data);    // First byte block of memory block
  SPI.transfer(Data+1);  // Second byte block of memory block
  SPI.transfer(Data+2);  // Third byte block of memory block
  SPI.transfer(Data+3);  // Fourth byte block of memory block
  digitalWrite(SSPin, HIGH);
  delay(1);
 
// step 2, poll for data ready
// data is ready when a read byte
// has bit 3 set (ex:  B'0000 1000')

  digitalWrite(SSPin, LOW);
  while(RXBuffer[0] != 8)
    {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // Bit 3 is set
    }
  digitalWrite(SSPin, HIGH);
  delay(1);

// step 3, read the data
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x02);             // SPI control byte for read         
  RXBuffer[0] = SPI.transfer(0);  // Response code
  RXBuffer[1] = SPI.transfer(0);  // Length of data
  for (i=0;i<RXBuffer[1];i++)      
       RXBuffer[i+2]=SPI.transfer(0);  // Data
  digitalWrite(SSPin, HIGH);
  delay(1);

  if (RXBuffer[0] == 128)  // is response code good?
      {
        NFCReady = 2;
      }
  else
      {
        NFCReady = 0;
      }
 }
//******************************************************************************************************
/* Read Memory reads data from a block of memory.  
This code assumes the RF tag has less than 256 blocks
of memory and that the block size is 4 bytes.  Data
read from is displayed via the serial monitor.

This requires three steps.
1. send command
2. poll to see if CR95HF has data
3. read the response
*/
int Read_Memory(byte Memory_Block, char str_data[4])
{
  byte i=0;
  int j=0;
// step 1 send the command
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x00);  // SPI control byte to send command to CR95HF
  SPI.transfer(0x04);  // Send Receive CR95HF command
  SPI.transfer(0x03);  // Length of data that follows
 // SPI.transfer(0x04); // length of data if mem > 256
  SPI.transfer(0x02);   // Request Flags byte
 // SPI.transfer(0x0A); // Request Flags byte if mem > 256
  SPI.transfer(0x20);   // Read Single Block command for ISO/IEC 15693
  SPI.transfer(Memory_Block); // Memory block address
//  SPI.transfer(0x00); // MSB of memory block address if mem > 256
  digitalWrite(SSPin, HIGH);
  delay(1);
 
// step 2, poll for data ready
// data is ready when a read byte
// has bit 3 set (ex:  B'0000 1000')

  digitalWrite(SSPin, LOW);
  while(RXBuffer[0] != 8)
    {
    RXBuffer[0] = SPI.transfer(0x03);  // Write 3 until
    RXBuffer[0] = RXBuffer[0] & 0x08;  // Bit 3 is set
    }
  digitalWrite(SSPin, HIGH);
  delay(1);
  
// step 3, read the data
  digitalWrite(SSPin, LOW);
  SPI.transfer(0x02);             // SPI control byte for read         
  RXBuffer[0] = SPI.transfer(0);  // Response code
  RXBuffer[1] = SPI.transfer(0);  // Length of data
  for (i=0;i<RXBuffer[1];i++)      
       RXBuffer[i+2]=SPI.transfer(0);  // Data
  digitalWrite(SSPin, HIGH);
  delay(1);

  if (RXBuffer[0] == 128) // Is response code good?
    {
      for(j=0;j<4;j++)
          str_data[j] = static_cast<char>(RXBuffer[j+3]);
      NFCReady = 2;
      Display=0;
      return 1;
     }
  else
     {
        NFCReady = 0;
      return 2;    
     }
 }
//******************************************************************************************************
void Read_Data(char *Data_To_Read,int NbBlock_To_Read, byte FirstBlock_To_Read) // Read data in EEPROM of tag 
 {
  int En,i,k,j=0;
  char Buffer_Data_Read[4]; 
  for(i=0;i<NbBlock_To_Read;i++)
     {
        En=Read_Memory(FirstBlock_To_Read+i,Buffer_Data_Read);
        if(En==1)
       {
        for(k=0;k<4;k++)
           {
             if(Buffer_Data_Read[k]!=0x00)
             {
             Data_To_Read[j]=Buffer_Data_Read[k];
             j++;
             }
          }
       }
    }
 }
//******************************************************************************************************
void loop() 
{  
   if(NFCReady == 0)
      { 
        SetProtocol_Command(); // ISO 15693 settings
        
      }
   else if (NFCReady == 1)
      {
        delay(100);  
        Inventory_Command();
      }
   else
      {
        delay(100);
        //lcd.setCursor(0, 1); 
        //lcd.print("Data:"); 
        Read_Data(Data_Eeprom,28,0);  // Read data in EEPROM of tag 
        delay(100);
        Serial.println(Data_Eeprom);
        delay(2000);
        Display=0;
      }   
}
/***********************************END OF FILE*********************************************************/
