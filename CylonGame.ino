#include "pitches.h"

// Mario flag song
int winMelody[] = {
  NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_E5,
  NOTE_GS3, NOTE_C4, NOTE_DS4, NOTE_GS4, NOTE_C5, NOTE_DS5, NOTE_GS5, NOTE_DS5,
  NOTE_AS3, NOTE_D4, NOTE_F4, NOTE_AS4, NOTE_D5, NOTE_F5, NOTE_AS5, NOTE_AS5, NOTE_AS5, NOTE_AS5, NOTE_C6, 0
};

// Durations of Mario flag song
int winDurations[] = {
  8, 8, 8, 8, 8, 8, 3, 3,
  8, 8, 8, 8, 8, 8, 3, 3,
  8, 8, 8, 8, 8, 8, 3, 8, 8, 8, 1, 2
};

// Up level music
int upLevelMelody[] = {
  NOTE_F4, NOTE_G4
};
int upLevelDurations[] = {
  8, 8
};

// Beginning music
int beginMelody[] = {
  NOTE_B3, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4
};
int beginDurations[] = { 
  8, 8, 8, 2
};

// Losing music
int loseMelody[] = {
  NOTE_AS3, NOTE_A3, NOTE_GS3, NOTE_G3, 0
};
int loseDurations[] = {
  4, 4, 4, 1, 2
};

// gameState
// 0 = starting position
// 1 = game running
// 2 = game won
// 3 = game lost
int gameState = 0;

const int speakerPin = 13;
const int buttonPin = 2;
int buttonState = 0;

const int cylonStart = 7;
const int cylonStartDir = 1;
const int highest = 12;
const int lowest = 3;
int cylonCurr = 3;
int cylonDir = 1;
int cylonSpeed = 50;
int level = 0;
int lowLimit = 2;
int highLimit = 13;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  switch(gameState) {
     case 0: // Starting position
       moveCylon();
       
       // Read button state
       buttonState = getButtonState();
       if (buttonState == HIGH) {
         gameState = 1;
         lowLimit = 2;
         highLimit = 13;
         cylonSpeed = 250;
         playMusic(1, 5);
         raiseLevel();
       }
       break;
     case 1: // Game running
       buttonState = getButtonState();
       if (buttonState == HIGH && cylonCurr < highLimit && cylonCurr > lowLimit && isGameWon()) {
         gameState = 2;
       } else if (buttonState == HIGH && cylonCurr < highLimit && cylonCurr > lowLimit) {
         playMusic(4, 2);
         raiseLevel();
       } else if (buttonState == HIGH && (cylonCurr >= highLimit || cylonCurr <= lowLimit)) {
         gameState = 3;
       } else {
         gameState = 1;
       }
       
       if (gameState == 1) {
         moveCylon();
         
         digitalWrite(lowLimit, HIGH);
         digitalWrite(highLimit, HIGH);
       }
       break;
     case 2: // Game won
       playMusic(2, 28);
       gameState = 0;
       resetVariables();
       break;
     case 3: // Game lost
       playMusic(3, 5);
       gameState = 0;
       resetVariables();
       break;
  }
}

void playMusic(int toPlay, int sizeOfArray) {
  int *melodyArr;
  int *durationArr;
  switch (toPlay) {
    case 1:
      melodyArr = beginMelody;
      durationArr = beginDurations;
      break;
    case 2:
      melodyArr = winMelody;
      durationArr = winDurations;
      break;
    case 3:
      melodyArr = loseMelody;
      durationArr = loseDurations;
      break;
    case 4:
      melodyArr = upLevelMelody;
      durationArr = upLevelDurations;
  }
  for (int currNote = 0; currNote < sizeOfArray; currNote++) {
    int noteDuration = 1000 / durationArr[currNote];
    tone(speakerPin, melodyArr[currNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerPin);
  }
}

int getButtonState() {
  return digitalRead(buttonPin);
}

boolean isGameWon() {
  return (level == 4);
}

void moveCylon() {
   digitalWrite(cylonCurr, HIGH);
   if (cylonCurr == highest) {
     cylonDir = -1;
     if (gameState == 1) tone(speakerPin, NOTE_C3, 50);
   }
   else if (cylonCurr == lowest) {
     cylonDir = 1;
     if (gameState == 1) tone(speakerPin, NOTE_C3, 50);
   }
   cylonCurr += cylonDir;
   delay(cylonSpeed);
   digitalWrite(cylonCurr - 1, LOW);
   digitalWrite(cylonCurr + 1, LOW);
}

void raiseLevel() {
  lowLimit++;
  highLimit--;
  level++;
  cylonCurr = cylonStart;
  cylonDir = cylonStartDir;
  cylonSpeed -= 50;
  digitalWrite(lowLimit - 1, LOW);
  digitalWrite(highLimit + 1, LOW);
  digitalWrite(lowLimit, HIGH);
  digitalWrite(highLimit, HIGH);
  digitalWrite(cylonCurr, HIGH);
  delay(2000);
}

void resetVariables() {
  cylonCurr = 3;
  cylonDir = 1;
  cylonSpeed = 50;
  level = 0;
  lowLimit = 2;
  highLimit = 13;
}
