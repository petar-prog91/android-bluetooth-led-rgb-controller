#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial mySerial(2, 4); // RX, TX

int mRed = 0;
int mGreen = 1;
int mBlue = 2;

/* clap sound on/off */

int clapInput = 0;
int clapValue = 0;
boolean clapped = false;

/* LED strip */

int blue = 11;
int red = 10;
int green = 9;
bool ledStripState = false; // on == true; off == false;

/* Bluetooth serial comm. */

char character;
String command;

/* User Functions */

void ledStripOn();
void ledOldColor();
void ledStripOff();

void clapToStart()
{
  clapValue = analogRead(clapInput);

  if (clapValue >= 550)
  {
    clapped = true;
    delay(200);

    if (!ledStripState)
    {
      ledStripOn();
      delay(5);
      ledOldColor();
    }

    else
    {
      ledStripOff();
    }

    ledStripState = !ledStripState;
    clapped = false;
  }
}

void commWithBT()
{

  while (mySerial.available() > 0)
  {
    character = mySerial.read();
    command.concat(character);
    delay(1);
  }

  if (command == "LED_ON")
  {
    ledStripOn();
    ledOldColor();
    ledStripState = true;
  }

  else if (command == "LED_OFF")
  {
    ledStripOff();
    ledStripState = false;
  }

  else if (command[0] == 'R')
  {
    command = command.substring(0, 5);
    int value = command.substring(2).toInt();

    analogWrite(red, value);
    EEPROM.write(mRed, value);
  }

  else if (command[0] == 'G')
  {
    command = command.substring(0, 5);
    int value = command.substring(2).toInt();

    analogWrite(green, value);
    EEPROM.write(mGreen, value);
  }

  else if (command[0] == 'B')
  {
    command = command.substring(0, 5);
    int value = command.substring(2).toInt();

    analogWrite(blue, value);
    EEPROM.write(mBlue, value);
  }

  command = "";
}

void ledStripOn()
{
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(blue, HIGH);
}

void ledStripOff()
{
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  digitalWrite(red, LOW);
}

void ledOldColor()
{
  int valueRed = EEPROM.read(mRed);
  analogWrite(red, valueRed);

  int valueGreen = EEPROM.read(mGreen);
  analogWrite(green, valueGreen);

  int valueBlue = EEPROM.read(mBlue);
  analogWrite(blue, valueBlue);
}

void setup()
{
  Serial.begin(9600);

  pinMode(clapInput, INPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  mySerial.begin(9600);
}

void loop()
{

  clapToStart();
  if (mySerial.available() > 0)
  {
    commWithBT();
  }
}
