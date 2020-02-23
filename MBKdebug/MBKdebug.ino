#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Calculate based on max input size expected for one command
#define INPUT_SIZE 11

// Get next command from Serial (add 1 for final 0)
char input[INPUT_SIZE + 1];
int cmd[3];                         //global cmd array for function
const byte numChars = 12;
char endMarker = '\n';

boolean newData = false;
BluetoothSerial SerialBT;

int Sol_1_Ret = 12; //****Pins for relays
int Sol_1_Ext = 14;
int Sol_2_Ret = 27;
int Sol_2_Ext = 26;

//----------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  //set GPIO output pins here
  pinMode(Sol_1_Ret, OUTPUT);
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
  //convert input into commandType, sol_number, and delayORposition
  recvWithStartEndMarkers();

  Serial.println("Input:");
  Serial.println(input);

  if (newData == true) {
    splitcommand(input);
  }

  int commandType = cmd[0];         //options are '0' for nothing or '1' for open loop or '2' for closed loop
  Serial.println("Command Type:");
  Serial.println(commandType);

  int sol_number = cmd[1];          //pre-defined pin number of the solenoid I care about
  Serial.println("Solenoid Number:");
  Serial.println(sol_number);

  int delayORposition = cmd[2];     //integer that states the delay time OR the desired position (once closed loop figured out)
  Serial.println("Delay:");
  Serial.println(delayORposition);

  //call function here: openloop() - simply open relay pin 'sol_number' for 'delayORposition' ms, then close relay pin.
  if (commandType == 1) {
    Serial.println("Opening Solenoid Number:");
    Serial.println(sol_number);
    digitalWrite(sol_number, LOW);
    delay(delayORposition);
    digitalWrite(sol_number, HIGH);
  }

  //Set the cmd to '0' so the cmd doesn't continue to execute
  commandType = 0;
  sol_number = 0;
  delayORposition = 0;

  cmd[0] = 0;
  cmd[1] = 0;
  cmd[2] = 0;

  for (int i = 0; i <= INPUT_SIZE + 1; i++) {
    input[i] = '\0';
  }
  newData = false;

  delay(1000); //not sure what this should be
}

//----------------------------------------------------------------------------------
void splitcommand(char* input) {

  // Add the final 0 to end the C string
  input[INPUT_SIZE] = 0;                     //I think i can delete this too since it's jsut a for loop

  // Read each command pair
  char* token = strtok(input, " ");          //returns first token (first part of command) to 'token'
  if (input[0] != '\n') {
    for (int i = 1; i <= 3; i++) {            //I think i can delete the while loop and just do this...

      if (i == 1) {
        cmd[0] = atoi(token);          //sends first 'token' to commandType
      }

      if (i == 2) {
        cmd[1] = atoi(token) ;           //sends 2nd 'token' to servoID
      }

      if (i == 3) {
        cmd[2] = atoi(token) ;            //you get it
      }
      // Find the next command in input string
      token = strtok(0, " ");
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
      //input[ndx] = '0'; // terminate the string         //** how does this affect the cmd type (delayORposition)
      ndx = 0;
      newData = true;
    }
  }
}
