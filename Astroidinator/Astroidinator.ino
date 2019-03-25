//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

//globals
bool enableDebugLog = true;

void setup() {
  // put your setup code here, to run once:
    InitSerial();
    InitCreator();
    InitGlobals();
}

void InitSerial()
{
  Serial.begin(115200);
  Serial.println("READY: serial initialized");
}

void InitCreator()
{
  Serial.println("Created: Jeremey van Uden, 11feb2019, v0");
}

void InitGlobals()
{
  Serial.println("INIT: globals");
  enableDebugLog = true;
  Serial.println("READY: globals initialized");
}

void loop() {
  // put your main code here, to run repeatedly:

}
void PrintLn(String a_inputString)
{
  if(enableDebugLog == true)
  {
    Serial.println(a_inputString);
  }
}

