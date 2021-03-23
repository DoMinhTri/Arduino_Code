/*Arduino Total Control for beginners
 Basic functions to control and display information into the app.
 This code works for every arduino board.
 For full features please check the arduino uno and mega codes.
 
 * Ethernet shield Module attached
   - For UNO pins 10, 11, 12 and 13 are dedicated
   - For MEGA pins 10, 50, 51 and 52 are dedicated
 * Controls 8 relays, appliances, circuits, etc. (for n relays check uno/mega code)
 * A button from GND to A5 explains how to manualy turn on/off relay 1
 * Relays connected to RelayPinss[MAX_RELAYS]
 * Take analog samples from sensors connected to A0 to A4 
 * Relay states are remembered
 
 To send data to app use tags:
 For buttons: (<ButnXX:Y\n), XX 0 to 19 is the button number, Y 0 or 1 is the state
 Example: server.println("<Butn05:1"); will turn the app button 5 on
 
 For texts: <TextXX:YYYY\n, XX 0 to 19 is the text number, YYYY... is the string to be displayed
 Example: server.println("<Text01:A1: 253"); will display the text "A1: 253" at text 1
 
 For images: <ImgsXX:Y\n, XX 0 to 19 is the image number, Y is the image state(0, 1 or 2)to be displayed
 Example: server.println("<Imgs02:1"); will change image 2 to the pressed state
 
 If a  no tag new line ending string is sent, it will be displayed at the top of the app
 Example: server.println("Hello Word"); "Hello Word" will be displayed at the top of the app
 
 Author: Juan Luis Gonzalez Bello 
 Date: October 2014   
 Get the app: https://play.google.com/store/apps/details?id=com.apps.emim.btrelaycontrol
 ** After copy-paste of this code, use Tools -> Atomatic Format 
 */

#include <EEPROM.h> 
#include <SPI.h> 
#include <Ethernet.h> 

// Number of relays
#define MAX_RELAYS 8 
// Relay 1 is at pin 2, relay 2 is at pin 3 and so on.
int RelayPins[MAX_RELAYS]  = {2, 3, 4, 5, 6, 7, 8, 9};
// Relay 1 will report status to toggle button and image 1, relay 2 to button 2 and so on.
String RelayAppId[] = {"00", "01", "02", "03", "04", "05", "06", "07"};
 
// Command list
#define CMD_R1_ON  'A'
#define CMD_R1_OFF 'a'
#define CMD_R2_ON  'B'
#define CMD_R2_OFF 'b'
#define CMD_R3_ON  'C'
#define CMD_R3_OFF 'c'
#define CMD_R4_ON  'D'
#define CMD_R4_OFF 'd'
#define CMD_R5_ON  'E'
#define CMD_R5_OFF 'e'
#define CMD_R6_ON  'F'
#define CMD_R6_OFF 'f'
#define CMD_R7_ON  'G'
#define CMD_R7_OFF 'g'
#define CMD_R8_ON  'H'
#define CMD_R8_OFF 'h' 
  
// Used to keep track of the relay status 
int RelayStatus = 0; 
int STATUS_EEADR = 20; 

// Used to prescale sample time
int Prescaler = 0;
boolean buttonLatch = false;

// Enter a MAC address and IP address for your controller below. 
// The IP address will be dependent on your local network: 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
IPAddress ip(192,168,1,15); 

// Initialize the Ethernet server library 
// with the IP address and port you want to use 
// (port 80 is default for HTTP): 
EthernetServer server(80); 
EthernetClient client; 

void setup() {  
  pinMode(A5, INPUT);     // seet A5 as digital input
  digitalWrite(A5, HIGH); // Enable pull up register

  // start the Ethernet connection and the server: 
  Ethernet.begin(mac, ip); 
  server.begin();

  // Initialize Output PORTS 
  for(int i = 0; i < MAX_RELAYS; i++){ 
    pinMode(RelayPins[i], OUTPUT); 
  }

  // Load last known status from eeprom 
  RelayStatus = EEPROM.read(STATUS_EEADR); 
  for(int i = 0; i < MAX_RELAYS; i++){ 
    // Turn on and off according to relay status
    if((RelayStatus & (1 << i)) == 0){ 
      digitalWrite(RelayPins[i], LOW);
      server.println("<Butn" + RelayAppId[i] + ":0");
    } 
    else {
      digitalWrite(RelayPins[i], HIGH); 
      server.println("<Butn" + RelayAppId[i] + ":1");      
    }  
  } 

  // Greet arduino total control on top of the app
  server.println("Thanks for your support!"); 
} 

