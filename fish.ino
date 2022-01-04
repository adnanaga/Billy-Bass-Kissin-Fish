// Initialize our libraries

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Connect up the SD card serial RX and TX ports
//Serial
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int counter = 0;
int buttonCounter = 0;
int buttonPresses = 0;
int languageCounter = 1;

// Variables to keep track of past state

bool lastButtonState = false;
bool lastSensorState = false;

//state of kiss mode, as in "did fish ask for kiss"
bool kissReadyMode = false;

void setup() {

  //Set up
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

// Set up our button and our photoresistor
  pinMode(2, INPUT_PULLUP);
  pinMode(30, INPUT);

  while (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Not Connected!");
  }

  Serial.println("DFPlayer Connected!!!");
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down
  // Set EQ
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  // Set the SD Card as default source.
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  myDFPlayer.volume(25);  //Set volume value. From 0 to 30

  delay(1000);
}

void loop() {

  int sensorValue = analogRead(A7);

  if (analogRead(A7) < 35) {
    (Serial.println("incrementing counter"));
    counter++;
  } else {
    counter = 0;
    lastSensorState = false;
  }

  // Checks red button push
  if (digitalRead(2) == 0) {
    buttonCounter++;
  } else {
    buttonCounter = 0;
    lastButtonState = false;
  }

  Serial.println(sensorValue);

  if (buttonCounter == 3) {
    if (lastButtonState == true) {
      return;
    } else  {
      delay(1000);
      bodyForwards();
      if (languageCounter == 6) {
        languageCounter = 2;
      } else {
        languageCounter++;
      }
      giveMeAKiss(languageCounter);
      delay(500);
      delay(1000);
    }
  }

  // if close enough to kiss

  if (counter == 3) {
    if (lastSensorState == true) {
      return;
    } else if (kissReadyMode) {
      // Add button press code here
      kissAnim();
    }
  }
  delay(100);
}

void bodyForwards() {
  analogWrite(12, LOW);
  analogWrite(13, 255);
}

void bodyBackwards() {
  analogWrite(12, 255);
  analogWrite(13, 255);
}

void flapTail() {
    analogWrite(9, LOW);
    analogWrite(8, 255);
    delay(500);
    analogWrite(9, 255);
    analogWrite(8, LOW);
    delay(500);
}

void openMouth(int moufCount) {
  int i = 0;
  for (i = 0; i < moufCount; i++) {
    analogWrite(5, LOW);
    analogWrite(4, 255);
    delay(400);
    analogWrite(5, 255);
    analogWrite(4, LOW);
    delay(400);
  }

}

void closeMouth(int seconds) {
  analogWrite(5, 255);
  analogWrite(4, LOW);
  delay(seconds);
}

void talk(int seconds) {
  Serial.println(seconds);
  for (int i = 0; i < (seconds / 1000) * 5; i++) {
    if (analogRead(A0) > 512) {
      analogWrite(5, LOW);
      analogWrite(4, 255);
      delay(100);
      analogWrite(5, 255);
      analogWrite(4, LOW);
      delay(100);
    }
  }
}

void talkSyllables(int numWords, int totalTime) {

  int syllableLength = totalTime / numWords;
  for (int i = 0; i < numWords; i++) {
    analogWrite(5, LOW);
    analogWrite(4, 255);
    delay(syllableLength / 2);
    analogWrite(5, 255);
    analogWrite(4, LOW);
    delay(syllableLength / 2);
  }
}

void giveMeAKiss(int whichFile) {
  Serial.println(whichFile);

  if (whichFile == 2) {
    //English
    Serial.println("English");
    myDFPlayer.playMp3Folder(2);
    closeMouth(69);
    talkSyllables(1, 467);
    closeMouth(178);
    talkSyllables(3, 596);
    closeMouth(96);
    talkSyllables(1, 591);
  } else if (whichFile == 3) {
    //Chinese
    Serial.println("Chinese");
    myDFPlayer.playMp3Folder(3);
    closeMouth(19);
    talkSyllables(2, 369);
    talkSyllables(3, 410);
    talkSyllables(1, 250);
    talkSyllables(3, 508);

  } else if (whichFile == 4) {
    //Hindi
    Serial.println("Hindi");
    myDFPlayer.playMp3Folder(4);
    closeMouth(60);
    talkSyllables(2, 211);
    talkSyllables(3, 528);
    talkSyllables(3, 432);
    closeMouth(105);

  } else if (whichFile == 5) {
    //Korean
    Serial.println("Korean");
    myDFPlayer.playMp3Folder(5);
    closeMouth(95);
    talkSyllables(1, 126);
    closeMouth(85);
    talkSyllables(3, 561);
    closeMouth(117);
    talkSyllables(1, 130);
    talkSyllables(2, 335);
    closeMouth(113);
    talkSyllables(1, 287);
    closeMouth(115);

  } else if (whichFile == 6) {
    //Spanish
    Serial.println("Spanish");
    myDFPlayer.playMp3Folder(6);
    closeMouth(95);
    talkSyllables(1, 411);
    closeMouth(318);
    talkSyllables(4, 892);
    closeMouth(181);
  }
  //  flapTail(4);
  kissReadyMode = true;
}

void kissAnim() {
  myDFPlayer.playMp3Folder(1);
  delay(1000);
  openMouth(1);
  delay(1000);
  bodyBackwards();
  kissReadyMode = false;
}