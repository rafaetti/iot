/*
 * This file contains code example on how to control the light and fan on Arno VX10 
 * and VX12 with Amazon Alexa.
 * 
 * The code can be used with either an ESP8266 or an ESP32 device.
 * 
 * The 3rd party libraries can be installed through the Arduino IDE library manager 
 * and also the source files for the projects can be found at:
 * 
 * ESP8266Wifi: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 * Espalexa: https://github.com/Aircoookie/Espalexa
 * Arno Fan Controller: https://github.com/ralexsander/arno-fan-controller
 * 
 * @Author: Rafael Favretti
 * @Date: 14/09/2023
 * @Version: 1.0
 */

//Include libraries
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include "ArnoController.h"

#define RF_PIN 2                                                                        //RF emitter pin
#define DEVICE_ID 9                                                                     //Decimal conversion of the binary information from the dip switches on the remote control where 0 means off and 1 (e.g. 1001 = to 9)

//Constants
const char* ssid      = "XXXXXXXXX";                                                    //Wifi SSID connection.
const char* password  = "XXXXXXXXXXXXXX";                                               //Wifi password.

//Variables
String lightDevice    = "XXXX";                                                         //Name of the device on the Alexa app. 
String fanDevice      = "XXXXXXXXXXXXXXXX";                                             //Name of the device on the Alexa app. 
boolean wifiConnected = false;                                                          //Check to validate wifi connection.
Espalexa espalexa;                                                                      //Espalexa class object.
ArnoController controller (RF_PIN, DEVICE_ID);                                          //ArnoController class object.

//Prototypes
boolean connectWifi();                                                                  //Prototype function that checks wifi connection.
void lightDeviceChanged(uint8_t bright);                                                //Prototype function to control change to the device.
void fanDeviceChanged(uint8_t bright);                                                  //Prototype function to control change to the device.

void setup()
{
  Serial.begin(9600);                                                                   //Start serial monitor on the board.

  wifiConnected = connectWifi();                                                        //Connect to the wifi.

  if (wifiConnected)
  {
    espalexa.addDevice(lightDevice, lightDeviceChanged);                                //Add the device(s) here.
    espalexa.addDevice(fanDevice, fanDeviceChanged);                                    //Add the device(s) here.
    espalexa.begin();                                                                   //Begin the Alexa module.
  }
  else
  {
    while (1)
    {
      Serial.println("Error connecting to the WiFi");                                   //Keep printing message to the serial monitor to inform that the connection has failed.
      delay(2500);
    }
  }
}

void loop()
{
  espalexa.loop();                                                                      //Perform a loop on the Alexa library to keep checking for activity.
  delay(1);
}

/*
 * Function that will control the light status (on/off)
 */
void lightDeviceChanged(uint8_t bright){
  Command cmd = LIGHT;
  controller.send(cmd);
}

/*
 * Function that will control the fan status (on/off)
 */
void fanDeviceChanged(uint8_t bright){
  Command cmd = FAN;
  controller.send(cmd);
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

  while (WiFi.status() != WL_CONNECTED) {                                                                   //Wait for connection
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  
  if (state) {                                                                                              //If the state remains as true, the connection is sucessful and connection information is printed.
    Serial.println("Connected to " + (String)ssid);
    Serial.println("IP address: " + WiFi.localIP().toString());
    return state;
  }
  
  Serial.println("Connection failed.");                                                                     //Returns a message informing that the connection failed.
  return state;
}
