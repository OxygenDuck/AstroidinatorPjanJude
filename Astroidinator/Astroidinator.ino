

//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

//Libraries
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
//objects
SimpleTimer timer;
//Constants
#define readX A0
#define readY A1
#define readS 2

//structs
struct bomb {
  int currentpositionY = 0;
  int currentpositionX;
  bool beenShot = false;
  bool blownUp = false;
};

typedef struct
{
  int x; //= random(0 ,20); ||Set Upon creation later
  int y; //= random(0, 3);  ||Set Upon creation later
  bool destroyed = false;
  bool exist = false;
} Asteroid;

//globals
LiquidCrystal_I2C GameLcd(0x3F, 20, 4); //Refer to the screen //Alternative 0x27
bool enableDebugLog = true;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool pressed = false;
int gameState = 0; //0: Difficulty Select, 1: Name Select, 2: Ingame, 3: GameOver
int difficulty = 0; //0: unset, 1: little boy, 2: fat man, 3: tsar bomba
int playerPos = 0; //Possible positions: 0, 1, 2, 3
int chooseNameIndex = 0; //0: 1st letter - 3: 4th letter
int AstIndex = 1;
int bombCounter = 0;
String characters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", " "};
String playerName[4] = {"A", "A", "A", "A"};
Asteroid asteroids[20];
bomb bombs[20];

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
  randomSeed(42);
}

//Select Difficulty
void HandleJoystick()
{
  int xValue = analogRead(readX);
  int yValue = analogRead(readY);
  int sValue = digitalRead(readS);

  //Set all inputs to false
  up = false;
  down = false;
  left = false;
  right = false;
  pressed = false;

  //Check input
  if (xValue > 1000)
  {
    right = true;
  }
  else if (xValue < 200)
  {
    left = true;
  }

  if (yValue > 1000)
  {
    up = true;
  }
  else if (yValue < 200)
  {
    down = true;
  }

  if (sValue == 0)
  {
    pressed = true;
  }
}

void playerPosition(int m_max_position)
{
  if (up)
  {
    playerPos--;
    if (playerPos == -1)
    {
      playerPos = m_max_position - 1;
    }
  }
  else if (down)
  {
    playerPos++;
    if (playerPos == m_max_position)
    {
      playerPos = 0;
    }
  }
}
void setDifficulty()
{
  //print lcd
  WriteToLcd(0, 0, "Select Difficulty", true);

  //set playerPos
  playerPosition(3);

  switch (playerPos)
  {
    case (0):
      WriteToLcd(0, 1, "> Little Boy");
      WriteToLcd(0, 2, "  Fat Man");
      WriteToLcd(0, 3, "  Tsar Bomba");
      break;
    case (1):
      WriteToLcd(0, 1, "  Little Boy");
      WriteToLcd(0, 2, "> Fat Man");
      WriteToLcd(0, 3, "  Tsar Bomba");
      break;
    case (2):
      WriteToLcd(0, 1, "  Little Boy");
      WriteToLcd(0, 2, "  Fat Man");
      WriteToLcd(0, 3, "> Tsar Bomba");
      break;
  }

  if (pressed)
  {
    checkIfPressed();
  }
}

