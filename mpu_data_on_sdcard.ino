/*
  SD card read/write
  
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 */
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

}

int s=0;
int m=30;
int h=18;

void loop() {
  float temp = mpu.readTemperature();
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  String payload=String(h);
  payload+=":";
  payload+=String(m);
  payload+=":";
  payload+=String(s);
  payload+="|";
  payload+=String(26);
  payload+="/";
  payload+=String(9);
  payload+="/";
  payload+=String(18);
  payload+=",";
  payload+=String("temperature(C)");
  payload+=",";
  payload+=String(temp);
  payload+=",";
  payload+=String("X-Axis(g)");
  payload+=",";
  payload+=normAccel.XAxis;
  payload+=",";
  payload+=String("Y-Axis(g)");
  payload+=",";
  payload+=normAccel.YAxis;
  payload+=",";
  payload+=String("Z-Axis(g)");
  payload+=",";
  payload+=normAccel.ZAxis;
  
  Serial.println(payload);
  // nothing happens after setup
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("Beacon.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
//    Serial.print("Writing to Beacon.txt...");
    myFile.println(payload);
    // close the file:
    myFile.close();
//    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Beacon.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("Beacon.txt");
  if (myFile) {
//    Serial.println("Beacon.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Beacon.txt");
  }
  delay(1000);
  s++;
  if(s==60)
  {
    m++;
    s=0;
    }
    if(m==60)
    {
      h++;
      m=0;
      }
}


