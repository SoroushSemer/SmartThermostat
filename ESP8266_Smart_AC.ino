/* MQTT AIR CONDITIONER NODE RED CONNECTION
 * CODE WRITTEN BY: SOROUSH SEMERKANT
 * LINKEDIN: https://www.linkedin.com/in/soroush-semerkant/
 * GITHUB: https://github.com/SoroushSemer
 * PUBLISHED ON: 6/7/2021
 * THIS CODE WAS WRITTEN FOR A FRIGIDAIRE AC
 */


#include <ESP8266WiFi.h>                                                  //ESP8266 Wifi Library
#include <PubSubClient.h>                                                 //MQTT Library 

const char* ssid = "SSID";                                                //wifi ssid for connection
const char* wifi_password = "PASSWORD";                                   //wifi password

const char* mqtt_server = "IP ADDRESS";                                   //MQTT server address
const char* mqtt_topic = "ac_onoff";                                      //MQTT topic to subscribe to for AC on/off state
const char* mqtt_topic2 = "ac_state";                                     //MQTT topic to subscribe to for AC state (auto (energy saver) / cool)
const char* mqtt_username = "MQTT USERNAME";                              //MQTT login username
const char* mqtt_password = "MQTT PASSWORD";                              //MQTT login password

const char* clientID = "ESP8266_5";                                       //Device ID to display to MQTT server

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

int IRledPin = D1;                                                        //IR LED Pin number

bool firstboot = false;                                                   //Variable to check if this the first time the code is running after connecting to the MQTT Server this is to prevent it turning on/off the AC when the device boots or wifi reconnects
void MQTTMessage(char* topic, byte* payload, unsigned int length) {       //Messaged Recieved from MQTT Server
  String onoff = "0", setting = "0";
  if (!firstboot) {                                                       
    if (strcmp(topic, "ac_onoff") == 0) {                                 //Checks if the message was from the AC on/off topic
      onoff = char(payload[0]);                                           //Gets the first character of the message
      AC_on_off();                                                        //Sends the power button IR signal
    }
    if (strcmp(topic, "ac_state") == 0) {                                 //Checks if the message was from the AC state topic
      setting = char(payload[0]);                                         //Gets the first character of the message
      if (setting == "0") {                                               //Checks if the state recieved was auto (code 0) 
        delay(1000);
        SendEnergySaver();                                                //Sends Energy Saver IR signal
        Serial.println("SENT ENERGY SAVER");
      }
      if (setting == "2") {                                               //Checks if the state recieved was cool (code 2)
        delay(1000);
        SendCool();                                                       //Sends Cool IR signal
        Serial.println("SENT COOL");
      }
      if (setting == "3") {                                               //Checks if the state recieved was up (code 3)
        delay(500);
        SendUP();                                                         //Sends Up (Puts up the AC temp) IR signal
      }
      if (setting == "4") {                                               //Checks if the state recieved was up (code 4)
        delay(500);
        SendDOWN();                                                       //Sends Down (Puts up the AC temp) IR signal
      }
    }
  }
  else {
    firstboot = false;                                                    //Checks if it was the firstboot then changes the variable to be false if so
  }
}

bool Connect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {           //Connects to the MQTT server with the given ID username and password
    firstboot = true;                                                     //This is the first boot/connection
    client.subscribe(mqtt_topic);                                         //Subscribes to the AC on/off topic
    client.subscribe(mqtt_topic2);                                        //Subscribes to the AC state topic
    return true;
  }
  else {
    return false;                                                         //Sends false if the connection failed
  }
}

void sendIR(long microsecs) {

  cli();                                                                  //stops interupts

  while (microsecs > 0) {                                                 //repeats the pulsing of LED based on the given amount of time
    digitalWrite(IRledPin, HIGH);                                         //turns on and off the IR LED
    delayMicroseconds(10);
    digitalWrite(IRledPin, LOW);
    delayMicroseconds(10);

    microsecs -= 26;
  }

  sei();                                                                  //puts interupts back
}
void AC_on_off() {
  Serial.println("Sending IR signal");
  SendOnOff();                                                            //sends the ON/OFF signal 3 times to make sure it is recieved (Simulates holding the power button for one second)
  SendOnOff();
  SendOnOff();
  Serial.println("SENT ON/OFF");
}
void SendOnOff() {                                                        //Codes for the ON/OFF signal for the AC Picked up using an IR Reciever
  delayMicroseconds(11924);                                              
  sendIR(9320);
  delayMicroseconds(4580);
  sendIR(620);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(560);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(1620);
  sendIR(700);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1720);
  sendIR(600);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(1700);
  sendIR(620);
  delayMicroseconds(1720);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(540);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(1700);
  sendIR(620);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(540);
  sendIR(620);
  delayMicroseconds(1740);
  sendIR(560);
  delayMicroseconds(1760);
  sendIR(580);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(42680);
  sendIR(9240);
  delayMicroseconds(2300);
  sendIR(580);
  delayMicroseconds(34084);
  sendIR(9160);
  delayMicroseconds(2300);
  sendIR(580);
}

