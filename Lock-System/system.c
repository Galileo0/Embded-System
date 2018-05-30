#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
#include <TimeLib.h>
#include <string.h>
#include    <Servo.h>          //Servo library
#include <SoftwareSerial.h>

SoftwareSerial HC12(14,15);
//#include <pt.h>
 
Servo servo_test;        //

int angle = 0;    
int wrong = 0;
const int trigpin = 26;
const int echopin = 27;
int pir = 53;
int pirState=LOW;
int val =0;


#define TIME_HEADER "T"
#define TIME_REGUEST 7

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8  // can be a digital pin
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

//Recorder

#define REC 26
#define PLAY_E 27
#define PLAY_L 28
#define FT 29
#define playtime 5000
#define recordtime 3000
#define playLtime 900

#define LCD_RESET A4 

#define FLAME_SENSOR A8
int flame = 0;

/*____Calibrate TFT LCD_____*/
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
/*______End of Calibration______*/

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

TouchScreen ts = TouchScreen(XP,YP,XM,YM, 300); //300 is the sensitivity // XP ,YP ,XM, Ym
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int X,Y;
String statu = "[ Locked ]";
String cloc = "";

char pass[] = {'1','7','0','0'};


void setup() {
  // put your setup code here, to run once:

 
  Serial.begin(9600);
  
 
  
  progmemPrintln(PSTR("TFT LCD test"));

  pinMode(23,OUTPUT);//ledRed
  pinMode(24,OUTPUT); // ledBlue
  pinMode(25,OUTPUT); //Buzzer
  pinMode(53,INPUT);
   //Recorder
  pinMode(REC,INPUT);
  pinMode(PLAY_L,OUTPUT);
  pinMode(PLAY_E,OUTPUT);
  pinMode(FT,OUTPUT);

 
  
#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  progmemPrintln(PSTR("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  tft.reset();
  tft.begin(0x7575); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial

  // *** SPFD5408 change -- End

  progmemPrintln(PSTR("Benchmark                Time (microseconds)"));

  progmemPrint(PSTR("Screen fill              "));
 // Serial.println(testFillScreen());
 // delay(500);

 servo_test.attach(22);      // attach the signal pin of servo to pin9 of arduino

}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

int printDigits2(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  
  if(digits < 10)
    return 0;
  return digits;
}

void digitalClockDisplay()
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();

  cloc = String(hour())+":"+String(printDigits2(minute()))+":"+String(printDigits2(second()));
}


void record()
{
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.setCursor(30,130);
  tft.println("Recording");
  digitalWrite(REC,HIGH);
  Serial.println("Record start");
  delay(6000);
  digitalWrite(REC,LOW);
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.setCursor(25,130);
  tft.println("Record End");
  Serial.println("Record END");
  delay(1000);
   
}

void play()
{

  digitalWrite(PLAY_E,HIGH);
  Serial.println("PLAY start");
  delay(6000);
  digitalWrite(PLAY_E,LOW);
  Serial.println("Play END");
  delay(2000);
  
}


void pirm()
{
  
  
  
  val = digitalRead(pir);
  if(val == HIGH)
  {
    //Serial.println("LED ON");
    if(pirState == LOW)
    {
      Serial.println("Motion Detected");
      pirState = HIGH;
    }
  }else
  {
    //Serial.println("LED OFF");
    if(pirState == HIGH)
    {
      Serial.println("NOOOOOOOOOOOOOO Motion END");
      pirState = LOW;
    }
  }
}


