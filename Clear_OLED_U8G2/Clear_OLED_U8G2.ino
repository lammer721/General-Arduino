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
 
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"sec1");  // write something to the internal memory
  u8g2.drawStr(70,10,"sec2");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(75);  

}
