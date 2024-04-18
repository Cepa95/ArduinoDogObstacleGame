#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define PIN_BUTTON 2
#define PIN_AUTOPLAY 1
#define SPRITE_RUN1 1
#define SPRITE_RUN2 2
#define SPRITE_JUMP 3
#define SPRITE_JUMP_UPPER '.'
#define SPRITE_JUMP_LOWER 4
#define SPRITE_TERRAIN_EMPTY ' '
#define SPRITE_TERRAIN_SOLID 5
#define SPRITE_TERRAIN_SOLID_RIGHT 6
#define SPRITE_TERRAIN_SOLID_LEFT 7
#define DOG_HORIZONTAL_POSITION 1
#define TERRAIN_WIDTH 16
#define TERRAIN_EMPTY 0
#define TERRAIN_LOWER_BLOCK 1
#define TERRAIN_UPPER_BLOCK 2

#define DOG_POSITION_OFF 0
#define DOG_POSITION_RUN_LOWER_1 1
#define DOG_POSITION_RUN_LOWER_2 2
#define DOG_POSITION_JUMP_1 3
#define DOG_POSITION_JUMP_2 4
#define DOG_POSITION_JUMP_3 5
#define DOG_POSITION_JUMP_4 6
#define DOG_POSITION_JUMP_5 7
#define DOG_POSITION_JUMP_6 8
#define DOG_POSITION_JUMP_7 9
#define DOG_POSITION_JUMP_8 10
#define DOG_POSITION_RUN_UPPER_1 11
#define DOG_POSITION_RUN_UPPER_2 12

LiquidCrystal_I2C lcd(0x27, 16, 2);


static char terrainUpper[TERRAIN_WIDTH + 1];
static char terrainLower[TERRAIN_WIDTH + 1];
static bool buttonPushed = false;


void initializeGraphics();
void advanceTerrain(char *terrain, byte newTerrain);
bool drawDog(byte position, char *terrainUpper, char *terrainLower, unsigned int score);
void buttonPush();
void setupPins();
void displayGameOver(unsigned int distance);
void displayStartGame();
void startGame(byte &dogPos, bool &playing, unsigned int &distance);
void updateTerrain(byte &newTerrainType, byte &newTerrainDuration);
void updateDogPosition(byte &dogPos, bool &playing, unsigned int &distance);

void initializeGraphics();

void buttonPush() {
  buttonPushed = true;
}

void setupPins() {
  pinMode(PIN_BUTTON, INPUT);
  digitalWrite(PIN_BUTTON, HIGH);
  pinMode(PIN_AUTOPLAY, OUTPUT);
  digitalWrite(PIN_AUTOPLAY, HIGH);
}

void displayGameOver(unsigned int distance) {
  lcd.clear();
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(distance >> 3);
}

void displayStartGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start Game");
}

void startGame(byte &dogPos, bool &playing, unsigned int &distance) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Started");
  delay(300);
  initializeGraphics();
  dogPos = DOG_POSITION_RUN_LOWER_1;
  playing = true;
  buttonPushed = false;
  distance = 0;
}

void updateTerrain(byte &newTerrainType, byte &newTerrainDuration) {
  advanceTerrain(terrainLower, (newTerrainType == TERRAIN_LOWER_BLOCK) ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);
  advanceTerrain(terrainUpper, (newTerrainType == TERRAIN_UPPER_BLOCK) ? SPRITE_TERRAIN_SOLID : SPRITE_TERRAIN_EMPTY);

  if (--newTerrainDuration == 0) {
    newTerrainType = (newTerrainType == TERRAIN_EMPTY) ? (random(3) == 0 ? TERRAIN_UPPER_BLOCK : TERRAIN_LOWER_BLOCK) : TERRAIN_EMPTY;
    newTerrainDuration = 10 + random(6);
  }
}

void updateDogPosition(byte &dogPos, bool &playing, unsigned int &distance) {
  if (buttonPushed) {
    if (dogPos <= DOG_POSITION_RUN_LOWER_2) dogPos = DOG_POSITION_JUMP_1;
    buttonPushed = false;
  }

  if (drawDog(dogPos, terrainUpper, terrainLower, distance >> 3)) {
    playing = false;                
    for (int i = 0; i <= 2; i++) {} 
  } else {
    if (dogPos == DOG_POSITION_RUN_LOWER_2 || dogPos == DOG_POSITION_JUMP_8) {
      dogPos = DOG_POSITION_RUN_LOWER_1;
    } else if ((dogPos >= DOG_POSITION_JUMP_3 && dogPos <= DOG_POSITION_JUMP_5) && terrainLower[DOG_HORIZONTAL_POSITION] != SPRITE_TERRAIN_EMPTY) {
      dogPos = DOG_POSITION_RUN_UPPER_1;
    } else if (dogPos >= DOG_POSITION_RUN_UPPER_1 && terrainLower[DOG_HORIZONTAL_POSITION] == SPRITE_TERRAIN_EMPTY) {
      dogPos = DOG_POSITION_JUMP_5;
    } else if (dogPos == DOG_POSITION_RUN_UPPER_2) {
      dogPos = DOG_POSITION_RUN_UPPER_1;
    } else {
      ++dogPos;
    }
    ++distance;
    digitalWrite(PIN_AUTOPLAY, terrainLower[DOG_HORIZONTAL_POSITION + 2] == SPRITE_TERRAIN_EMPTY ? HIGH : LOW);
  }
}