void hc12(){

if(Serial.available())
  {
    HC12.write(Serial.read());
  }

  if(HC12.available())
  {
    Serial.write(HC12.read());
  }
  
/*
  while(HC12.available())
  {
    Serial.write(HC12.read());
  }

   while(HC12.available())
  {
    HC12.write(HC12.read());
  }*/
}
void loop() {
  // put your main code here, to run repeatedly:
  //testFillScreen();

  //digitalClockDisplay(); //clock
    
    //reset();
   
    
    screensaver();
   
    interface0();
    int flag = 0;
    while(flag !=1)
    {
        
          
        TSPoint p = waitTouch();
        X=p.y; Y=p.x;
        //Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);
        // Recorded Message
        if(X>=6&&X<=73)
        {
          record();
          play();
          flag = 1;
          break;
        }
        interface1();
        delay(1000);
        
        
        while (true)
        {
           p = waitTouch();
           X=p.y; Y=p.x;
          char method = detect();
          if(method == 'P')
          {
           // Serial.print('P');
            //Serial.write('P');
            pad();
            flag = 1;
            break;
          }
          else if(method == 'V')
          {
          //  Serial.print('V'); // voice use python assis
            Serial.write("VVVVV");
            delay(8000);

            while(true)
            {
              
                char bytein = Serial.read();
                
                if(bytein=='B')
               {
                Serial.write("OOOOOOOOO");
                delay(500);
                Serial.write("OOOOOOO");
                unlock();
                unlockinter();
                flag = 1;
                break;
                
               }else if(bytein=='E')
               {
                   error();
                   flag = 1;
                   break;
               }
               
                Serial.println(bytein);
                Serial.write(bytein);
              
              
              
               //break;
              
            }

            
           
           /* while(Serial.read())
            {
               // Serial.println("loop");
               // Serial.write('X');
                    // Serial.write(Serial.read());
                   if(Serial.read()=='A')
                   {
                      Serial.write('X');
                      break;
                    }
                
            }*/
            
            break;
          }
          else if(method == 'F')
          {
            //Serial.print('F');
            Serial.write("CAMCAMCAMCAMCAMCAMCAM");
            delay(8000);

            while(true)
            {
              
                char bytein = Serial.read();
                
                if(bytein=='B')
               {
                Serial.write("OOOOOOOOO");
                delay(500);
                Serial.write("OOOOOOO");
                unlock();
                unlockinter();
                flag = 1;
                break;
                
               }else if(bytein=='E')
               {
                   error();
                   flag = 1;
                   break;
               }
               
                Serial.println(bytein);
                Serial.write(bytein);
              
              
              
               //break;
              
            }


            break;
          }
        }
      
      }
  
  
  //delay(10000);
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}


// Text

unsigned long text() {
unsigned long start = micros();


tft.fillScreen(BLACK);
tft.setCursor(0,0);
tft.setTextColor(WHITE);  
tft.setTextSize(3);
tft.println("Hello World!");
circle();
//rec();
//vline();
//hline();

tft.setCursor(100,100);
tft.setTextColor(WHITE);  
tft.setTextSize(3);
tft.println("V");

  
}


unsigned long circle() {
unsigned long start = micros();

tft.drawCircle(100,100,30,YELLOW); // 100,100 center
tft.fillCircle(100,200,30,RED);    // 70 left-Right

}

unsigned long rec() {
unsigned long start = micros();

tft.fillRect(100,200,60,60,YELLOW);

}

// Lines
unsigned long hline() {
unsigned long start = micros();

for(int v=80;v<=320;v+=60)
{
  tft.drawFastHLine(v,80,240,WHITE);
}

}


unsigned long vline() {
unsigned long start = micros();

for(int v=0;v<=240;v+=60)
{
  tft.drawFastVLine(v,80,240,WHITE);
}

}


TSPoint waitTouch() {
  TSPoint p;
  do {
    p = ts.getPoint(); 
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);;
  return p;
}


void camreset()
{
  //Serial.print('F');
            Serial.write("CAMCAMCAMCAMCAMCAMCAM");
            delay(8000);

            while(true)
            {
              
                char bytein = Serial.read();
                
                if(bytein=='B')
               {
                Serial.write("OOOOOOOOO");
                delay(500);
                Serial.write("OOOOOOO");
                pad2();
                break;
                
               }else if(bytein=='E')
               {
                   error();
                  
                   break;
               }
               
                Serial.println(bytein);
                Serial.write(bytein);
              
              
              
               break;
              
            }


}

char detect()
{

  if(X>=6&&X<=88)
  {
   // Serial.println("P");
    return('P');
  }else if (X>=104&&X<=174)
  {
   // Serial.println("V");
    return('V');
  }else if (X>=195&&X<=245)
  {
   // Serial.println("F");
    return('F');
    
  }
}

char detectreset()
{

  if(X>=184&&X<=237)

  {

    if(Y>=195&&Y<=233)
    {
      return('Y');
    }else if (Y=37&&Y<=110)
    {
      return('N');
    }else{return('F');}
  }else{return('F');}
  
}

