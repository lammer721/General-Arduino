#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>

U8G2_SSD1309_128X64_NONAME0_F_4W_SW_SPI u8g2(U8G2_R2, 18, 23, 5, 17, 21);  

const int button1Pin = 12;                                                    //Input pin(s) for buttons and HR monitor
const int button2Pin = 14;

int start1Time;
int start2Time;
int stop1Time;
int stop2Time;

int timer1Time;
int timer2Time;

boolean timergo = false;
boolean subTimerGo = false;

void setup(void) {
  Serial.begin(115200);
  u8g2.begin();
  Serial.println("Setup");

  //---------- Wait for Button to start timer-----------------------
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  int button1State;
  button1State = digitalRead(button1Pin);
  Serial.println(button1State);

    while (button1State == HIGH){
     Serial.println("Waiting");

     button1State = digitalRead(button1Pin);
    }
  delay(500);  
}

void loop(void) {

int j;
int i;

float subTimes[10];
float lapTimes[10];
//---------- Buttons for timer------------------------------------

int button1State = digitalRead(button1Pin);               //read the state of the timer button value:
     Serial.println("button1State");
     Serial.println(button1State);

int button2State = digitalRead(button2Pin);
     Serial.println("button2State");
     Serial.println(button2State);

//----------IF MAIN TIMER IS RUNNING------------------------------

  if (timergo == true){                                  //Check if main timer is running
      Serial.println("Timer Running");
      if (button1State == LOW) {                       //Check if main timer button is pressed.
        stop1Time = millis();                           //If so, record stop time,
        lapTimes[i] = stop1Time - start1Time;           //add time to lap times,
        i++;                                            //and increment lap time array.
        if (button2State == LOW){                      //And if the second button is pressed at the same time
            timergo=false;                              //stop timer readings altogether.
            subTimerGo = false;
            delay(100);                                 //and wait a bit for button to be released
            }
        else{
          start1Time = millis();                          //Otherwise, just perform a lap, reset timer and continue.
          stop1Time = 0;
        }
      }

      if (subTimerGo == true){                           //Check if sub timer is running.
        Serial.println("Subtimer Running");

        if (button2State == LOW){                      //Check if sub timer button is pressed.
            stop2Time = millis();                       //If so, record stop time,
            subTimes[j] = stop2Time - start2Time;       //add time to sub lap times,
            j++;                                        //and increment sub lap time array,
            subTimerGo = false;                         //and stop sub timer readings.
          }
        else{
            timer2Time = millis()-start2Time;         
        }
        }
      else{                                             //If sub timer is NOT running,
        if (button2State == LOW){                      //Check if sub timer button is pressed
            start2Time = millis();                      //If so, start sub timer,
            subTimerGo = true;                          //and start sub timer readings.
            delay(250);
          }
        else{
            timer2Time = stop2Time - start2Time;            //If sub timer isn't running AND button isn't pressed DISPLAY LAST KNOWN SUB TIME.
          }
      }
      timer1Time = millis()-start1Time;                //If neither button is pressed, just continue to display uninterrupted main time,
  }

//----------IF TIMER IS NOT RUNNING-------------------------------

  if (timergo == false){
      Serial.println("Timer not running");
      button1State = digitalRead(button1Pin);               //read the state of the timer button value:
      if (button1State == LOW) {                      //check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        start1Time = millis();
        timergo=true;
        Serial.println("Timer Started");
      }
      timer1Time = stop1Time - start1Time;
      timer2Time = stop2Time - start2Time;
  }

    timer1Time = timer1Time/1000;
    timer2Time = timer2Time/1000;
     
    int hour1 = timer1Time/3600;
    timer1Time = timer1Time%3600;
    int min1 = timer1Time/60;
    timer1Time = timer1Time%60;
    int sec1 = timer1Time;
    
    int hour2 = timer2Time/3600;
    timer2Time = timer2Time%3600;
    int min2 = timer2Time/60;
    timer2Time = timer2Time%60;
    int sec2 = timer2Time;

    Serial.println("Timer:");
    Serial.print(hour1);
    Serial.print(":");
    Serial.print(min1);
    Serial.print(":");
    Serial.print(sec1);

    Serial.println(" ");
    Serial.println("Sub Timer:");
    Serial.print(hour2);
    Serial.print(":");
    Serial.print(min2);
    Serial.print(":");
    Serial.print(sec2);
    Serial.println(" ");

    char buffer[33];
    itoa (sec1,buffer,10);
    
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,buffer);  // write something to the internal memory
  u8g2.drawStr(75,10,"sec2");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(75);  

}
