//Name: ArduInterTH1
//Description: This program measures the DHT11 Temperature and Humidity sensor when certain conditions
//             are met. It interacts with other another Arduino device using XBee communication and posts
//             the current values of the measurement to Thingworx IoT platform every 3 secs using a WiFi
//             Shield for Arduino.
//Authors: David Velasquez (mail: dvelas25@eafit.edu.co)
//         Raul Mazo       (mail: raul.mazo@univ-paris1.fr)
//Version: 1.0
//Date: 11/1/2016

//Required Libraries
#include <SPI.h>
#include <WiFi.h>
#include <ThingWorxWiFi.h>
#include "DHT.h"

//Pin I/O Labeling
#define DHTPIN 2  //DHT sensor connected to Arduino digital pin 2

//Constants
const unsigned long postingInterval = 2 * 1000; //Delay between TWX POST updates, 2000 milliseconds
const unsigned int sensorCount = 2; //Number of sensor vars sent to TWX, 2 vars

//Variables
//->WiFi Shield Vars
char ssid[] = "Private";  //Network SSID (name)
char pass[] = "eafit321*";  //Network password
int status = WL_IDLE_STATUS;  //Network status
//->TWX Vars
char* server = "10.102.19.60";  //TWX server (do not include http://)
char* appKey = "3a0678fa-eeb3-4d8a-b793-c3e968624739";  //API Key from TWX
char* thingName = "DVthing";  //Name of your Thing in TWX
char* serviceName = "DVservice";  //Name of your Service in TWX
char* nameArray[] = {"ArdValue1", "ArdValue2"}; //Vector Var names from TWX service inputs
float sensorValues[sensorCount];  //Vector for Var values
ThingWorx myThing(server, appKey, thingName, serviceName);  //Thingworx object var
//->DHT11 Vars
#define DHTTYPE DHT11 //Use DHT11 sensor variant
float t = 0;  //Var Environment Temperature (t) as float
float h = 0;  //Var Relative Humidity (h) as float
DHT dht(DHTPIN, DHTTYPE); //DHT object var
//->XBee Communication Vars
char readCommXB = '\0'; //Char command read through XBee communication
char sendCommXB = '\0'; //Char command sent through XBee communication
//->Computer Communication Vars
char readCommPC = '\0'; //Char command read through PC communication
char sendCommPC = '\0'; //Char command sent through PC communication
//->Timing Vars
unsigned long lastConnectionTime = 0; //Last time you connected to the server, in milliseconds

//Subroutines & Functions
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void WiFiInit() {
  // give the WiFi Shield module time to boot up:
  delay(1000);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status:
  printWifiStatus();
}

void readPC() {
  if (Serial.available() > 0) { //Check if there are bytes sent from PC
    readCommPC = Serial.read(); //Store in readCommPC the incoming char
    Serial.flush(); //Clean Serial port buffer
  }
}

void readXBee() {
  if (Serial1.available() > 0) { //Check if there are bytes sent from XBee
    readCommXBee = Serial1.read(); //Store in readCommXBee the incoming char
    Serial1.flush(); //Clean Serial1 port buffer
  }
}

void setup() {
  //Pin type definition
  //pinMode(PIN#, OUTPUT or INPUT);

  //Physical output cleaning
  //digitalWrite(PIN#, LOW);

  //Communications initialization
  Serial.begin(9600); //Initialize Serial communications through TX0 and RX0 (PC)
  Serial1.begin(9600);  //Initialize Serial communications through TX1 and RX1 (XBee)
  dht.begin();  //Initialize communications with DHT11 sensor
  WiFiInit(); //Initialize communications through WiFi Shield
}

void loop() {
  readPC(); //Check and store the incoming command from PC
  readXBee(); //Check and store the incoming command from XBee
  t = dht.readTemperature();
  h = dht.readHumidity();
  sensorValues[0] = t;
  sensorValues[1] = h;
  if ((millis() - lastConnectionTime) > postingInterval)
  {
    myThing.UpdateValues(sensorCount, nameArray, sensorValues);  //This is the library function that constructs the REST call and sends it to your ThingWorx service
    lastConnectionTime = millis();  //Update the last connection time
  }
}
