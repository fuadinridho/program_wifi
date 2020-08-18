#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> //display
#include <Adafruit_SSD1306.h>
#include <NMEAGPS.h> // GPS
#include <GPSport.h>
#include <NMEAGPS_cfg.h>
#include <GPSfix_cfg.h>
#include <MS5611.h> //barometer
#include <SHT1x.h> //suhu & kelembaban
#include <BH1750.h> //sensor cahaya
#include <ArduinoJson.h>
#include <Keypad.h>
#include "Config.h"

Adafruit_SSD1306 display(OLED_RESET);
SHT1x sht1x(dataPin, clockPin);
static NMEAGPS  gps;
MS5611 ms5611;
BH1750 lightMeter;

char check_;
unsigned long currentTimeWifi, previousTimeWifi;
unsigned long currentTimeSonar, previousTimeSonar;
int k=0;
char pass[4] = {'_','_','_','_'};
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 10, 11}; //connect to the column pinouts of the keypad
bool huruf = false;
//initialize an instance of class NewKeypad
bool adapass = false;

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

static void GPSloop();

float kelembaban;
float suhu;
float sudutX,sudutY,sudutZ;
float akselX,akselY,akselZ;
float speedWind;
unsigned char SW1,SW2,SW3,SW4;
unsigned char pinadc;
float volbat;
float lux =0;
float curahhujan=0;
double longitude,latitude;
bool first=false;
double pressure; 
#define RainPin 2                         // The Rain input is connected to digital pin 2 on the arduino

bool bucketPositionA = false;             // one of the two positions of tipping-bucket               
const double bucketAmount = 0.053;        // 0.053 inches atau 1.346 mm of rain equivalent of ml to trip tipping-bucket 

double dailyRain = 0.0;                   // rain accumulated for the day
double hourlyRain = 0.0;                  // rain accumulated for one hour
double dailyRain_till_LastHour = 0.0;     // rain accumulated for the day till the last hour          

//AT+CIPSTART="TCP","amicagama.id",80

String cmdconnect="";
String header1,header2,header3,header4;
String postdata="";

int menit=0;

/**
 * Method to build the body request
 * @param  buffer   variable to return data from struct
 * @param  tempData struct as an input for processing the body request
 * @return          body parameters
 */
const char* param(char buffer[], int deviceId, float temp, float hum, float flux, float baro, float longitude, float latitude) {
  char valueBuffer[10];

  strcpy_P(buffer, (const char *)F(""));
  strcat_P(buffer, (const char *)F("device_id="));
  strcat(buffer, itoa(deviceId, valueBuffer, 10));
  strcat_P(buffer, (const char *)F("&temperature="));
  strcat(buffer, dtostrf(temp, 2, 2, valueBuffer));
  strcat_P(buffer, (const char*)F("&humidity="));
  strcat(buffer, dtostrf(hum, 2, 2, valueBuffer));
  strcat_P(buffer, (const char*)F("&luminous_flux="));
  strcat(buffer, dtostrf(flux, 2, 2, valueBuffer));
  strcat_P(buffer, (const char*)F("&air_pressure="));
  strcat(buffer, dtostrf(baro, 2, 2, valueBuffer));
  strcat_P(buffer, (const char*)F("&longitude="));
  strcat(buffer, dtostrf(longitude, 2, 2, valueBuffer));
  strcat_P(buffer, (const char*)F("&latitude="));
  strcat(buffer, dtostrf(latitude, 2, 2, valueBuffer));

  return buffer;
}

/**
 * Method to concat the header and body request
 * @return concat of header and body request
 */
