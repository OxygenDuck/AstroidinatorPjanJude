//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

#include <LiquidCrystal_I2C.h>

//globals
LiquidCrystal_I2C GameLcd(0x3F, 20, 4); //Refer to the screen //Alternative 0x3F
bool enableDebugLog = true;

//Init Functions
void InitSerial()
{
  Serial.begin(115200);
  Serial.println("READY: serial initialized");
}

void InitCreator()
{
  Serial.println("Created: Jeremey van Uden and Peter Janssen, 25mar2019, v0");
}

void InitGlobals()
{
  Serial.println("INIT: globals");
  enableDebugLog = true;
  Serial.println("READY: globals initialized");
}

void InitGameLcd()
{
  GameLcd.init();
  GameLcd.setBacklight(1);
  GameLcd.clear();
  StartUp1();
  StartUp2();
}

//Write to lcd
void WriteToLcd(int a_x, int a_y, String a_text)
{
  int m_length;
  String m_singleLetter;

  GameLcd.setCursor(a_x, a_y);

  m_length = a_text.length();

  for (int m_index = 0; m_index < m_length; m_index++)
  {
    GameLcd.print(a_text[m_index]) ; 
  }
}

void WriteToLcd(int a_x, int a_y, String a_text, bool a_clear) //Overload with clear
{
  if (a_clear)
  {
    GameLcd.clear();  
  }
  WriteToLcd(a_x, a_y, a_text);
}

//Serial PrintLine Method
void PrintLn(String a_inputString)
{
  if(enableDebugLog == true)
  {
    Serial.println(a_inputString);
  }
}

//LCD Startup Methods in order
void StartUp1()
{
  WriteToLcd(0,0,"Jeremey vU, Peter J");
  WriteToLcd(0,1,"Version 0");
  WriteToLcd(0,2,"March 25 2019");
  delay(2000);
}

void StartUp2()
{
  WriteToLcd(0,0,"Astroidinator", true);
  WriteToLcd(0,1,"I am Become Death");
  WriteToLcd(0,2,"Destroyer of worlds");
  delay(2000);
}

void setup() {
  // put your setup code here, to run once:
    InitSerial();
    InitCreator();
    InitGlobals();
    InitGameLcd();
}

void loop() {
  // put your main code here, to run repeatedly:

}

