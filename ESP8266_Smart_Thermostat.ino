/* MQTT SMART THERMOSTAT/HEAT NODE RED CONNECTION
 * CODE WRITTEN BY: SOROUSH SEMERKANT
 * LINKEDIN: https://www.linkedin.com/in/soroush-semerkant/
 * GITHUB: https://github.com/SoroushSemer
 * PUBLISHED ON: 6/8/2021
 */

#include <ESP8266WiFi.h>                                                  //ESP8266 Wifi Library
#include <PubSubClient.h>                                                 //MQTT Library 
#include <OneWire.h>                                                      //One Wire Library for Temperature Sensor
#include <DallasTemperature.h>                                            //Temperature Sensor Library

#define ONE_WIRE_BUS 4                                                    // Pin definition for Temperature sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);                                      //Temperature sensor initialization
int relayPin = D0;                                                        //Pin definition for relay

char output_mode = '0';                                                   //output mode for the thermostat
double temp = 68;                                                         //variable to store current room temp
float setpoint = 78;                                                      //variable for goal temp

const char* ssid = "HM8";                                                 //wifi ssid for connection
const char* wifi_password = "squ@r3-n33dl3!";                             //wifi password for connection

const char* mqtt_server = "192.168.168.123";                              //MQTT server address
const char* mqtt_topic = "thermostat_goaltemp";                           //MQTT topic to subscribe to for thermostate goal temp
const char* mqtt_topic_pub = "thermostat_currenttemp";                    //MQTT topic to publish current temperature to
const char* mqtt_topic2 = "thermostat_goalstate";                         //MQTT topic to subscribe to for thermostat state
const char* mqtt_username = "soroushsemer";                               //MQTT login username
const char* mqtt_password = "femo56789!";                                 //MQTT login password
const char* clientID = "ESP8266_thermo";                                  //Device ID to display to MQTT server

WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); 

void publishTemp() {                                                      //Temperature Publish
  sensors.requestTemperatures();                                          //sense temp using the sensor

  temp = sensors.getTempCByIndex(0) - 4;                                  //gets the temperature in celsius and subtracts 4 (more accurate value)
  static char temperature[7];                                             //array of chars for temperature
  dtostrf(temp, 6, 2, temperature);                                       //converts temperature double to string
  
  if (client.publish(mqtt_topic_pub, temperature)) {                      //Publishes temperature value to MQTT server
    Serial.print("Current Temp: ");
    Serial.println(temperature);
  }
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    if (Connect()) {
      Serial.println("Connected Successfully to MQTT Broker!");
    }
    else {
      Serial.println("Connection Failed!");
    }
    client.publish(mqtt_topic_pub, temperature);
  }
}
void ReceivedMessage(char* topic, byte* payload, unsigned int length) {   //Messaged Recieved from MQTT Server
  if (strcmp(topic, "thermostat_goaltemp") == 0) {                        //Checks if the message was from the Goal Temp topic
    String output = "";
    for (int i = 0; i < 4; i++) {
      output += char(payload[i]);                                         //Reads first 4 characters from message
    }
    Serial.print("Goal Temp: ");
    Serial.println(output);
    setpoint = output.toFloat();                                          //Converts string to float for the goal temp
  }
  if (strcmp(topic, "thermostat_goalstate") == 0) {                       //Checks if the message was from the Goal state topic
    output_mode = char(payload[0]);                                       //Reads first character from message
    
    Serial.print("Goal State: ");
    Serial.println(output_mode);
  }

}

bool Connect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {           //Connects to the MQTT server with the given ID username and password
    client.subscribe(mqtt_topic);                                         //Subscribes to the Thermostat goal temp topic
    client.subscribe(mqtt_topic2);                                        //Subscribes to the AC state topic
    return true;
  }
  else {
    return false;                                                         //Sends false if the connection failed
  }

}

void setup() {
  Serial.begin(115200);                                                   //Turns on Serial Monitor to 115200 baud rate

  pinMode(relayPin, OUTPUT);                                              //Initializes relay
  digitalWrite(relayPin, LOW);                                            //Sets the relay to be LOW
  sensors.begin();                                                        //Starts the temperature sensor

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, wifi_password);                                        //Connects to WiFi

  while (WiFi.status() != WL_CONNECTED) {                                 //Checks status of WiFi connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());                                         //Prints IP address of the module

  client.setCallback(ReceivedMessage);

  if (Connect()) {                                                        //Checks if the device was able to connect to the MQTT server
    Serial.println("Connected Successfully to MQTT Broker!");
  }
  else {
    Serial.println("Connection Failed!");
  }
}
int msg = 0;
void loop() {

  if (!client.connected()) {                                              //Checks if the device is not connected to the MQTT server and connects it
    Connect();
  }

  client.loop();

  if (output_mode == '1' && temp <= setpoint - 1) {                       //if the Heat state is selected and if the current room temperature is one degree or less than the goal temperature
    digitalWrite(relayPin, HIGH);                                         //turn on the heat
    if (msg != 1) {                                                       
      Serial.println("HEAT IS NOW ON");
      msg = 1;
    }
  }

  if (temp >= setpoint || output_mode != '1') {                           //if the temperature is greater than the goal temperature and the current state is not heat
    digitalWrite(relayPin, LOW);                                          //turn off the heat
    if (msg != 2) {
      Serial.println("HEAT IS NOW OFF");
      msg = 2;
    }
  }
  publishTemp();                                                          //Publish the temperature
}
