#include <gprs.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#include <bluetooth.h>
#include <fm.h>
#include <gprs.h>
#include <sim800.h>
#define TIMEOUT    5000
#define LED_PIN    4
#define LED_PIN    5
#define LED_PIN    6
#define ON LOW
#define OFF HIGH
int b,bb;

GPRS gprs;
 
void setup() 
  {
        pinMode(4, OUTPUT);
        pinMode(5, OUTPUT);
        pinMode(6, OUTPUT);
        pinMode(13, OUTPUT);
        pinMode(2, INPUT_PULLUP);
        
        Serial.begin(9600);
        while(!Serial);
       
        Serial.println("Starting SIM800 Auto Read SMS");
        gprs.preInit();
        delay(1000);
       
        while(0 != gprs.init()) 
          {
              delay(1000);
              Serial.print("init error\r\n");
          }
        if(0 != gprs.sendCmdAndWaitForResp("AT+CMGF=1\r\n", "OK", TIMEOUT)) 
          {
            ERROR("ERROR:CNMI");
            return;
          }
        if(0 != gprs.sendCmdAndWaitForResp("AT+CNMI=1,2,0,0,0\r\n", "OK", TIMEOUT)) 
          {
            ERROR("ERROR:CNMI");
            return;
          }
        Serial.println("Init success");
        digitalWrite(13, 1);
    }
char currentLine[500] = "";
int currentLineIndex = 0;
bool nextLineIsMessage = false;
 
void loop() 
  {
    b=digitalRead(3);
    if(gprs.serialSIM800.available()){
      char lastCharRead = gprs.serialSIM800.read();
      if(lastCharRead == '\r' || lastCharRead == '\n')
      {
          String lastLine = String(currentLine);
          if(lastLine.startsWith("+CMT:"))
            {  
                Serial.println(lastLine);
                nextLineIsMessage = true;
            } 
          else if (lastLine.length() > 0)
            {  
                if(nextLineIsMessage) 
                  {
                    Serial.println(lastLine);
//==================================CH-01==========================================//
                     if(lastLine.indexOf("1") >= 0)
                          {
                              gprs.sendSMS ("085255286623","Lampu sudah dinyalakan ");
                              digitalWrite(4, 1);//pin4 sama dengan on;
                          } 
                      else if(lastLine.indexOf("A") >= 0) 
                          {
                              gprs.sendSMS ("085255286623","Lampu sudah dimatikan ");
                              digitalWrite(4, 0); //pin4 sama dengan off;
                              //gprs.sendSMS ("085394544432","LAMPU DIMATIKAN");
                          }
//==================================CH-02==========================================//
                     if(lastLine.indexOf("2") >= 0)
                          {
                              gprs.sendSMS ("085255286623","Lampu2 sudah dinyalakan ");
                              digitalWrite(5, 1);//pin5 sama dengan on;
                          } 
                      else if(lastLine.indexOf("B") >= 0) 
                          {
                              gprs.sendSMS ("085255286623","Lampu2 sudah dimatikan ");
                              digitalWrite(5, 0); //pin5 sama dengan off;
                              //gprs.sendSMS ("085394544432","LAMPU DIMATIKAN");
                          }
//==================================CH-03==========================================//
                     if(lastLine.indexOf("3") >= 0)
                          {
                              gprs.sendSMS ("085255286623","Lampu3 sudah dinyalakan ");
                              digitalWrite(6, 1);//pin6 sama dengan on;
                          } 
                      else if(lastLine.indexOf("C") >= 0) 
                          {
                              gprs.sendSMS ("085255286623","Lampu3 sudah dimatikan ");
                              digitalWrite(6, 0); //pin6 sama dengan off;
                              //gprs.sendSMS ("085394544432","LAMPU DIMATIKAN");
                          }
//==================================CH-all==========================================//
                  
                       if(lastLine.indexOf("4") >= 0)
                          {
                              digitalWrite(4, 1);
                              digitalWrite(5, 1);
                              digitalWrite(6, 1);
                              gprs.sendSMS ("085255286623","Semua lampu dinyalakan");
                          } 
                  
                       else if(lastLine.indexOf("D") >= 0)
                          {
                              digitalWrite(4, 0);
                              digitalWrite(5, 0);
                              digitalWrite(6, 0);
                              gprs.sendSMS ("085255286623","Semua lampu dimatikan");
                          }
                     
                     
                    nextLineIsMessage = false;
                  }
            }           
          //Clear char array for next line of read
          for( int i = 0; i < sizeof(currentLine);  ++i )
            {
               currentLine[i] = (char)0;
            }
          currentLineIndex = 0;
        } 
      else 
        {
          currentLine[currentLineIndex++] = lastCharRead;
        }
    }
   else if(b==0)
      {
        bb++;
        delay(250);
        if(bb>2){bb=1;}
        if(bb==1)
            {
             digitalWrite(10, 1);
            }
        else if(bb==2)
            {
             digitalWrite(10, 0);
            }   
      }
}  
