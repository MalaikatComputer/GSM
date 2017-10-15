
#include <gprs.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#define TIMEOUT    5000
#define LED_PIN    13
#define ON LOW
#define OFF HIGH
int b,bb;

GPRS gprs;
 
void setup() 
  {
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
        Serial.println("Sukses mi cest...!!!!");
        digitalWrite(12, 1);
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
                     if(lastLine.indexOf("Lampu Ok") >= 0)
                          {
                              digitalWrite(2, 1);
                              gprs.sendSMS ("085242942595","mau apa.???.LAMPU DINYALAKAN");
                              gprs.sendSMS ("085394544432","mau apa.???.LAMPU DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("Lampu OFF") >= 0) 
                          {
                              gprs.sendSMS ("085242942595","lampu sudah dimatikan");
                              gprs.sendSMS ("085394544432","lampu sudah dimatikan");
                              digitalWrite(2, 0);
                              //gprs.sendSMS ("085394544432","LAMPU DIMATIKAN");
                          }/*
//==================================CH-02==========================================//
                     else if(lastLine.indexOf("CH 2 ON") >= 0)
                          {
                              digitalWrite(12, 1);
                              gprs.sendSMS ("085394544432","CHANEL 2 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 2 OFF") >= 0) 
                          {
                              digitalWrite(12, 1);
                              gprs.sendSMS ("085394544432","CHANEL 2 DIMATIKAN");
                          }
//==================================CH-03==========================================//
                     else if(lastLine.indexOf("CH 3 ON") >= 0)
                          {
                              analogWrite(A0, 255);
                              gprs.sendSMS ("085394544432","CHANEL 3 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 3 OFF") >= 0) 
                          {
                              analogWrite(A0, 0);
                              gprs.sendSMS ("085394544432","CHANEL 3 DIMATIKAN");
                          }
//==================================CH-04==========================================//
                     else if(lastLine.indexOf("CH 4 ON") >= 0)
                          {
                              analogWrite(A1, 255);
                              gprs.sendSMS ("085394544432","CHANEL 4 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 4 OFF") >= 0) 
                          {
                              analogWrite(A1, 0);
                              gprs.sendSMS ("085394544432","CHANEL 4 DIMATIKAN");
                          }*/
                          
                          /*
//==================================CH-05==========================================//
                     else if(lastLine.indexOf("CH 5 ON") >= 0)
                          {
                              analogWrite(A2, 255);
                              gprs.sendSMS ("085394544432","CHANEL 5 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 5 OFF") >= 0) 
                          {
                              analogWrite(A2, 0);
                              gprs.sendSMS ("085394544432","CHANEL 5 DIMATIKAN");
                          }
//==================================CH-06==========================================//
                     else if(lastLine.indexOf("CH 6 ON") >= 0)
                          {
                              analogWrite(A3, 255);
                              gprs.sendSMS ("085394544432","CHANEL 6 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 6 OFF") >= 0) 
                          {
                              analogWrite(A3, 0);
                              gprs.sendSMS ("085394544432","CHANEL 6 DIMATIKAN");
                          }
//==================================CH-07==========================================//
                     else if(lastLine.indexOf("CH 7 ON") >= 0)
                          {
                              analogWrite(A4, 255);
                              gprs.sendSMS ("085394544432","CHANEL 7 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 7 OFF") >= 0) 
                          {
                              analogWrite(A4, 0);
                              gprs.sendSMS ("085394544432","CHANEL 7 DIMATIKAN");
                          }
//==================================CH-08==========================================//
                     else if(lastLine.indexOf("CH 8 ON") >= 0)
                          {
                              analogWrite(A5, 255);
                              gprs.sendSMS ("085394544432","CHANEL 8 DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("CH 8 OFF") >= 0) 
                          {
                              analogWrite(A5, 0);
                              gprs.sendSMS ("085394544432","CHANEL 8 DIMATIKAN");
                          }
//==================================SEMUA==========================================//
                     else if(lastLine.indexOf("SEMUA ON") >= 0)
                          {
                              analogWrite(A0, 255);
                              analogWrite(A1, 255);
                              analogWrite(A2, 255);
                              analogWrite(A3, 255);
                              analogWrite(A4, 255);
                              analogWrite(A5, 255);
                              digitalWrite(12, 1);
                              digitalWrite(13, 1);
                              gprs.sendSMS ("085394544432","SEMUA CHANEL DINYALAKAN");
                          } 
                      else if(lastLine.indexOf("SEMUA OFF") >= 0) 
                          {
                              analogWrite(A0, 0);
                              analogWrite(A1, 0);
                              analogWrite(A2, 0);
                              analogWrite(A3, 0);
                              analogWrite(A4, 0);
                              analogWrite(A5, 0);
                              digitalWrite(12, 0);
                              digitalWrite(13, 0);
                              gprs.sendSMS ("085394544432","SEMUA CHANEL DIMATIKAN");
                          }*/
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

