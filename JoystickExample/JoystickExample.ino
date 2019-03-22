#define readX A0
#define readY A1
#define readS 2

int xValue, yValue, sValue;

void setup() {
  //Start the serial monitor connection
  Serial.begin(9600);
  pinMode(readS, INPUT);
}

void loop() {
  //get the values from the joystick
  xValue = analogRead(readX);
  yValue = analogRead(readY);
  sValue = digitalRead(readS);

  //print the values in the serial monitor
  Serial.print("X: " + String(xValue));
  Serial.print("\t");
  Serial.print("Y: " + String(yValue));
  Serial.print("\t");
  Serial.print("S: " + String(sValue));
  Serial.print("\n");

  //delay(100);
}