void interface1()
{
  tft.fillScreen(BLACK);
  tft.fillCircle(115,60,30,RED); // 100,100 center
  tft.fillCircle(115,160,30,RED); 
  tft.fillCircle(115,260,30,RED);// 70 left-Right
  tft.setCursor(110,50);
  tft.setTextColor(WHITE);  
  tft.setTextSize(3);
  tft.println("P");
  tft.setCursor(108,150);
  tft.println("V");
  tft.setCursor(108,250);
  tft.println("F");
  
}

void interface0()
{
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);  
  tft.setCursor(115,60);
   tft.fillCircle(120,70,30,RED);
   tft.println("M");  
  
  tft.setTextSize(3);
  tft.setCursor(30,140);
  tft.println(statu);

  tft.setTextColor(RED);  
  tft.setTextSize(3);
  
   tft.setCursor(105,245);
   tft.fillCircle(120,250,30,GREEN);
   tft.println("UL");
  //digitalClockDisplay();
  //tft.println(cloc);
  
  
}

void reset()
{

  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.setCursor(30,140);
  tft.println("RESET Pass");

  tft.setCursor(30,180);
  tft.println("[Face REC]");

  tft.drawCircle(75,270,30,GREEN);
  tft.setCursor(70,260);    // Right\Left | UP\Down
   tft.println("Y");

   tft.drawCircle(175,270,30,GREEN);
   tft.setCursor(165,260);
   tft.println("N");
  while(true)
  {
    TSPoint p = waitTouch();
    X=p.y; Y=p.x;
    Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);
    delay(500);
    char answer = detectreset();
    Serial.println(answer);
    if(answer == 'Y')
    {
      //Cam + Validation 
      camreset();
      break;
    }else if(answer == 'N')
    {
      break;
    }
  }
   
  
  
}

void unlock()
{


  digitalWrite(24,HIGH);
  digitalWrite(25,HIGH);
  delay(1000);
  digitalWrite(24,LOW);
  digitalWrite(25,LOW);
  delay(1000);
    for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);  
   // ultrafunc();//command to rotate the servo to the specified angle
    //delay(5); 
                          
  } 
 
  delay(100);
  
  /*for(angle = 180; angle>=0; angle-=1)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo_test.write(angle);
//ultrafunc();//command to rotate the servo to the specified angle
   // delay(5);                       
  } 

   for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);  
   // ultrafunc();//command to rotate the servo to the specified angle
    //delay(5); 
                          
  } */
  
}


void lock()
{
  
    for(angle = 180; angle >= 60; angle -= 10)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);  
   // ultrafunc();//command to rotate the servo to the specified angle
    //delay(5); 
                          
  } 
 
  delay(100);/*

for(angle = 0; angle < 180; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);  
   // ultrafunc();//command to rotate the servo to the specified angle
    //delay(5); 
                          
  } 


  delay(100);
  for(angle = 180; angle>=0; angle-=1)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo_test.write(angle);
//ultrafunc();//command to rotate the servo to the specified angle
   // delay(5);                       
  } 

   */
  
}

void error()
{
    
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);  
  tft.setTextSize(3);
  tft.setCursor(60,140);
  tft.println("Access");
  tft.setCursor(60,175);
  tft.println("Failed");
  digitalWrite(23,HIGH);
  digitalWrite(25,HIGH);
  delay(1000);
  digitalWrite(23,LOW);
  digitalWrite(25,LOW);
  delay(500);
  digitalWrite(23,HIGH);
  digitalWrite(25,HIGH);
  delay(1000);
  digitalWrite(23,LOW);
  digitalWrite(25,LOW);
  
  //Buzzer Sound error

  delay(3000);

  
  
   
  
}

void flamed()
{
  flame = analogRead(FLAME_SENSOR);
  if(flame >0)
  {
    flame = analogRead(FLAME_SENSOR);
    while(flame > 0)
    {
      tft.fillScreen(RED);
      tft.setTextColor(YELLOW);  
      tft.setTextSize(3);
      tft.setCursor(60,140);
      tft.println("FIRE");
      digitalWrite(23,HIGH);
      digitalWrite(25,HIGH);
      delay(1000);
      digitalWrite(23,LOW);
      digitalWrite(25,LOW);
      
    }
  }
}

