const int xInput = 13;
const int yInput = 12;
const int zInput = 14;
 
// Raw Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xRawMin = 512;
int xRawMax = 512;
 
int yRawMin = 512;
int yRawMax = 512;
 
int zRawMin = 512;
int zRawMax = 512;
 
// Take multiple samples to reduce noise
const int sampleSize = 10;


void setup() {
   // Start serial communication for debugging purposes
  Serial.begin(115200);
}

void loop() {
  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);

  AutoCalibrate(xRaw, yRaw, zRaw);
    
  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
  long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
  long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);

  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;
  
  Serial.print(xAccel);
  Serial.print(",");
  Serial.print(yAccel);
  Serial.print(",");
  Serial.print(zAccel);
  Serial.print("\n");
  
  delay(50);
}

//
// Read "sampleSize" samples and report the average
//
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}
 
//
// Find the extreme raw readings from each axis
//
void AutoCalibrate(int xRaw, int yRaw, int zRaw)
{
  if (xRaw < xRawMin)
  {
    xRawMin = xRaw;
  }
  if (xRaw > xRawMax)
  {
    xRawMax = xRaw;
  }
  
  if (yRaw < yRawMin)
  {
    yRawMin = yRaw;
  }
  if (yRaw > yRawMax)
  {
    yRawMax = yRaw;
  }
 
  if (zRaw < zRawMin)
  {
    zRawMin = zRaw;
  }
  if (zRaw > zRawMax)
  {
    zRawMax = zRaw;
  }
  
//  Serial.print("Raw Ranges: X: ");
//  Serial.print(xRawMin);
//  Serial.print("-");
//  Serial.print(xRawMax);
  
//  Serial.print(", Y: ");
//  Serial.print(yRawMin);
//  Serial.print("-");
//  Serial.print(yRawMax);
  
//  Serial.print(", Z: ");
//  Serial.print(zRawMin);
//  Serial.print("-");
//  Serial.print(zRawMax);
//  Serial.println();
//  Serial.print(xRaw);
//  Serial.print(", ");
//  Serial.print(yRaw);
//  Serial.print(", ");
//  Serial.print(zRaw);
}
