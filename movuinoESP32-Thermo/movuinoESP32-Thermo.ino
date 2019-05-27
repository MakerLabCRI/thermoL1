#include "FS.h"
#include "SPIFFS.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_NeoPixel.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme1; // I2C

/*Neopixels global values*/
#define PIN           15
#define NUMPIXELS     1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

/*Pins*/
int buttonPin=13;
long lastButtonTime = 0;
int debounceDelay = 400;
int buttonState = 1;
int opMode=0;
long int sampleNb=0;
long int startTimer;
long int timer1;
int samplingFrequency = 1; //in HZ
int samplingPeriod=1000/samplingFrequency;
bool pixelState=false;
File fw;

void read_memory() {
  // nothing to do for now, this is just a simple test
  File f = SPIFFS.open("/data/1.txt", "r");
  // we could open the file
  while (f.available()) {
    //Lets read line by line from the file
    String line = f.readStringUntil('\n');
    Serial.println(line);
    delay(5);
   // yield();
  }
  f.close();
}
void blinkPixel()
  {
  if(!pixelState){
    pixels.setPixelColor(0, pixels.Color(0,0,125));
    }
  else {
    pixels.setPixelColor(0, pixels.Color(125,125,125));
    }
  pixelState=!pixelState;  
  pixels.show();  
  }

void setup(){
    Serial.begin(115200);
    pinMode(13,INPUT);
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    //Setup of BME280 sensor
    Serial.println(F("BME280 test"));
    bool status;
    status = bme1.begin(0x76);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    /*Pixel setup*/
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.setPixelColor(0, pixels.Color(125,125,125));
    pixels.show();
    delay(100);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
    delay(100);
}
void loop(){
  buttonState = digitalRead(buttonPin);
  if (!buttonState) {
    if (millis() - lastButtonTime > debounceDelay) {
      lastButtonTime = millis();
      Serial.println("Recording...");
      //if we are recording stop recording
      if (opMode == 2) {
        //TODO fonction start_recording & stop_recording
        fw.close();
        Serial.println("Stop recording");
        pixels.setPixelColor(0, pixels.Color(0,0,0));
        pixels.show();
        opMode = 0;
      }
      else {
        Serial.println("Recording");
        fw = SPIFFS.open("/data/1.txt", "w+");
        opMode = 2;
        }
    }
  }
 /*Serial Manager*/   
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    //Ping
    if (inByte == '?') {
      Serial.println("?");
    }
    else if (inByte == 'l') {
      Serial.println("Start live");
      opMode = 1;
    }
    //quit live mode
    else if (inByte == 'L') {
      opMode = 0;
    }
    else if (inByte == 'r') {
      Serial.println("recording!");
      fw = SPIFFS.open("/data/1.txt", "w+");
      startTimer=millis(); 
      opMode = 2;
    }
    else if (inByte == 'R') {
      Serial.println("Stop recording!");
      fw.close();
      pixels.setPixelColor(0, pixels.Color(0,0,0));
      pixels.show();
      //reset sample nb
      sampleNb=0;
      opMode = 0;
    }
    else if (inByte == 'p') {
      Serial.println("Read");
      read_memory();
      Serial.println("End of Read");
    }
      //delete file
    else if (inByte == 'P') {
      Serial.println("Deleting file");
      SPIFFS.remove("/data/1.txt");
      Serial.println("deletion");
      opMode = 0;
      //read_memory();
    }
  }  
  /*opMode Manager*/
  if (opMode == 1) {
    Serial.print(bme1.readTemperature());
    Serial.print(" *C ");
    Serial.print(bme1.readPressure() / 100.0F);
    Serial.print(" hPa  ");
    Serial.print(bme1.readHumidity());
    Serial.println(" % humidity");
    }
  else if (opMode == 2) {
    timer1=millis(); 
    if(timer1-startTimer>samplingPeriod) {
      startTimer=timer1;
      blinkPixel();
      fw.print(sampleNb);
      fw.print(" ");
      fw.print(bme1.readTemperature());
      fw.print(" ");
      fw.print(bme1.readPressure() / 100.0F);
      fw.print(" ");
      fw.println(bme1.readHumidity());
      sampleNb++;
      }
    else delay(1);
    }
}