void SendUP() {


  // This is the code for the on/off for the AC
  delayMicroseconds(52688);
  sendIR(9300);
  delayMicroseconds(4580);
  sendIR(600);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(600);
  delayMicroseconds(560);
  sendIR(560);
  delayMicroseconds(1780);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(600);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(600);
  delayMicroseconds(1740);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(1720);
  sendIR(580);
  delayMicroseconds(580);
  sendIR(580);
  delayMicroseconds(1780);
  sendIR(540);
  delayMicroseconds(1780);
  sendIR(540);
  delayMicroseconds(1780);
  sendIR(560);
  delayMicroseconds(1760);
  sendIR(560);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(1760);
  sendIR(560);
  delayMicroseconds(1780);
  sendIR(540);
  delayMicroseconds(1660);
  sendIR(680);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(580);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(580);
  sendIR(580);
  delayMicroseconds(1760);
  sendIR(560);
  delayMicroseconds(580);
  sendIR(600);
  delayMicroseconds(540);
  sendIR(580);
  delayMicroseconds(560);
  sendIR(580);
  delayMicroseconds(1780);
  sendIR(560);
  delayMicroseconds(1760);
  sendIR(580);
  delayMicroseconds(1740);
  sendIR(560);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(42720);
  sendIR(9220);
  delayMicroseconds(2300);
  sendIR(580);


}

void SendDOWN() {
  delayMicroseconds(6908);
  sendIR(9240);
  delayMicroseconds(4560);
  sendIR(640);
  delayMicroseconds(540);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(1660);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(1660);
  sendIR(660);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(1660);
  sendIR(660);
  delayMicroseconds(1660);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(500);
  sendIR(660);
  delayMicroseconds(500);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(1680);
  sendIR(640);
  delayMicroseconds(42620);
  sendIR(9300);
  delayMicroseconds(2240);
  sendIR(640);
  delayMicroseconds(34024);
  sendIR(9220);
  delayMicroseconds(2220);
  sendIR(640);
}

void SendCool() {
  delayMicroseconds(46852);
  sendIR( 9300);
  delayMicroseconds(4600);
  sendIR( 620);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 580);
  delayMicroseconds(580);
  sendIR( 580);
  delayMicroseconds(1760);
  sendIR( 580);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(580);
  sendIR( 560);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(580);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 560);
  delayMicroseconds(1760);
  sendIR( 560);
  delayMicroseconds(1760);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 620);
  delayMicroseconds(42640);
  sendIR( 9320);
  delayMicroseconds(2200);
  sendIR( 660);
  delayMicroseconds(34024);
  sendIR( 9140);
  delayMicroseconds(2300);
  sendIR( 660);

}

void SendEnergySaver() {
  delayMicroseconds(17356);
  sendIR( 9280);
  delayMicroseconds(4600);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(1720);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(520);
  sendIR( 620);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(540);
  sendIR( 600);
  delayMicroseconds(560);
  sendIR( 600);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(540);
  sendIR( 620);
  delayMicroseconds(1720);
  sendIR( 600);
  delayMicroseconds(1720);
  sendIR( 580);
  delayMicroseconds(1760);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(1740);
  sendIR( 580);
  delayMicroseconds(42680);
  sendIR( 9240);
  delayMicroseconds(2280);
  sendIR( 600);
  delayMicroseconds(34084);
  sendIR( 9140);
  delayMicroseconds(2280);
  sendIR( 600);


}

void setup() {

  pinMode(IRledPin, OUTPUT);                                            //Initializes IR LED
  Serial.begin(115200);                                                 //Turns on Serial Monitor to 115200 baud rate

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);                                      //Connects to WiFi

  while (WiFi.status() != WL_CONNECTED) {                               //Checks status of WiFi connection
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());                                       //Prints IP address of the module

  client.setCallback(MQTTMessage);
  if (Connect()) {                                                      //Checks if the device was able to connect to the MQTT server
    Serial.println("Connected Successfully to MQTT Broker!");
  }
  else {
    Serial.println("Connection Failed!");
  }
}

void loop() {
  if (!client.connected()) {                                            //Checks if the device is not connected to the MQTT server then connects to the server
    Connect();
  }
  client.loop();
}