void checkIfPressed()
{
  difficulty = playerPos + 1; //Set difficulty to selected index

  WriteToLcd(0, 0, "Select Difficulty", true);

  switch (difficulty)
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
  PrintLn("Difficulty has been set");
  playerPos = 0;
}
void SelectName()
{
  int maxPos = 26;

  WriteToLcd(0, 0, "Select Name", true);

  //set playerPos and chooseNameIndex
  if (pressed or right)
  {
    chooseNameIndex++;
  }
  else if (left and chooseNameIndex != 0)
  {
    chooseNameIndex--;
  }

  for (int i = 0; i < maxPos; i++) //Set player pos to the currently selected letter
  {
    if (playerName[chooseNameIndex] == characters[i])
    {
      playerPos = i;
      break;
    }
  }

  if (up)
  {
    playerPos--;
  }
  else if (down)
  {
    playerPos++;
  }

  //Check if playerPos is outside the bounds of characters[]
  if (playerPos > maxPos)
  {
    playerPos = 0;
  }
  else if (playerPos == -1)
  {
    playerPos = maxPos;
  }

  playerName[chooseNameIndex] = characters[playerPos]; //Set the selected character

  //Write Name
  WriteToLcd(2, 2, playerName[0]);
  WriteToLcd(3, 2, playerName[1]);
  WriteToLcd(4, 2, playerName[2]);
  WriteToLcd(5, 2, playerName[3]);
  //Current position

  if (chooseNameIndex < 4)
  {
    WriteToLcd(2 + chooseNameIndex, 3, "^");
  }
  else
  {
    gameState++; //Go to game
    PrintLn("Player Name has been set");
    playerPos = 0;
    GameLcd.clear();
  }
}
void ShipMovement()
{
  playerPosition(4);
  switch (playerPos)
  {
    case (0):
      WriteToLcd(0, 0, ">");
      break;
    case (1):
      WriteToLcd(0, 1, ">");
      break;
    case (2):
      WriteToLcd(0, 2, ">");
      break;
    case (3):
      WriteToLcd(0, 3, ">");
      break;
  }
}

void BombMovement()
{
  if  (bombCounter < 21)
  {
    if (pressed)
    {
      bombs[bombCounter].beenShot = true;
      bombs[bombCounter].currentpositionX = playerPos;
      bombCounter++;
      PrintLn("Bomb Created");
    }

    for (int i = 0; i < bombCounter; i++) {
      if (bombs[i].currentpositionY > 18)
      {
        bombs[i].currentpositionY = 1;
        bombs[i].currentpositionX = 1;
        bombs[i].beenShot = false;
      }
      else if (bombs[i].beenShot == true)
      {
        bombs[i].currentpositionY = bombs[i].currentpositionY + 1;
        WriteToLcd(bombs[i].currentpositionY, bombs[i].currentpositionX, "=");
      }
    }
  }
}
void checkGameState()
{
  switch (gameState)
  {
    case (0):
      setDifficulty();
      break;
    case (1):
      SelectName();
      break;
    case (2):
      GameLcd.clear();
      ShipMovement();
      AsteroidMovement();
      BombMovement();
      checkCollision();
      break;
    case (3):
      GameLcd.clear();
      WriteToLcd(0, 0, "You lose !!!!");
      break;
  }
  HandleJoystick();
}
//Loop
void loop() {
  timer.run();
}
void checkCollision()
{
  for (int i = 0; i < 20; i++)
  {
    if (asteroids[i].exist)
    {
      for (int x = 0; x < bombCounter; x++) {
        if (bombs[x].beenShot == true)
        {
          if (bombs[x].currentpositionY == asteroids[i].x && bombs[x].currentpositionX == asteroids[i].y)
          {
            bombs[x].beenShot = false;
            asteroids[i].destroyed = true;
            WriteToLcd(asteroids[i].x, asteroids[i].y, "#");
            PrintLn("Asteroid destroyed");
          }
          else if (bombs[x].currentpositionY == asteroids[i].x - 1 && bombs[x].currentpositionX == asteroids[i].y)
          {
            bombs[x].beenShot = false;
            asteroids[i].destroyed = true;
            WriteToLcd(asteroids[i].x, asteroids[i].y, "#");
            WriteToLcd(bombs[x].currentpositionY, bombs[x].currentpositionX, " ");
            PrintLn("Asteroid destroyed");
          }
        }
      }
      if (asteroids[i].x == 0 && asteroids[i].y == playerPos)
      {
        gameState++;
        PrintLn("Player destroyed");
        WriteToLcd(asteroids[i].x, asteroids[i].y, "#");
      }
    }
  }
}

void AsteroidMovement()
{

  //Move existing asteroids
  for (int i = 0; i < 20; i++)
  {
    if (asteroids[i].exist)
    {
      if (asteroids[i].destroyed == false && asteroids[i].x > 0)
      {
        asteroids[i].x--;
        WriteToLcd(asteroids[i].x, asteroids[i].y, "*");
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
      PrintLn("create new asteroid");
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
