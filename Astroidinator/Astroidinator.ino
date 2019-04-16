

//Created: Jeremey van Uden, Peter Janssen, 25Maart2019, v0
//Version
//v0, 11feb2019: initial version

//Libraries
#include <LiquidCrystal_I2C.h>
#include <SimpleTimer.h>
#include "music.h"
//objects
SimpleTimer timerJude;
//Constants
#define readX A0
#define readY A1
#define readS 2

//structs
struct bombJude {
  int currentpositionYJude = 0;
  int currentpositionXJude;
  bool beenShotJude = false;
  bool blownUpJude = false;
};

typedef struct
{
  int xPjan; //= random(0 ,20); ||Set Upon creation later
  int yPjan; //= random(0, 3);  ||Set Upon creation later
  bool destroyedPjan = false;
  bool existPjan = false;
} AsteroidPjan;

//globals
LiquidCrystal_I2C GameLcd(0x3F, 20, 4); //Refer to the screen //Alternative 0x27
LiquidCrystal_I2C SecondLcd(0x3B, 20, 4); //Refer to the sub screen
bool enableDebugLogJude = true;
bool upPjan = false;
bool downPjan = false;
bool leftPjan = false;
bool rightPjan = false;
bool pressedPjan = false;
bool shotLastFramePjan = false;
int gameStatePjan = 0; //0: Difficulty Select, 1: Name Select, 2: Ingame, 3: GameOver
int difficultyPjan = 0; //0: unset, 1: little boy, 2: fat man, 3: tsar bomba
int playerPosPjan = 0; //Possible positions: 0, 1, 2, 3
int chooseNameIndexPjan = 0; //0: 1st letter - 3: 4th letter
int scorePjan = 0;
int spawnCounterPjan = 4; //counter for spawning asteroids
int bombCounterJude = 0;
String charactersPjan[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", " "};
String playerNamePjan[4] = {"A", "A", "A", "A"};
AsteroidPjan asteroidsPjan[10];
bombJude bombsJude[20];

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
  enableDebugLogJude = true;
  timerJude.setInterval(500, CheckGameState);
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
  if (enableDebugLogJude == true)
  {
    Serial.println(a_inputString);
  }
}

//LCD Startup Methods in order
//show creator and version
void StartUp1()
{
  WriteToLcd(0, 0, "Jeremey vU, Peter J");
  WriteToLcd(0, 1, "Version 0");
  WriteToLcd(0, 2, "March 25 2019");
  PrintLn("Creater and Version screen shown");
  delay(2000);
}

