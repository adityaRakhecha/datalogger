#include <Wire.h>
#include <DS1307.h> //rtc library
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

DS1307 rtc;
RTCDateTime dt;
const int CS=10;
String dataString="";
File dataFile;
TinyGPS gps;
SoftwareSerial ss(4,3); //RX 4 , TX 3

/*
* SD card attached to SPI bus as follows:
  MOSI - pin 11
  MISO - pin 12
  CLK - pin 13
  CS - pin 10
*/


static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);
float flat, flon;
unsigned long age, date, time, chars = 0;
unsigned short sentences = 0, failed = 0;
static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;


void setup() {
  rtc.begin();
  SD.begin(CS);
  ss.begin(9600); //if 9600 doesnt work change it to 4800
  //if date not set 
  if(!rtc.isReady()){
    rtc.setDateTime(__DATE__,__TIME__);
    }

dataFile=SD.open("data.txt",FILE_WRITE); 
//make a file data.txt in root of sd card and format card in FAT16/FAT32
dt=rtc.getDateTime();
dataString="Engine started at"+
            String(dt.hour)+":"+String(dt.minute)+":"+String(dt.second)+"  "+
            String(dt.day)+"-"+String(dt.month)+"-"+String(dt.year)+".";
            
if(dataFile){
  dataFile.println(dataString);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
dt=rtc.getDateTime();








}