void loop() { 
  String sSample; 
  String sSampleNo;  
  int iSample; 
  int appData; 

  delay(1); 
  
  // This is true each second approx. 
  if(Prescaler++ > 1000){ 
    Prescaler = 0; // Reset prescaler 

    // Send 3 analog samples to be displayed at text tags 
    iSample = analogRead(A0);  // Take sample
    sSample = String(iSample); // Convert into string

    // Example of how to use text tags 
    server.println("<Text00:Sensor 0: " + sSample);

    sSample = String(analogRead(A1)); 
    server.println("<Text01:Temp 1: " + sSample);

    sSample = String(analogRead(A2)); 
    server.println("<Text02:Speed 2: " + sSample);

    sSample = String(analogRead(A3)); 
    server.println("<Text03:Photo 3: " + sSample);

    sSample = String(analogRead(A4)); 
    server.println("<Text16:Flux 4: " + sSample);
  } 

  // =========================================================== 
  // This is the point were you get data from the App 
  client = server.available(); 
  if (client){ 
    appData = client.read(); 
  }
  
  switch(appData){ 
  case CMD_R1_ON:
    // turn on relay 1, save relay status and set image and button feedbacks
    setRelayState(0, 1);
    break;

  case CMD_R1_OFF:
    // turn off relay 1, save relay status and set image and button feedbacks
    setRelayState(0, 0);
    break;

  case 'B':
    // The following is equal to setRelayState(1, 1);
    digitalWrite(RelayPins[1], HIGH);        // Write ouput port
    server.println("<Butn" + RelayAppId[1] + ":1"); // Feedback button state to app
    server.println("<Imgs" + RelayAppId[1] + ":1");             // Set image to pressed state
    RelayStatus |= (0x01 << 1);              // Set relay status
    EEPROM.write(STATUS_EEADR, RelayStatus); // Save new relay status
    break;

  case 'b':
    // The following is equal to setRelayState(1, 0);
    digitalWrite(RelayPins[1], LOW);        // Write ouput port
    server.println("<Butn" + RelayAppId[1] + ":0"); // Feedback button state to app
    server.println("<Imgs" + RelayAppId[1] + ":0");             // Set image to default state
    RelayStatus &= ~(0x01 << 1);             // Clear relay status
    EEPROM.write(STATUS_EEADR, RelayStatus); // Save new relay status
    break;

  case CMD_R3_ON:
    setRelayState(2, 1);
    break;

  case CMD_R3_OFF:
    setRelayState(2, 0);
    break;

  case CMD_R4_ON:
    setRelayState(3, 1);
    break;

  case CMD_R4_OFF:
    setRelayState(3, 0);
    break;

  case CMD_R5_ON:
    setRelayState(4, 1);
    break;

  case CMD_R5_OFF:
    setRelayState(4, 0);
    break;

  case CMD_R6_ON:
    setRelayState(5, 1);
    break;

  case CMD_R6_OFF:
    setRelayState(5, 0);
    break;

  case CMD_R7_ON:
    setRelayState(6, 1);
    break;

  case CMD_R7_OFF:
    setRelayState(6, 0);
    break;

  case CMD_R8_ON:
    setRelayState(7, 1);
    break;

  case CMD_R8_OFF:
    setRelayState(7, 0);
    break;

  case '<': 
    // Special command received 
    // This is for accelerometer data, check uno/mega codes
    break; 

  case '[': 
    // Character '[' is received every 2.5s, use
    // this event to tell the android all relay states 
    server.println("Beginner's Code"); 

    for(int i = 0; i < MAX_RELAYS; i++){ 
      // Refresh button states to app (<BtnXX:Y\n)
      if(digitalRead(RelayPins[i])){ 
        server.println("<Butn" + RelayAppId[i] + ":1");
        server.println("<Imgs" + RelayAppId[i] + ":1");
      } 
      else {
        server.println("<Butn" + RelayAppId[i] + ":0"); 
        server.println("<Imgs" + RelayAppId[i] + ":0");
      }  
    } 
    break; 
  } 
  
    // Manual button
  if(!digitalRead(A5)){ // If button pressed
    // don't change relay status until button has been released and pressed again
    if(buttonLatch){ 
      setRelayState(0, !digitalRead(RelayPins[0])); // toggle relay 0 state
      buttonLatch = false;                       
    }
  }
  else{
    // button released, enable next push
    buttonLatch = true;
  }
  // ========================================================== 
} 

// Sets the relay state for this example
// relay: 0 to 7 relay number
// state: 0 is off, 1 is on
void setRelayState(int relay, int state){  
  if(state == 1){ 
    digitalWrite(RelayPins[relay], HIGH);           // Write ouput port
    server.println("<Butn" + RelayAppId[relay] + ":1"); // Feedback button state to app
    server.println("<Imgs" + RelayAppId[relay] + ":1"); // Set image to pressed state
    
    RelayStatus |= (0x01 << relay);                 // Set relay status
    EEPROM.write(STATUS_EEADR, RelayStatus);        // Save new relay status
  } 
  else {
    digitalWrite(RelayPins[relay], LOW);            // Write ouput port
    server.println("<Butn" + RelayAppId[relay] + ":0"); // Feedback button state to app
    server.println("<Imgs" + RelayAppId[relay] + ":0"); // Set image to default state
   
    RelayStatus &= ~(0x01 << relay);                // Clear relay status
    EEPROM.write(STATUS_EEADR, RelayStatus);        // Save new relay status
  }
}


