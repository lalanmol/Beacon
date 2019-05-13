#include <MPU6050.h>

#include <MPU6050.h>

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <MPU6050.h>
#include<SoftwareSerial.h>
#include<TinyGPS.h>
#include <SPI.h>

MPU6050 mpu;
int SCL_PIN=D5;
int SDA_PIN=D4;

TinyGPS gps;
SoftwareSerial sgps(D2,D3); 

#define WIFI_AP "Beacon" //Wifi Name to be set
#define WIFI_PASSWORD "makeinindia"  //Wifi password to be set
#define TOKEN "9An5ZJlNWb7hrwAK359g" //Token key to send the data to cloud
char thingsboardServer[] = "demo.thingsboard.io"; //cloud server 

WiFiClient wifiClient;
PubSubClient client(wifiClient);
void reconnect();
void InitWiFi();
unsigned long lastSend;
int status = WL_IDLE_STATUS;

void setup()
{
pinMode(D0, OUTPUT);
pinMode(D1, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);


Serial.begin(115200);
  Wire.begin();
delay(10);
   sgps.begin(9600);
   
   InitWiFi();
   
   client.setServer( thingsboardServer, 1883 );
   lastSend = 0;
}

void loop()
{
if ( !client.connected() ) 
{
reconnect();
}
  digitalWrite(D1, LOW);
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  float combine=normAccel.XAxis+normAccel.YAxis+normAccel.ZAxis;  //Vibration data from MPU6050
  float temp = mpu.readTemperature(); //Temperature data from MPU6050

String payload = "{";  //Sting formation to upload the data 
payload += "\"temperature\":";
payload += String(temp);
payload += ",";
payload += "\"Vibrations\":";
payload += String(combine);

  while(sgps.available())   //GPS status
  {
    int c=sgps.read();
    if (gps.encode(c))
    {
      float lat,lon;
      gps.f_get_position(&lat,&lon);
      payload += ",";
      payload += "\"latitude\":";
      payload += String(lat,8);
      payload += ",";
      payload += "\"longitude\":";
      payload += String(lon,8);
      payload += "}";
      char attributes[100];
      payload.toCharArray( attributes, 100 );
      client.publish( "v1/devices/me/telemetry", attributes );
      Serial.println( attributes );
    }
  }
client.loop();
}


void InitWiFi()
{
   digitalWrite(D0, HIGH); //Connecting to wifi
   Serial.println("Connecting to AP ..."); // attempt to connect to WiFi network
     
   WiFi.begin(WIFI_AP, WIFI_PASSWORD);   //Parsing the wifi AP and password to get connect to wifi
   while (WiFi.status() != WL_CONNECTED) // checking if connected
     {
       delay(500);
       Serial.print("."); // if not then print 
     }
Serial.println("Connected to AP");
digitalWrite(D0, LOW);//Connected to wifi
}


void reconnect() 
{
 while (!client.connected()) // Loop until we're reconnected
 {
   status = WiFi.status();
   if ( status != WL_CONNECTED)
     {
      digitalWrite(D0, HIGH); //connecting to wifi
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED)
         {
          delay(500);
          Serial.print(".");
         }
      Serial.println("Connected to AP");
      digitalWrite(D0, LOW);// wifi connected
     }
      digitalWrite(D1, HIGH); //connnecting to data server
     Serial.print("Connecting to Thingsboard node ...");
     if ( client.connect("ESP8266 Device", TOKEN, NULL) ) // Attempt to connect (clientId, username, password)
        {
            Serial.println( "[DONE]" );
            // connected to the server
            digitalWrite(D6, LOW);
        }
    else
   {
    Serial.print( "[FAILED] [ rc = " );
    Serial.print( client.state() );
    Serial.println( " : retrying in 5 seconds]" );
    digitalWrite(D6, HIGH); //failed reconnecting to server
    delay( 5000 ); // Wait 5 seconds before retrying
   }
  }
}