void unlockinter()
{
  statu = "UnLocked";
    tft.fillScreen(BLACK);
  tft.setTextColor(GREEN);  
  tft.setTextSize(3);
  tft.setCursor(35,140);
  tft.println(statu);

  tft.setTextColor(GREEN);  
  tft.setTextSize(3);
  

   tft.setCursor(115,245);
   tft.fillCircle(120,250,30,RED);
   tft.println("L");

   delay(3000);
   
   
  //digitalClockDisplay();
  //tft.println(cloc);

  while (true)
  {
     
     TSPoint p = waitTouch();
    X=p.y; Y=p.x;
    //Serial.print(X); Serial.print(','); Serial.println(Y);// + " " + Y);
    if(X>=166&&X<=232)
    {
      lock(); //function
      break; // Lock The System
    }
  }
  statu = "[ Locked ]";
}

void screensaver()
{
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);  
  tft.setTextSize(3);
  tft.setCursor(30,140);
  tft.println("ROOM_LOCK");
  tft.setCursor(30,180);
  tft.println("SYSTEM");
  
  TSPoint p = waitTouch();
  
  
}


void pad()
{
  
    tft.fillScreen(BLACK);
  tft.drawCircle(115,60,30,YELLOW); // 100,100 center
  tft.drawCircle(115,160,30,YELLOW); 
  tft.drawCircle(115,260,30,YELLOW);// 70 left-Right
  tft.setCursor(110,50);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.println("1");
  tft.setCursor(108,150);
  tft.println("0");
  tft.setCursor(108,250);
  tft.println("7");
  int flag = 0;
  int flag2 = 1;
  for(int i =0;i<4;i++)
  {
    TSPoint p = waitTouch();
    
    X=p.y; Y=p.x;
    delay(500);
    char key = detectpass();
    Serial.println(key);
    if(key == pass[i])
    {
      flag = 1;
    }else
     {
      flag =0;
      flag2=0;
     
      }
    
  }

  if(flag == 1&&flag2==1)
  {
      unlock(); //motor
      //Buzzer True
      unlockinter();
        
  }else{error();
  
     if(wrong == 1)
     {
         wrong = 0;
          reset();
     }
     wrong++;
  
  }
  
  
  
}

void pad2()
{
     digitalWrite(24,HIGH);
    tft.fillScreen(BLACK);
  tft.drawCircle(115,60,30,YELLOW); // 100,100 center
  tft.drawCircle(115,160,30,YELLOW); 
  tft.drawCircle(115,260,30,YELLOW);// 70 left-Right
  tft.setCursor(110,50);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.println("1");
  tft.setCursor(108,150);
  tft.println("0");
  tft.setCursor(108,250);
  tft.println("7");
  int flag = 0;
  int flag2 = 1;
  for(int i =0;i<4;i++)
  {
    TSPoint p = waitTouch();
    
    X=p.y; Y=p.x;
    delay(500);
    char key = detectpass();
    Serial.println(key);
    pass[i] = key;
    
  }

  delay(2000);
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);  
  tft.setTextSize(3);
  tft.setCursor(100,130);
  tft.println("Done");
  digitalWrite(24,HIGH);
  digitalWrite(25,HIGH);
  delay(1000);
  digitalWrite(24,LOW);
  digitalWrite(25,LOW);
  delay(1000);
  
  delay(1000);
  
  
  
}

char detectpass()
{

  if(X>=6&&X<=88)
  {
   // Serial.println("P");
   digitalWrite(24,HIGH);
  delay(200);
  digitalWrite(24,LOW);
    return('1');
  }else if (X>=104&&X<=174)
  {
   // Serial.println("V");
   digitalWrite(24,HIGH);
  delay(200);
  digitalWrite(24,LOW);
    return('0');
  }else if (X>=195&&X<=245)
  {
   // Serial.println("F");
   digitalWrite(24,HIGH);
  delay(200);
  digitalWrite(24,LOW);
    return('7');
    
  }
}


void progmemPrint(const char *str) {
  char c;
  while(c = pgm_read_byte(str++)) Serial.print(c);
}

// Same as above, with trailing newline
void progmemPrintln(const char *str) {
  progmemPrint(str);
  Serial.println();
}
