
const int potPin = 34; //pin A0 to read analog input

//Variables:
int value; //save analog value


void setup(){
  Serial.begin(115200);
  //Input or output?
  pinMode(potPin, INPUT); //Optional 

}

void loop(){
  
  value = analogRead(potPin);          //Read and save analog value from potentiometer
  delay(10);                          //Small delay
  Serial.println(value);
}


                           
