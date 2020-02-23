
const int button1Pin = 4;                                                    //Input pin(s) for buttons and HR monitor
const int button2Pin = 16;

void setup() {
  Serial.begin(115200);                                                          // Start serial communication for debugging purposes

//---------- Wait for Button to start timer-----------------------
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
}

void loop() {

int button1State = digitalRead(button1Pin);               //read the state of the timer button value:
//     Serial.println("button1State");
     Serial.println(button1State);

     delay(75);
 }
