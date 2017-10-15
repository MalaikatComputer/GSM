#include <gprs.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#include <bluetooth.h>
#include <fm.h>
#include <gprs.h>
#include <sim800.h>
#define TIMEOUT    5000
#define LED_PIN    13
#define ON LOW
#define OFF HIGH
int b,bb;

GPRS gprs;
 
void setup() 
  {
        pinMode(4, OUTPUT);
        pinMode(2, OUTPUT);
        pinMode(12, OUTPUT);
        pinMode(13, OUTPUT);
        pinMode(3, INPUT_PULLUP);
        
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
                     if(lastLine.indexOf("Lampu biru ON") >= 0)
                          {
                              gprs.sendSMS ("085242942595","lampu wes nyala");
                              digitalWrite(4, 1);
                          } 
                      else if(lastLine.indexOf("Lampu biru OFF") >= 0) 
                          {
                              gprs.sendSMS ("085242942595","Lampu wes mati");
                              digitalWrite(4, 0);
                              //gprs.sendSMS ("085242942595","LAMPU DIMATIKAN");
                          }
                      else if(lastLine.indexOf("Lampu merah ON") >= 0)
                          {
                              digitalWrite(2, 1);
                              gprs.sendSMS ("085242942595","lampu merah dinyalakan");
                          } 
                      else if(lastLine.indexOf("Lampu merah OFF") >= 0)
                          {
                              digitalWrite(2, 0);
                              gprs.sendSMS ("085242942595"," lampu merah dimatikan");
                          }  
                       else if(lastLine.indexOf("Semua OFF") >= 0)
                          {
                              digitalWrite(2, 0);
                              digitalWrite(4, 0);
                              gprs.sendSMS ("085242942595","semua lampu dimatikan");
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