const char* getPostData(void) {
  char body[120];
  char postData[300];
  char valueBuffer[45];
  sprintf_P(valueBuffer, (const char *) F("POST /api/devices/%d/measurements HTTP/1.1\r\n"), DEVICE_ID);

  param(body, DEVICE_ID, suhu, kelembaban, lux, pressure, longitude, latitude);

  Serial.println(body);

  strcpy_P(postData, (const char *)F(""));
  strcat(postData, valueBuffer);
  strcat_P(postData, (const char *)F("Host: amicagama.id\r\n"));
  strcat_P(postData, (const char *)F("Accept: application/json\r\n"));
  strcat_P(postData, (const char *)F("Content-Type: application/x-www-form-urlencoded\r\n"));
  strcat_P(postData, (const char *)F("User-Agent: amicagama\r\n"));
  strcat_P(postData, (const char *)F("Connection: close\r\n"));
  strcat_P(postData, (const char *)F("Content-Length:")); strcat(postData, itoa(strlen(body), valueBuffer, 10)); strcat_P(postData, (const char *)F("\r\n\r\n"));

  strcat(postData, body);

//  strcpy(requestData, postData);

  Serial.println("getPostData has done");
  
  return postData;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  ms5611.begin(MS5611_ULTRA_HIGH_RES);
  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
    
  ms5611.getOversampling();
  
  pinMode(SW1_PIN,INPUT_PULLUP);
  pinMode(SW2_PIN,INPUT_PULLUP);
  pinMode(SW3_PIN,INPUT_PULLUP);
  pinMode(SW4_PIN,INPUT_PULLUP);
  
  // put your setup code here, to run once:
  init_wifi();
  gpsPort.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  char customKey = customKeypad.getKey();
  
  Serial.println("loop...");


//display.setTextSize(1);
//display.setTextColor(WHITE);
//
//  SW1 = digitalRead(SW1_PIN);
//  SW2 = digitalRead(SW2_PIN);
//  SW3 = digitalRead(SW3_PIN);
//  SW4 = digitalRead(SW4_PIN);
//
//if(adapass == false)
//{
//    display.clearDisplay();
//    display.setCursor(0, 0);
//    display.println("   input device id:");
//display.println("1 2 3");
//display.println("4 5 6");
//display.println("7 8 9");
//
//
//for(int i=0;i<4;i++)
//{
//display.setCursor(70+(i*8), 8);
//display.println(pass[i]);
//}
//
//display.setCursor(70+(k*8), 16);
//display.println("^");
//
//if(k>3){k=3;}
//if(k<0){k=0;}
//
//
//display.display();
//  if (customKey=='1'){pass[k]='1';k++;delay(100);}
//  if (customKey=='2'){pass[k]='2';k++;delay(100);}
//  if (customKey=='3'){pass[k]='3';k++;delay(100);}
//  if (customKey=='4'){pass[k]='4';k++;delay(100);}
//  if (customKey=='5'){pass[k]='5';k++;delay(100);}
//  if (customKey=='6'){pass[k]='6';k++;delay(100);}
//  if (customKey=='7'){pass[k]='7';k++;delay(100);}
//  if (customKey=='8'){pass[k]='8';k++;delay(100);}
//  if (customKey=='9'){pass[k]='9';k++;delay(100);}
//  if (customKey=='0'){pass[k]='0';k++;delay(100);}
//  if (customKey=='*'){pass[k]='_';k--;delay(100);}
//  
//  
//if (customKey=='#' && k==3){
//      display.clearDisplay();
//      display.setCursor(3, 0);
//      display.println("      DEVICE ID");
//      display.setCursor(0, 8);
//      display.println("       DISIMPAN");
//      
//      for(int i=0;i<4;i++)
//{
//display.setCursor(50+(i*8),20);
//display.println(pass[i]);
//display.display();
//adapass = true;
//delay(4000);
//display.clearDisplay();
//}
//
//  }
//}
//else
//{
  

  GPSloop();

  suhu = read_temp();
  kelembaban = read_hum();
  lux =read_lux();
  pressure = read_baro();
  tampil();
  currentTimeWifi = micros();
  // ngirim tiap 1 menit
  
  if(currentTimeWifi-previousTimeWifi>=20000000)
  { 
    Serial.println("prepare to sending data...");
    menit++;
    if(menit>59){menit=0;}
    //getPostData();
    //strcpy(postData, getPostData());

    char body[120];
    char postData[300];
    char valueBuffer[45];
    sprintf_P(valueBuffer, (const char *) F("POST /api/devices/%d/measurements HTTP/1.1\r\n"), DEVICE_ID);
  
    param(body, DEVICE_ID, suhu, kelembaban, lux, pressure, longitude, latitude);
  
    strcpy_P(postData, (const char *)F(""));
    strcat(postData, valueBuffer);
    strcat_P(postData, (const char *)F("Host: amicagama.id\r\n"));
    strcat_P(postData, (const char *)F("Accept: application/json\r\n"));
    strcat_P(postData, (const char *)F("Content-Type: application/x-www-form-urlencoded\r\n"));
    strcat_P(postData, (const char *)F("User-Agent: amicagama\r\n"));
    strcat_P(postData, (const char *)F("Connection: close\r\n"));
    strcat_P(postData, (const char *)F("Content-Length:")); strcat(postData, itoa(strlen(body), valueBuffer, 10)); strcat_P(postData, (const char *)F("\r\n\r\n"));
  
    strcat(postData, body);

    Serial.println(postData);
    
    httpPost(postData);
  
    previousTimeWifi = micros();
  }
  first = false;
//}

}