//Show catchy title and quote on lcd
void StartUp2()
{
  WriteToLcd(0, 0, "NUKEinator", true);
  WriteToLcd(0, 1, "I am Become Death");
  WriteToLcd(0, 2, "Destroyer of Worlds");
  PrintLn("Catchy title shown");
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

//Handle joystick inputs
void HandleJoystick()
{
  int m_xValue = analogRead(readX);
  int m_yValue = analogRead(readY);
  int m_sValue = digitalRead(readS);

  //Set all inputs to false
  upPjan = false;
  downPjan = false;
  leftPjan = false;
  rightPjan = false;
  pressedPjan = false;

  //Check input
  if (m_xValue > 1000)
  {
    rightPjan = true;
  }
  else if (m_xValue < 200)
  {
    leftPjan = true;
  }

  if (m_yValue > 1000)
  {
    upPjan = true;
  }
  else if (m_yValue < 200)
  {
    downPjan = true;
  }

  if (m_sValue == 0)
  {
    pressedPjan = true;
  }
}

//Handle the player's position according to HandleJoystick()
void PlayerPosition(int m_max_position)
{
  if (upPjan)
  {
    playerPosPjan--;
    if (playerPosPjan == -1)
    {
      playerPosPjan = m_max_position - 1;
    }
  }
  else if (downPjan)
  {
    playerPosPjan++;
    if (playerPosPjan == m_max_position)
    {
      playerPosPjan = 0;
    }
  }
}

//Loop
void loop() {
  //Run the timer with a 0.5 second interval
  timerJude.run();
}

//Check at which state the game is and perform the necessary actions
void CheckGameState()
{
  switch (gameStatePjan)
  {
    case (0):
      SetDifficulty();
      break;
    case (1):
      SelectName();
      break;
    case (2):
      GameLcd.clear();
      ShipMovement();
      AsteroidMovement();
      BombMovement();
      CheckCollision();
      WriteInfoScreen();
      break;
    case (3):
      GameOver();
      break;
    case (4):
      ResetGame();
      gameStatePjan = 0;
      midiGameOverPjan();
      break;
  }
  HandleJoystick();
}

//Set difficulty from given options
void SetDifficulty()
{
  //print lcd
  GameLcd.clear();
  WriteToLcd(0, 0, "Select Difficulty");
  //set playerPos
  PlayerPosition(3);

  switch (playerPosPjan)
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

  if (pressedPjan)
  {
    CheckIfPressed();
  }
}

//Check weither the button is pressed when selecting difficulty
void CheckIfPressed()
{
  difficultyPjan = playerPosPjan + 1; //Set difficulty to selected index

  WriteToLcd(0, 0, "Select Difficulty", true);

  switch (difficultyPjan)
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
  gameStatePjan++; //Go to name select
  PrintLn("Difficulty has been set");
  playerPosPjan = 0;
}

//Select the players name by looping through an alphabetic array
void SelectName()
{
  int m_maxPos = 26;

  WriteToLcd(0, 0, "Select Name", true);

  //set playerPos and chooseNameIndexPjan
  if (pressedPjan or rightPjan)
  {
    chooseNameIndexPjan++;
  }
  else if (leftPjan and chooseNameIndexPjan != 0)
  {
    chooseNameIndexPjan--;
  }

  for (int i = 0; i < m_maxPos; i++) //Set player pos to the currently selected letter
  {
    if (playerNamePjan[chooseNameIndexPjan] == charactersPjan[i])
    {
      playerPosPjan = i;
      break;
    }
  }

  if (upPjan)
  {
    playerPosPjan--;
  }
  else if (downPjan)
  {
    playerPosPjan++;
  }

  //Check if playerPos is outside the bounds of characters[]
  if (playerPosPjan > m_maxPos)
  {
    playerPosPjan = 0;
  }
  else if (playerPosPjan == -1)
  {
    playerPosPjan = m_maxPos;
  }

  playerNamePjan[chooseNameIndexPjan] = charactersPjan[playerPosPjan]; //Set the selected character

  //Write Name
  WriteToLcd(2, 2, playerNamePjan[0]);
  WriteToLcd(3, 2, playerNamePjan[1]);
  WriteToLcd(4, 2, playerNamePjan[2]);
  WriteToLcd(5, 2, playerNamePjan[3]);
  //Current position

  if (chooseNameIndexPjan < 4)
  {
    WriteToLcd(2 + chooseNameIndexPjan, 3, "^");
  }
  else
  {
    gameStatePjan++; //Go to game
    PrintLn("Player Name has been set");
    playerPosPjan = 0;
    GameLcd.clear();
  }
}

//move the ship
void ShipMovement()
{
  PlayerPosition(4);
  WriteToLcd(0, playerPosPjan, ">");
}

//move the asteroids and create new ones
void AsteroidMovement()
{
  bool m_endGame = true;

  //Move existing asteroids
  for (int i = 0; i < 10; i++)
  {
    if (asteroidsPjan[i].existPjan)
    {
      if (asteroidsPjan[i].destroyedPjan == false && asteroidsPjan[i].xPjan > 0)
      {
        asteroidsPjan[i].xPjan--;
        WriteToLcd(asteroidsPjan[i].xPjan, asteroidsPjan[i].yPjan, "*");
      }
      else if (asteroidsPjan[i].xPjan == 0)
      {
        asteroidsPjan[i].destroyedPjan = true;
      }
      /*if (asteroids[i].destroyed)
        {
        asteroids[i].exist = false;
        }*/
    }
    if (asteroidsPjan[i].destroyedPjan == false)
    {
      m_endGame = false;
    }
  }

  //Create new asteroid
  int m_spawnCountMax = 4 - difficultyPjan;
  PrintLn("SpawnMax:");
  PrintLn(String(m_spawnCountMax));
  if (spawnCounterPjan > m_spawnCountMax)
  {
    spawnCounterPjan = m_spawnCountMax;
  }

  spawnCounterPjan--;

  PrintLn(String(spawnCounterPjan));

  if (spawnCounterPjan == 0)
  {
    for (int i = 0; i < 10; i++)
    {
      if (asteroidsPjan[i].existPjan == false)
      {
        PrintLn("create new asteroid");
        asteroidsPjan[i].xPjan = 20;
        asteroidsPjan[i].yPjan = random(0, 4);
        asteroidsPjan[i].destroyedPjan = false;
        asteroidsPjan[i].existPjan = true;
        break;
      }
    }
    spawnCounterPjan = 4;
  }

  if (m_endGame)
  {
    gameStatePjan++;
  }
}

//Move bombs and shoot new ones
void BombMovement()
{
  if  (bombCounterJude < 21)
  {
    if (pressedPjan && !shotLastFramePjan)
    {
      bombsJude[bombCounterJude].beenShotJude = true;
      bombsJude[bombCounterJude].currentpositionXJude = playerPosPjan;
      bombCounterJude++;
      PrintLn("Bomb Created");
      shotLastFramePjan = true;
      ShootSoundPjan();
    }
    else if (shotLastFramePjan)
    {
      shotLastFramePjan = false;
    }
    for (int i = 0; i < bombCounterJude; i++) {
      if (bombsJude[i].beenShotJude == true && bombsJude[i].blownUpJude == false)
      {
        if(bombsJude[i].currentpositionYJude < 19)
        {
          bombsJude[i].currentpositionYJude = bombsJude[i].currentpositionYJude + 1;
          WriteToLcd(bombsJude[i].currentpositionYJude, bombsJude[i].currentpositionXJude, "=");
        }
        else
        {
          bombsJude[i].blownUpJude = true;
          scorePjan--;
        }
      }
    }
  }
}

//Checks for collision between the player, bombs and asteroids
void CheckCollision()
{
  for (int i = 0; i < 20; i++)
  {
    if (asteroidsPjan[i].existPjan && asteroidsPjan[i].destroyedPjan == false)
    {
      for (int x = 0; x < bombCounterJude; x++) {
        if (bombsJude[x].beenShotJude == true && bombsJude[x].blownUpJude == false)
        {
          if (bombsJude[x].currentpositionYJude == asteroidsPjan[i].xPjan && bombsJude[x].currentpositionXJude == asteroidsPjan[i].yPjan)
          {
            asteroidsPjan[i].destroyedPjan = true;
            bombsJude[x].blownUpJude = true;
            WriteToLcd(asteroidsPjan[i].xPjan, asteroidsPjan[i].yPjan, "#");
            PrintLn("Asteroid destroyed");
            scorePjan++;
            BoomSoundPjan();
          }
          else if (bombsJude[x].currentpositionYJude == asteroidsPjan[i].xPjan - 1 && bombsJude[x].currentpositionXJude == asteroidsPjan[i].yPjan)
          {
            asteroidsPjan[i].destroyedPjan = true;
            bombsJude[x].blownUpJude = true;
            WriteToLcd(asteroidsPjan[i].xPjan, asteroidsPjan[i].yPjan, "#");
            WriteToLcd(bombsJude[x].currentpositionYJude, bombsJude[x].currentpositionXJude, " ");
            PrintLn("Asteroid destroyed");
            scorePjan++;
            BoomSoundPjan();
          }
        }
      }
      if (asteroidsPjan[i].xPjan == 0 && asteroidsPjan[i].yPjan == playerPosPjan && asteroidsPjan[i].destroyedPjan == false)
      {
        gameStatePjan++;
        PrintLn("Player destroyed");
        WriteToLcd(asteroidsPjan[i].xPjan, asteroidsPjan[i].yPjan, "#");
        scorePjan = scorePjan - 2;
        BoomSoundPjan();
      }
    }
  }
}

//Write info to the second screen
void WriteInfoScreen()
{
  //Write name
  WriteToSecond(0, 0, playerNamePjan[0], true);
  WriteToSecond(1, 0, playerNamePjan[1]);
  WriteToSecond(2, 0, playerNamePjan[2]);
  WriteToSecond(3, 0, playerNamePjan[3]);
  //Write Difficulty
  switch (difficultyPjan)
  {
    case 1: WriteToSecond(5, 0, "Little Boy"); break;
    case 2: WriteToSecond(5, 0, "Fat Man"); break;
    case 3: WriteToSecond(5, 0, "Tsar Bomba"); break;
  }
  //Write Bullets
  int m_bombsLeft = 0;
  for (int i = 0; i < 20; i++)
  {
    if (bombsJude[i].beenShotJude == false)
    {
      m_bombsLeft++;
    }
  }

  WriteToSecond(0, 1, "Bombs left: " + String(m_bombsLeft));

  WriteToSecond(0, 2, "Score: " + String(scorePjan));
}

//Gameover handling
void GameOver()
{
  SecondLcd.clear();
  GameLcd.clear();
  WriteToLcd(0, 0, "GAME OVER");
  WriteToLcd(0, 1, playerNamePjan[0] + playerNamePjan[1] + playerNamePjan[2] + playerNamePjan[3]);
  WriteToLcd(0, 2, "Your Final score: ");
  WriteToLcd(18, 2, String(scorePjan));
  WriteToLcd(0, 3, "Level: ");
  switch (difficultyPjan)
  {
    case (1): WriteToLcd(8, 3, "Little Boy"); break;
    case (2): WriteToLcd(8, 3, "Fat Man"); break;
    case (3): WriteToLcd(8, 3, "Tsar Bomba"); break;
  }
  PrintLn("Game over!");
  gameStatePjan++;
}

//Resets all variables and structs.
void ResetGame()
{
  enableDebugLogJude = true;
  upPjan = false;
  downPjan = false;
  leftPjan = false;
  rightPjan = false;
  pressedPjan = false;
  shotLastFramePjan = false;
  gameStatePjan = 0; 
  difficultyPjan = 0;
  playerPosPjan = 0;
  chooseNameIndexPjan = 0;
  scorePjan = 0;
  spawnCounterPjan = 4;
  bombCounterJude = 0;
  for (int i = 0; i < 21; i++)
  {
    bombsJude[i].currentpositionYJude = 0;
    bombsJude[i].currentpositionXJude = 0;
    bombsJude[i].beenShotJude = false;
    bombsJude[i].blownUpJude = false;
  }
  for (int i = 0; i < 11; i++)
  {
    asteroidsPjan[i].xPjan = 0;
    asteroidsPjan[i].yPjan = 0;
    asteroidsPjan[i].destroyedPjan = false;
    asteroidsPjan[i].existPjan = false;
  }
  PrintLn("Game has been reset");
}
