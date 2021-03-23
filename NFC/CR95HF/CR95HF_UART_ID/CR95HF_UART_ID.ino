/**
  ******************************************************************************
    @file    20190122_arduino_cr95_ByteArray2Dec.ino
    @editor  Bui Thanh Tung
    @company Tag4yu
    @mail    tung.bui@tag4yu.com
    @version V1.1.0
    @date    22-Jan-2019
    @brief   Arduino firmware for CR95HF module (PN: IDER1501CR95).
    @history Add one function: ByteArray2Dec -> converted UID 7 bytes from HEX to DEC (8 bytes not including "E0")
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <SoftwareSerial.h>
/* Private define ------------------------------------------------------------*/
//#define USE_TT1//Remove rem this Flag if your want to use TT1 detect
#define USE_TT5

#define CR95HF_TX (5)//Pin 2 on Ardruino -> TX on Reader
#define CR95HF_RX (6) //Pin 3 on Ardruino connect to RX on Reader
byte UID[8];
int Type[1];
SoftwareSerial CR95HF_UART(CR95HF_TX, CR95HF_RX); // D2 is RX on arduino, D3 is TX on arduino

void setup() {
  led_init();
  led_on();
  Serial.begin(9600);
  first_display();
  led_off();
  CR95HF_UART_init();
}

void loop() {

  Protocol_Selection_type2();
  delay(20);
  for (byte i = 0; i < 3; i++)
    if (Type2_Request_A(UID, Type))
    {
      switch (Type[0])
      {
		#ifdef USE_TT1
        case 1:
          Serial.println(F("---Tag Type 1 was Detected---"));
          break;
		#endif //USE_TT1

        case 2:
          Serial.println(F("---Tag Type 2 was Detected---"));
          if (UID[5] == 0 && UID[6] == 0)
          {
            PrintHex8(UID, 4);
            //ByteArray2Dec(UID, 2);
          }
          else
          {
            PrintHex8(UID, 7);
            //ByteArray2Dec(UID, 4);
          }
          break;

        case 4:
          Serial.println(F("---Tag Type 4 was Detected---"));
          PrintHex8(UID, 7);
//          ByteArray2Dec(UID, 4);
          break;

		#ifdef USE_TT1
        case 7:
          Serial.println(F("---Mifare Tag was Detected---"));
          PrintHex8(UID, 4);
          //ByteArray2Dec(UID, 4);
          break;
		#endif//USE_TT1
      }
      led_on();
    }
  Anten_off();
  delay(20);

#ifdef USE_TT5
  Protocol_Selection_typeV();//Detect TT5
  delay(20);
  for (byte i = 0; i < 3; i++) {

    if (TypeV_Inventory(UID, Type))
    {
      Serial.println(F("---Tag Type 5 was Detected---"));
      PrintHex8(UID, 8);
//      ByteArray2Dec(UID, 5);
      led_on();
    }
    Anten_off();
  }
#endif//USE_TT5
  delay(200);
  led_off();
}

/* CR95HF function ------------------------------------------------------------*/
void CR95HF_UART_init()
{
  byte echo[] = {0x55};
  CR95HF_UART.begin(57600); //CR95HF-baudrate-default

  Serial.println(F("---CR95HF connecting...---"));
  while (CR95HF_UART.available() == 0) {
    CR95HF_UART.write(echo, 1); // wakeup CR95HF
  }
  Serial.println(F("---CR95HF connected---"));
}

/* 15693 ------------------------------------------------------------*/
#ifdef USE_TT5
void Protocol_Selection_typeV()
{
  byte message1[] = {0x02, 0x02, 0x01, 0x0D};
  byte message2[] = {0x09, 0x04, 0x68, 0x01, 0x01, 0x50};
  CR95HF_UART.write(message1, sizeof(message1));
  CR95HF_UART.write(message2, sizeof(message2));
}

