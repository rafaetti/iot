/*
 * This file contains code example on how to add an relay device connected to a lightbulb 
 * and control it with Amazon Alexa, how ever the base code is simple and can be adjusted 
 * as needed to control all sort of devices.
 * 
 * The code can be used with either an ESP8266 or an ESP32 device.
 * 
 * The 3rd party libraries can be installed through the Arduino IDE library manager 
 * and also the source files for the projects can be found at:
 * 
 * ESP8266Wifi: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 * Espalexa: https://github.com/Aircoookie/Espalexa
 * 
 * @Author: Rafael Favretti
 * @Date: 01/05/2021
 * @Version: 1.0
 */

//Include libraries
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>

//Constants
const int replayPin   = 0;                                                                                  //Integer variable correspoding the port number for the relay pin on the board.
const char* ssid      = "XXXXXXXXX";                                                                        //Wifi SSID connection.
const char* password  = "XXXXXXXXXXXXXX";                                                                   //Wifi password.

//Variables
String deviceName     = "XXXX";                                                                             //Name of the device on the Alexa app.  
boolean wifiConnected = false;                                                                              //Check to validate wifi connection.
Espalexa espalexa;                                                                                          //Espalexa class object.

//Prototypes
boolean connectWifi();                                                                                      //Prototype function that checks wifi connection.
void lightDeviceChanged(uint8_t bright);                                                                    //Prototype function to control change to the device.


void setup()
{
  Serial.begin(9600);                                                                                       //Start serial monitor on the board.
  pinMode(replayPin, OUTPUT);                                                                               //Configure the pinMode as output.  

  wifiConnected = connectWifi();                                                                            //Connect to the wifi.

  if (wifiConnected)
  {
    espalexa.addDevice(deviceName, lightDeviceChanged);                                                     //Add the device(s) here.
    espalexa.begin();                                                                                       //Begin the Alexa module.
  }
  else
  {
    while (1)
    {
      Serial.println("Error connecting to the WiFi");                                                       //Keep printing message to the serial monitor to inform that the connection has failed.
      delay(2500);
    }
  }
}

void loop()
{
  espalexa.loop();                                                                                          //Perform a loop on the Alexa library to keep checking for activity.
  delay(1);
}

/*
 * Function that will control the light status (on/off)
 */
void lightDeviceChanged(uint8_t bright){
  int action = !digitalRead(replayPin);                                                                     //Read the status of the relay and set the action as the oposite.
  digitalWrite(replayPin, action);
}

/*
 * Function that perform wifi authentication and connection
 * @Return State of the wifi connection
 */
boolean connectWifi()
{
  boolean state = true;                                                                                     //State of the connection.
  int counter   = 0;                                                                                        //Retry counter for the connection.

  WiFi.mode(WIFI_STA);                                                                                      //Set the wifi mode as station.
  WiFi.begin(ssid, password);                                                                               //Try to connect
  
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {                                                                   //Wait for connection while and keep waiting until retry count reaches 20
    delay(500);
    Serial.print(".");
    if (counter > 20) {
      state = false; 
      break;
    }
    counter++;
  }
  
  Serial.println("");
  
  if (state) {                                                                                              //If the state remains as true, the connection is sucessful and connection information is printed.
    Serial.println("Connected to " + (String)ssid);
    Serial.println("IP address: " + WiFi.localIP());
    return state;
  }
  
  Serial.println("Connection failed.");                                                                     //Returns a message informing that the connection failed.
  return state;
}
