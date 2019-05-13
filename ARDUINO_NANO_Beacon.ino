#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int server=2; //server status reading pin
const int Wifi=3;   //Wifi status reading pin

void setup()   {            
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  display.clearDisplay(); //Clear oled screen

  pinMode(server,INPUT);
  pinMode(Wifi,INPUT);
  
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(28,0);
    display.println("BEACON");  //Project name
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("Initialising...");
    display.display();
    
    delay(1500);
    
    display.clearDisplay();
}

int wifi_status=0,server_status=0;  //initial status of connectivity
void loop() {
    display.clearDisplay();
  if(digitalRead(Wifi)==1 && digitalRead(server)==0 && wifi_status==0)  //check wifi status
  {
  connecting_wifi();
  while(digitalRead(Wifi)); //Wait for wifi connectivity
  wifi_status=1;  //update wifi status
  wifi_connected();
  }
  else if(digitalRead(Wifi)==0 && digitalRead(server)==1) //check server status
  {
  connecting_server();
  while(digitalRead(server)); //Wait for server connectivity
  server_status=1;  //update server status
  server_connected();
  }
  else if(digitalRead(Wifi)==0 && digitalRead(server)==0)
  {
    wifi_connected();
    server_connected();
    s_connected();
  }
}

void connecting_wifi(void)
{   
    display.setCursor(0,0);
    display.print("Connecting WiFi...");
    display.display();
    delay(500);
  }
void wifi_connected(void)
{   
    display.setCursor(0,8);
    display.print("WiFi Connected...");
    display.display();
    delay(500);
  }
void connecting_server(void)
{   
    display.setCursor(0,16);
    display.print("Error Connecting Host");
    display.display();
    delay(500);
  }
void server_connected(void)
{   
    display.setCursor(0,24);
    display.print("Server Connected...");
    display.display();
    delay(500);
  }
void s_connected(void)
{   
    display.clearDisplay();
  
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(28,0);
    display.println("BEACON");
    display.display();
    
    display.setTextSize(1);
    display.setCursor(0,24);
    display.print("Device Ready");
    display.display();
    delay(500);
  }