bool TypeV_Inventory(byte* UID, int* Type)
{
  byte response[16];
  byte i = 0;
  const byte message1[5] = {0x04, 0x03, 0x26, 0x01, 0x00};
  Type[0] = 5;
  CR95HF_UART.write(message1, sizeof(message1));
  i = 0;
  delay(4);
  while (CR95HF_UART.available())         // reading data into char array
  {
    response[i++] = CR95HF_UART.read();
  }
  if ((response[0] == 0x80) && (response[1] == 0x0D))
  {
    //Read and reverse UID of TT5
    UID[0] = response[11];
    UID[1] = response[10];
    UID[2] = response[9];
    UID[3] = response[8];
    UID[4] = response[7];
    UID[5] = response[6];
    UID[6] = response[5];
    UID[7] = response[4];
    return true;
  }
  return false;
}
#endif //USE_TT5
/* 14443 ------------------------------------------------------------*/
void Protocol_Selection_type2()
{
  const byte message1[4] = {0x02, 0x02, 0x02, 0x00};
  const byte message2[6] = {0x09, 0x04, 0x3A, 0x00, 0x5A, 0x04};
  const byte message3[6] = {0x09, 0x04, 0x68, 0x01, 0x01, 0xDF};
  CR95HF_UART.write(message1, sizeof(message1));
  CR95HF_UART.write(message2, sizeof(message2));
  CR95HF_UART.write(message3, sizeof(message3));
}

bool Type2_Request_A(byte* UID, int* Type)
{
  byte response[16];
  byte i = 0;
  const byte message1[4] = {0x04, 0x02, 0x26, 0x07};                                     //----------- ReQuest A
  const byte message2[5] = {0x04, 0x03, 0x93, 0x20, 0x08};                               //----------- Anticolision 1
  byte message3[10]      = {0x04, 0x08, 0x93, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28}; //----------- Select 1
  const byte message4[5] = {0x04, 0x03, 0x95, 0x20, 0x08};                               //----------- Anticolision 2
#ifdef USE_TT1
  const byte message5[10] = {0x04, 0x08, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA8}; //---------- Topaz
#endif//USE_TT1

  Type[0] = 0;
  CR95HF_UART.write(message1, sizeof(message1));
  delay(2);
  while (CR95HF_UART.available()) // reading data into char array
  {
    response[i++] = CR95HF_UART.read();
  }
  if (response[2] == 0x44)
    Type[0] = 2;
  if (response[2] == 0x42)
    Type[0] = 4;
#ifdef USE_TT1
  if (response[2] == 0x04) Type[0] = 7;
  if (response[2] == 0x00)
  {
    Type[0] = 1;
    { //-------------------topaz
      CR95HF_UART.write(message5, sizeof(message5));
      i = 0;
      delay(4);
      while (CR95HF_UART.available())         // reading data into char array
      {
        response[i++] = CR95HF_UART.read();
      }
      if ((response[0] == 0x80) && (response[1] == 0x0B))
      {
        UID[0] = response[4];
        UID[1] = response[5];
        UID[2] = response[6];
        UID[3] = response[7];
        return true;
      }

    }
  }
#endif//USE_TT1
  if ((response[0] == 0x80) && (response[1] == 0x05))
  {
    CR95HF_UART.write(message2, sizeof(message2));
    i = 0;
    delay(2);
    while (CR95HF_UART.available())         // reading data into char array
    {
      response[i++] = CR95HF_UART.read();
    }

    if ((response[0] == 0x80) && (response[1] == 0x08))
    {
#ifdef USE_TT1
      if (Type[0] == 7)
      { //------------------Mifare
        UID[0] = response[2];
        UID[1] = response[3];
        UID[2] = response[4];
        UID[3] = response[5];
        return true;
      }
#endif //USE_TT1
      UID[0] = response[3];
      UID[1] = response[4];
      UID[2] = response[5];
      message3[4] = response[2];
      message3[5] = response[3];
      message3[6] = response[4];
      message3[7] = response[5];
      message3[8] = response[6];
      CR95HF_UART.write(message3, sizeof(message3));
      i = 0;
      delay(2);
      while (CR95HF_UART.available())         // reading data into char array
      {
        response[i++] = CR95HF_UART.read();
      }
      if ((response[0] == 0x80) && (response[1] == 0x06))
      {
        CR95HF_UART.write(message4, sizeof(message4));
        i = 0;
        delay(2);
        while (CR95HF_UART.available())         // reading data into char array
        {
          response[i++] = CR95HF_UART.read();
        }
        if ((response[0] == 0x80) && (response[1] == 0x08))
        {
          UID[3] = response[2];
          UID[4] = response[3];
          UID[5] = response[4];
          UID[6] = response[5];
          message3[4] = response[2];
          message3[5] = response[3];
          message3[6] = response[4];
          message3[7] = response[5];
          message3[8] = response[6];
          CR95HF_UART.write(message3, sizeof(message3));
          return true;
        }
      }
    }
  }
  return false;
}

