//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

//Libraries
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>

//objects
typedef struct
{
  int x; //= random(0 ,20); ||Set Upon creation later
  int y; //= random(0, 3);  ||Set Upon creation later
  bool destroyed = false;
  bool exist = false;
} Asteroid;

SimpleTimer timer;

//Constants
#define readX A0
#define readY A1
#define readS 2

//globals
LiquidCrystal_I2C GameLcd(0x3F, 20, 4); //Refer to the main screen //Alternative 0x27
LiquidCrystal_I2C SecondLcd(0x3B, 20, 4); //Refer to the sub screen
bool enableDebugLog = true;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool pressed = false;
int gameState = 2; //0: Difficulty Select, 1: Name Select, 2: Ingame, 3: GameOver
int difficulty = 0; //0: unset, 1: little boy, 2: fat man, 3: tsar bomba
int playerPos = 0; //Possible positions: 0, 1, 2, 3
int chooseNameIndex = 0; //0: 1st letter - 3: 4th letter
int AstIndex = 1; 
String characters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", " "};
String playerName[4] = {"A", "A", "A", "A"};
Asteroid asteroids[20];

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
  timer.setInterval(1000, checkGameState);
  Serial.println("READY: globals initialized");
}

void InitGameLcd()
{
  GameLcd.init();
  GameLcd.setBacklight(1);
  GameLcd.clear();
  SecondLcd.init();
  SecondLcd.setBacklight(1);
  SecondLcd.clear();
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

void WriteToSecond(int a_x, int a_y, String a_text)
{
  int m_length;
  String m_singleLetter;

  SecondLcd.setCursor(a_x, a_y);

  m_length = a_text.length();

  for (int m_index = 0; m_index < m_length; m_index++)
  {
    SecondLcd.print(a_text[m_index]) ;
  }
}

void WriteToSecond(int a_x, int a_y, String a_text, bool a_clear) //Overload with clear
{
  if (a_clear)
  {
    SecondLcd.clear();
  }
  WriteToSecond(a_x, a_y, a_text);
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
  WriteToSecond(0, 0, "I AM SECOND", true);
  delay(2000);
}

void StartUp2()
{
  WriteToLcd(0, 0, "NUKEinator", true);
  WriteToLcd(0, 1, "I am Become Death");
  WriteToLcd(0, 2, "Destroyer of Worlds");
  WriteToSecond(0, 0, "I NEED", true);
  WriteToSecond(0, 1, "INFORMATION");
  delay(2000);
}

//Setup
void setup() {
  InitSerial();
  InitCreator();
  InitGlobals();
  InitPins();
  InitGameLcd();
  randomSeed(42);
}

void AsteroidTest()
{
  GameLcd.clear();
  
  //Move existing asteroids
  for (int i = 0; i < 20; i++)
  {
    if (asteroids[i].exist)
    {
      Serial.println(i);
      if (asteroids[i].destroyed == false && asteroids[i].x > 0)
      {
        asteroids[i].x--;
        WriteToLcd(asteroids[i].x,asteroids[i].y,"*");
      }
      else if (asteroids[i].x == 0)
      {
        asteroids[i].destroyed = true;
      }
      if (asteroids[i].destroyed)
      {
        asteroids[i].exist = false;
      }
    }
  }

  //Create new asteroid
  for (int i = 0; i < 20; i++)
  {
    if (asteroids[i].exist == false)
    {
      Serial.println("create new asteroid");
      asteroids[i].x = 20;
      asteroids[i].y = random(0, 4);
      asteroids[i].destroyed = false;
      asteroids[i].exist = true;
      break;
    }
  }
  
  /*
  for (int i = 0; i < AstIndex; i++)
  {
    
    Serial.println(String(asteroids[i].x));
    
    asteroids[i].x--;
    Serial.println("hey");

    WriteToLcd(asteroids[i].x, asteroids[i].y, "*");
    
    if (current.x < 0)
    {
      current.destroyed = true;  
    }
    else if (current.x < 21)
    {
       WriteToLcd(current.x, current.y, "*");
       /*Serial.print(String(i) + "\t");
       Serial.print(String(current.x) + "\t");
       Serial.println(String(current.y));
    }
    
  }*/

  //Spawn a new asteroid
  /*Asteroid add{
      20,
      random(3),
      false
  };

  asteroids[AstIndex] = add;*/
  /*AstIndex++;*/
}

void checkGameState()
{
  switch (gameState)
  {
    case (0):
      //setDifficulty();
      break;
    case (1):
      //SelectName();
      break;
    case (2):
      AsteroidTest();
  }
  //HandleJoystick();
}

//Loop
void loop() {
  timer.run();
}

