int Sol_1_Ret = 12; //****Pins for relays
int Sol_1_Ext = 14;


//----------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  //set GPIO output pins here
  pinMode(Sol_1_Ret, OUTPUT);
  digitalWrite(Sol_1_Ret, HIGH);

  pinMode(Sol_1_Ext, OUTPUT);
  digitalWrite(Sol_1_Ext, HIGH);
}
//----------------------------------------------------------------------------------
void loop() {
  Serial.println("Opening Relay");
  digitalWrite(Sol_1_Ret, LOW);
  delay(1000);
  digitalWrite(Sol_1_Ret, HIGH);

  Serial.println("Opening Other Relay");
  digitalWrite(Sol_1_Ext, LOW);
  delay(1000);
  digitalWrite(Sol_1_Ext, HIGH);
  delay(1000); //not sure what this should be
}
