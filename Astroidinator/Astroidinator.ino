//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

#include <LiquidCrystal_I2C.h>

//Constants
#define readX A0
#define readY A1
#define readS 2

//globals
LiquidCrystal_I2C GameLcd(0x3F, 20, 4); //Refer to the screen //Alternative 0x27
bool enableDebugLog = true;
bool pressed = false;
int gameState = 0; //0: Difficulty Select, 1: Name Select, 2: Ingame, 3: GameOver
int difficulty = 0; //0: unset, 1: little boy, 2: fat man, 3: tsar bomba
int playerPos = 1; //Possible positions: 0, 1, 2, 3

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

void InitPins()
{
  pinMode(readS, INPUT_PULLUP);
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
  if (enableDebugLog == true)
  {
    Serial.println(a_inputString);
  }
}

//LCD Startup Methods in order
void StartUp1()
{
  WriteToLcd(0, 0, "Jeremey vU, Peter J");
  WriteToLcd(0, 1, "Version 0");
  WriteToLcd(0, 2, "March 25 2019");
  delay(2000);
}

void StartUp2()
{
  WriteToLcd(0, 0, "NUKEinator", true);
  WriteToLcd(0, 1, "I am Become Death");
  WriteToLcd(0, 2, "Destroyer of Worlds");
  delay(2000);
}

//Setup
void setup() {
  InitSerial();
  InitCreator();
  InitGlobals();
  InitPins();
  InitGameLcd();
}

//Select Difficulty
void HandleJoystick()
{
  int xValue = analogRead(readX);
  int yValue = analogRead(readY);
  int sValue = digitalRead(readS);
  if (yValue > 1000)
  {
    playerPos--;
  }
  else if (yValue < 200)
  {
    playerPos++;
  }

  if (sValue == 0)
  {
    pressed = true;
  }
  else
  {
    pressed = false;
  }
}

void setDifficulty()
{
  //print lcd
  WriteToLcd(0, 0, "Select Difficulty", true);
  //TODO: Add Player Position
  switch (playerPos)
  {
    case (0):
      playerPos = 3;
    case (4):
      playerPos = 1;
    case (1):
      WriteToLcd(0, 1, "> Little Boy");
      WriteToLcd(0, 2, "  Fat Man");
      WriteToLcd(0, 3, "  Tsar Bomba");
      break;
    case (2):
      WriteToLcd(0, 1, "  Little Boy");
      WriteToLcd(0, 2, "> Fat Man");
      WriteToLcd(0, 3, "  Tsar Bomba");
      break;
    case (3):
      WriteToLcd(0, 1, "  Little Boy");
      WriteToLcd(0, 2, "  Fat Man");
      WriteToLcd(0, 3, "> Tsar Bomba");
      break;
  }

  if (pressed)
  {
    WriteToLcd(0,0, "Select Difficulty", true);
    
    switch (playerPos)
    {
      case (1):
        WriteToLcd(0, 1, "  Little Boy");
        break;
      case (2):
        WriteToLcd(0, 2, "  Fat Man");
        break;
      case (3):
        WriteToLcd(0, 3, "  Tsar Bomba");
        break;
    }
    gameState++; //Go to name select
    delay(2000);
  }
}

void SelectName()
{
  
}

//Loop
void loop() {
  switch (gameState)
  {
    case (0):
      setDifficulty();
      delay(1000);
      break;
    case (1):
      SelectName();
  }
  

  HandleJoystick();
}