void Anten_off(void)
{
  byte message1[] = {0x02, 0x02, 0x00, 0x00};
  CR95HF_UART.write(message1, sizeof(message1));
}

// Display first message
void first_display(void)
{
  Serial.println(F("-------Tag4yu Demo-------"));
  Serial.println(F("---CR95HF--------Arduino--"));
  Serial.println(F("---GND--------------GND---"));
  Serial.println(F("---VCC---------------5V---"));
  Serial.println(F("---TX-----------------2---"));
  Serial.println(F("---RX-----------------3---"));
  Serial.println(F("Read NFC UID Type 2 or 4 or 5"));
  Serial.println(F("----------start----------"));
}

// prints 8-bit data in hex with leading zeroes
void PrintHex8(uint8_t *data, uint8_t length)
{
  char tmp;
  Serial.print(F("UID_HEX: "));
  for (byte i = 0; i < length; i++) {
    //sprintf(tmp, "%.2X", data[i]);
    /*
       Note: Dung.LV change from sprintf to Manual convert because sprintf function use to much memory
        -Use sprintf Sketch size is 8060 bytes
        -But use Manual convert, Sketch size is 6532 bytes
        =>Reduce Sketch size is 1528 bytes
    */
    tmp = (data[i] >> 4) + 0x30;//0->9
    if (tmp > 0x39) tmp += 7; //A->F
    Serial.print(tmp);
    tmp = (data[i] & 0x0F) + 0x30;//0->9
    if (tmp > 0x39) tmp += 7; //A->F
    Serial.print(tmp);
  }
  Serial.println();
}

/* init LED */
void led_init(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
}

//Turn on LED
void led_on(void)
{
  digitalWrite(LED_BUILTIN, HIGH);
}

//Turn off LED
void led_off(void)
{
  digitalWrite(LED_BUILTIN, LOW);
}

// Function to convert Byte Array to Decimal
void ByteArray2Dec(char* UID, byte num)
{
  Serial.print(F("UID_DEC: "));
  switch (num)
  {
    case 2://TT2 with UID 4 bytes
      {
        uint32_t UID_DEC = 0;
        //Convert from Bytes to Non-sign Integer 32 bits
        UID_DEC = (UID[0] & 0xFF) << 24 | //byte 0
                  (UID[1] & 0xFF) << 16 | //byte 1
                  (UID[2] & 0xFF) << 8 | //byte 2
                  (UID[3] & 0xFF) << 0; //Byte 3

        Serial.print(UID_DEC);
        break;
      }
    case 5://TT5 with 8 bytes UID, include E0, in right order
      {
        //Convert from bytes to Decimal 64 bit using shift register
        uint64_t UID_DEC =  (uint64_t)(UID[1] & 0xFF) << 48 | //Byte 1
                            (uint64_t)(UID[2] & 0xFF) << 40 | //byte 2
                            (uint64_t)(UID[3] & 0xFF) << 32 | //byte 3
                            (uint64_t)(UID[4] & 0xFF) << 24 | //Byte 4
                            (uint64_t)(UID[5] & 0xFF) << 16 | //Byte 5
                            (uint64_t)(UID[6] & 0xFF) << 8  | //Byte 6
                            (uint64_t)(UID[7] & 0xFF) << 0;   //Byte 7

        Print64Bit(UID_DEC, 15);
        break;
      }
    case 4://TT4 or TT2 with uid 7 bytes
      {
        //Convert 7 bytes to int 64 bits
        uint64_t UID_DEC =  (uint64_t)(UID[0] & 0xFF) << 48 | //Byte 0
                            (uint64_t)(UID[1] & 0xFF) << 40 | //byte 1
                            (uint64_t)(UID[2] & 0xFF) << 32 | //byte 2
                            (uint64_t)(UID[3] & 0xFF) << 24 | //Byte 3
                            (uint64_t)(UID[4] & 0xFF) << 16 | //Byte 4
                            (uint64_t)(UID[5] & 0xFF) << 8  | //Byte 5
                            (uint64_t)(UID[6] & 0xFF) << 0;   //Byte 6
        Print64Bit(UID_DEC, 16);
        break;
      }
    default: break;
  }
  Serial.println();
}

//Print n in number to screen
void Print64Bit(uint64_t number, byte n)
{
  byte str_uid[n];
  for (byte i = 0; i <= n; i++) {
    str_uid[n - 1 - i] = (byte)(number % 10);
    number = number / 10;
  }
  for (byte i = 0; i < n; i++)
  {
    Serial.print(str_uid[i]);
  }

}
