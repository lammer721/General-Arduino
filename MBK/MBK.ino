#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define INPUT_SIZE 11                           // Calculate based on max input size expected for one command

char input[INPUT_SIZE + 1];                     // Get next command from Serial (add 1 for final 0)
int cmd[3] = {0, 0, 0};                         //global cmd array for function
const byte numChars = 12;
char endMarker = '\n';

boolean newData = false;
BluetoothSerial SerialBT;

int Sol_1_Ret = 12; //****Pins for relays
int Sol_1_Ext = 14;
int Sol_2_Ret = 27;
int Sol_2_Ext = 26;
int potPin = 24; // pins for potentiometer

float margin = 40;

//----------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(Sol_1_Ret, OUTPUT);                           //set GPIO output pins here
  digitalWrite(Sol_1_Ret, HIGH);

  pinMode(Sol_1_Ext, OUTPUT);
  digitalWrite(Sol_1_Ext, HIGH);

  pinMode(Sol_2_Ret, OUTPUT);
  digitalWrite(Sol_2_Ret, HIGH);

  pinMode(Sol_2_Ext, OUTPUT);
  digitalWrite(Sol_2_Ext, HIGH);
}
//----------------------------------------------------------------------------------

void loop() {

int commandType;
int sol_number;
int delayORposition;
//--------------------READ COMMAND-----------------------------------

  recvWithStartEndMarkers();                    //receive serial input

  if (newData == true) {
      splitcommand(input);

      commandType = cmd[0];         //options are '0' for nothing or '1' for open loop or '2' for closed loop
      Serial.println("Command Type:");
      Serial.println(commandType);

      sol_number = cmd[1];          //pre-defined pin number of the solenoid I care about OR 99 = TILT and 98 = ROLL
      Serial.println("Solenoid Number:");
      Serial.println(sol_number);

      delayORposition = cmd[2];     //integer that states the delay time OR the desired position (once closed loop figured out)
      Serial.println("Delay/Position:");
      Serial.println(delayORposition);
  }

//-------------------SEND OPEN LOOP MOTION COMMANDS--------------------------------------

  if (commandType == 1) {                               //call function here: openloop() - simply open relay pin 'sol_number' for 'delayORposition' ms, then close relay pin.
    Serial.println("Opening Solenoid Number:");
    Serial.println(sol_number);
    digitalWrite(sol_number, LOW);
    delay(delayORposition);
    digitalWrite(sol_number, HIGH);

    commandType = 0;                                        //Set the cmd to '0' so the cmd doesn't continue to execute
    sol_number = 0;
    delayORposition = 0;

    cmd[0] = 0;
    cmd[1] = 0;
    cmd[2] = 0;
  }


//-------------------SEND CLOSED LOOP MOTION COMMANDS--------------------------------------

  if (commandType == 2) {
    float realPos = analogRead(potPin);
    Serial.println(realPos);

    if (sol_number = 99){                                                                    //TILT command
        if (realPos < delayORposition + margin && realPos > delayORposition - margin){       //if actual position is close enough to goal position,
                digitalWrite(Sol_1_Ret, HIGH);
                digitalWrite(Sol_1_Ext, HIGH);
                //commandType = 0;          This prevents the CL code from re-executing (i.e. system won't maintain position)
        }
        if (realPos > delayORposition + margin) {                                            //if actual position is greater than goal position plus margin
                digitalWrite(Sol_1_Ret, LOW);
        }
        if (realPos < delayORposition - margin){                                             //if actual position is less than goal position - margin
                digitalWrite(Sol_1_Ext, LOW);
        }
    }
    if (sol_number == 98){                                                                    //ROLL command
        if (realPos < delayORposition + margin && realPos > delayORposition - margin){       //if actual position is close enough to goal position,
                digitalWrite(Sol_2_Ret, HIGH);                                              //reset command
                digitalWrite(Sol_2_Ext, HIGH);
                //commandType = 0;
        }
        if (realPos > delayORposition + margin) {                                            //if actual position is greater than goal position plus margin
                digitalWrite(Sol_2_Ret, LOW);
        }
        if (realPos < delayORposition - margin){                                             //if actual position is less than goal position - margin
                digitalWrite(Sol_2_Ext, LOW);
        }
    }
  }

//-------------------CLEAR VARIABLES--------------------------------------

  for (int i = 0; i <= INPUT_SIZE + 1; i++) {
    input[i] = '\0';
  }
  newData = false;

  delay(25); //not sure what this should be
}

//----------------------------------------------------------------------------------
void splitcommand(char* input) {

  input[INPUT_SIZE] = 0;                      // Add the final 0 to end the C string......I think i can delete this too since it's jsut a for loop

  char* token = strtok(input, " ");          // Read each command pair....returns first token (first part of command) to 'token'
  if (input[0] != '\n') {
    for (int i = 1; i <= 3; i++) {            //I think i can delete the while loop and just do this...

      if (i == 1) {cmd[0] = atoi(token);};    //sends first 'token' to commandType

      if (i == 2) {cmd[1] = atoi(token);} ;  //sends 2nd 'token' to servoID

      if (i == 3) {cmd[2] = atoi(token);} ;  //you get it

      token = strtok(0, " ");               // Find the next command in input string
    }
  }
}

void recvWithStartEndMarkers() {
  static byte ndx = 0;
  char rc;

  while (SerialBT.available() > 0 && newData == false) {
    rc = SerialBT.read();

    if (rc != endMarker) {
      input[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {              //is this necessary?
        ndx = numChars - 1;
      }
    }
    else {
      ndx = 0;
      newData = true;
    }
  }
}


/*void open(){
    digitalWrite(sol_number, LOW);
    delay(delayORposition);
    digitalWrite(sol_number, HIGH);
    Serial.println("Open move complete!");
    }

void closed(){
                                                                                                //write now this is configured for 1 axis, with multiple need to alternate checks and actuation, so it occurs at the same time
    int currentPos = analogRead(wherever the pot is)
    if (delayORposition > currentPos*1.1){
        while (delayORposition > currentPos*1.1 || delayORposition < currentPos*0.9){           //something like this, def need to check this logic
            digitalWrite(sol_1_Ext, LOW)
            currentPos = analogRead(wherever the pot is)
        }
            digitalWrite(sol_1_Ext, HIGH);

    if (delayORposition < currentPos*0.9){
        while (delayORposition < currentPos*0.9 || delayORposition > currentPos*1.1){           //something like this, def need to check this logic
            digitalWrite(sol_1_Ret, LOW)
            currentPos = analogRead(wherever the pot is)
        }
        digitalWrite(sol_1_Ret, HIGH);

}
*/
