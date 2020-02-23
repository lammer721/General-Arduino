const int button1Pin = 18;                                                    //Input pin(s) for buttons and HR monitor

bool button1State;
float start_time;
float stop_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);                                                          // Start serial communication for debugging purposes
  Serial.println("The device started!");
  
  pinMode(button1Pin, INPUT_PULLUP);

 button1State = digitalRead(button1Pin);

Serial.println(button1State);
//while ( button1State == 1);{
// Serial.println("Waiting");

 //button1State = 1;

}

void loop() {
  
  int button1State = digitalRead(button1Pin); 
  Serial.println("loop1");
  Serial.println(button1Pin);
  Serial.println(button1State);

                                           // read the state of the timer button value:

  if (button1State == LOW) {    
    Serial.println("button pressed") ;                                                     // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  }

  // put your main code here, to run repeatedly:

} 
