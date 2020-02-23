#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define INPUT_SIZE 11                           // Calculate based on max input size expected for one command

char input[INPUT_SIZE + 1];                     // Get next command from Serial (add 1 for final 0)
const byte numChars = 12;
char endMarker = '\n';
boolean newData = false;

BluetoothSerial SerialBT;

//----------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32BTtest"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
//----------------------------------------------------------------------------------

void loop() {

//--------------------READ COMMAND-----------------------------------

  recvWithStartEndMarkers();                    //receive serial input

  if (newData == true) {
      Serial.println("Message:");
      Serial.println(input[0]);
  }

//-------------------CLEAR VARIABLES--------------------------------------

  for (int i = 0; i <= INPUT_SIZE + 1; i++) {
    input[i] = '\0';
  }
  newData = false;

  delay(25); //not sure what this should be
}

//----------------------------------------------------------------------------------

void recvWithStartEndMarkers() {
  static byte ndx = 0;
  char rc;
  
  while (SerialBT.available() > 0) {
    rc = SerialBT.read();
    if (rc != endMarker) {
      input[ndx] = rc;
      ndx++;
      Serial.println("Okay still reading something...");
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
