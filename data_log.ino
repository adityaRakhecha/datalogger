#include <Wire.h>
#include <DS1307.h> //rtc library
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <math.h>

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
char sz[32];
char temp[100];

void setup() {
  rtc.begin();
  SD.begin(CS);
  ss.begin(4800); //if 9600 doesnt work change it to 4800
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
  dataString="";
  // put your main code here, to run repeatedly:
dt=rtc.getDateTime();
dataString= String(dt.hour)+":"+String(dt.minute)+":"+String(dt.second)+"  "+
            String(dt.day)+"-"+String(dt.month)+"-"+String(dt.year)+" ";
 print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
dataString=dataString+temp+" ";
gps.f_get_position(&flat, &flon, &age);
print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
dataString=dataString+temp+" ";
print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
dataString=dataString+temp+" ";
print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
dataString=dataString+temp+" ";
print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
dataString=dataString+temp+" ";








}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
   if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
    strcpy(temp,sz);
  smartdelay(0);
}
static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      sprintf(temp,"***");
      }
  else
  {
    sprintf(temp,"%0.6f",val);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    //for (int i=flen; i<len; ++i)
    //temp=temp+" ";
  }
  smartdelay(0);
}