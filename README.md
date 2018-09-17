# Weather Embedded System Interaction
![alt text](https://raw.githubusercontent.com/tidusdavid/weather-interaction-eafit/master/Resources/Architecture.png)
![alt text](https://raw.githubusercontent.com/tidusdavid/weather-interaction-eafit/master/Resources/Device.jpg)

If there is a problem sending the POST request, try the following instructions:
Please download the respective Arduino Board by going into the Arduino IDE App, then to the Tools menu, Board -> Boards Manager and
select the Arduino AVR Boards, then pick from the Select version list box the 1.6.7 version and click install (only applies to older WiFi Shield). After the installations finishes,
Close Arduino IDE and open it again, your Weather Embedded System Code should now work.

## Testing

In order to test this project
First upload the ArduinoCodes/ArduInterTH_masterWiFi101/ArduInterTH_masterWiFi101.ino to the Arduino MEGA, edit the code header to match your SSID, Thingworx server, thing name, service name, api key, etc and then monitor through Thingworx Platform that the values change depending on the DHT11 sensor state.

Then upload the ArduinoCodes/ArduInterTH_slaveWiFi101/ArduInterTH_slaveWiFi101.ino to the Arduino MEGA, edit the code header to match your SSID, Thingworx server, thing name, service name, api key, etc. Start heating the first device (master) with your hands and check that after 30°C the Slave starts sending information. If temperature goes back to 27°C slave goes back to sleep