void initializeGraphics() {
  static byte graphics[] = {
    0b00000,
    0b00000,
    0b00000,
    0b00010,
    0b00011,
    0b11110,
    0b01010,
    0b01010,
    0b00000,
    0b00000,
    0b00000,
    0b00010,
    0b00011,
    0b11110,
    0b01010,
    0b10001,
    0b00000,
    0b00000,
    0b00000,
    0b00010,
    0b00011,
    0b11110,
    0b01010,
    0b10001,
  };

  for (int i = 0; i < 3; ++i) {
    lcd.createChar(i + 1, &graphics[i * 8]);
  }

  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    terrainUpper[i] = SPRITE_TERRAIN_EMPTY;
    terrainLower[i] = SPRITE_TERRAIN_EMPTY;
  }
}

void advanceTerrain(char *terrain, byte newTerrain) {
  for (int i = 0; i < TERRAIN_WIDTH; ++i) {
    char current = terrain[i];
    char next = (i == TERRAIN_WIDTH - 1) ? newTerrain : terrain[i + 1];
    switch (current) {
      case SPRITE_TERRAIN_EMPTY:
        terrain[i] = (next == SPRITE_TERRAIN_SOLID) ? SPRITE_TERRAIN_SOLID_RIGHT : SPRITE_TERRAIN_EMPTY;
        break;
      case SPRITE_TERRAIN_SOLID:
        terrain[i] = (next == SPRITE_TERRAIN_EMPTY) ? SPRITE_TERRAIN_SOLID_LEFT : SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_RIGHT:
        terrain[i] = SPRITE_TERRAIN_SOLID;
        break;
      case SPRITE_TERRAIN_SOLID_LEFT:
        terrain[i] = SPRITE_TERRAIN_EMPTY;
        break;
    }
  }
}

bool drawDog(byte position, char *terrainUpper, char *terrainLower, unsigned int score) {

  bool collide = false;
  char upperTerrainSave = terrainUpper[DOG_HORIZONTAL_POSITION];
  char lowerTerrainSave = terrainLower[DOG_HORIZONTAL_POSITION];
  byte upperSprite, lowerSprite;

  if (position == DOG_POSITION_OFF) {
    upperSprite = lowerSprite = SPRITE_TERRAIN_EMPTY;
  } else if (position == DOG_POSITION_RUN_LOWER_1) {
    upperSprite = SPRITE_TERRAIN_EMPTY;
    lowerSprite = SPRITE_RUN1;
  } else if (position == DOG_POSITION_RUN_LOWER_2) {
    upperSprite = SPRITE_TERRAIN_EMPTY;
    lowerSprite = SPRITE_RUN2;
  } else if (position == DOG_POSITION_JUMP_1 || position == DOG_POSITION_JUMP_8) {
    upperSprite = SPRITE_TERRAIN_EMPTY;
    lowerSprite = SPRITE_JUMP;
  } else if (position == DOG_POSITION_JUMP_2 || position == DOG_POSITION_JUMP_7) {
    upperSprite = SPRITE_JUMP_UPPER;
    lowerSprite = SPRITE_JUMP_LOWER;
  } else if (position >= DOG_POSITION_JUMP_3 && position <= DOG_POSITION_JUMP_6) {
    upperSprite = SPRITE_JUMP;
    lowerSprite = SPRITE_TERRAIN_EMPTY;
  } else if (position == DOG_POSITION_RUN_UPPER_1) {
    upperSprite = SPRITE_RUN1;
    lowerSprite = SPRITE_TERRAIN_EMPTY;
  } else if (position == DOG_POSITION_RUN_UPPER_2) {
    upperSprite = SPRITE_RUN2;
    lowerSprite = SPRITE_TERRAIN_EMPTY;
  }

  if (upperSprite != ' ') {
    terrainUpper[DOG_HORIZONTAL_POSITION] = upperSprite;
    collide = (upperTerrainSave != SPRITE_TERRAIN_EMPTY);
  }
  if (lowerSprite != ' ') {
    terrainLower[DOG_HORIZONTAL_POSITION] = lowerSprite;
    collide |= (lowerTerrainSave != SPRITE_TERRAIN_EMPTY);
  }


  byte scoreDigits = (score > 9999) ? 5 : (score > 999) ? 4
                                        : (score > 99)  ? 3
                                        : (score > 9)   ? 2
                                                        : 1;
  terrainUpper[TERRAIN_WIDTH] = '\0';
  terrainLower[TERRAIN_WIDTH] = '\0';
  char temp = terrainUpper[16 - scoreDigits];
  terrainUpper[16 - scoreDigits] = '\0';
  lcd.setCursor(0, 0);
  lcd.print(terrainUpper);
  terrainUpper[16 - scoreDigits] = temp;
  lcd.setCursor(0, 1);
  lcd.print(terrainLower);
  lcd.setCursor(16 - scoreDigits, 0);
  lcd.print(score);


  terrainUpper[DOG_HORIZONTAL_POSITION] = upperTerrainSave;
  terrainLower[DOG_HORIZONTAL_POSITION] = lowerTerrainSave;

  return collide;
}

void setup() {
  setupPins();
  attachInterrupt(0, buttonPush, FALLING);  
  initializeGraphics();
  lcd.init();
  lcd.backlight();
}

void loop() {
  static byte dogPos = DOG_POSITION_RUN_LOWER_1;
  static byte newTerrainType = TERRAIN_EMPTY;
  static byte newTerrainDuration = 1;
  static bool playing = false;
  static unsigned int distance = 0;

  if (!playing) {
    if (distance > 0) {
      displayGameOver(distance);
    } else {
      displayStartGame();
    }

    if (buttonPushed) {
      startGame(dogPos, playing, distance);
    }
    delay(200);
    return;
  }

  updateTerrain(newTerrainType, newTerrainDuration);
  updateDogPosition(dogPos, playing, distance);
}