// Based on code at http://www.lamja.com/?p=451

// Fuses to wipe EEPROM on upload:
// avrdude -c usbtiny -p atmega328p -U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0x05:m

// Fuses to prevent loss of EEPROM on upload:
// avrdude -c usbtiny -p atmega328p -U lfuse:w:0xff:m -U hfuse:w:0xd6:m -U efuse:w:0x05:m

#include <Servo.h> 
#include <EEPROM.h>
#include <Debounce.h>

const int MODE_NORMAL = 0;
const int MODE_ADJUST_LID = 1;
const int MODE_ADJUST_SWITCH = 2;
int mode = 0;

const int EEPROM_LID_FROM = 10;
const int EEPROM_SWITCH_TO = 20;

const int switchServoPin = 8;
const int lidServoPin = 7;
const int buttonPin = 2;
const int ledPin = 3;
const int potPin = A0;

StableDebounce modeButton = StableDebounce();

Servo switchServo;
Servo lidServo;  
                 
int lidFrom = 1200; // adjustable
int lidTo = 1900; // fixed
int lidRange = lidTo - lidFrom;

int switchFrom = 1800; // fixed
int switchTo = 1100; // adjustable
int switchRange = switchFrom - switchTo;

void setup() 
{ 
  pinMode(6, OUTPUT);  // turn off motor power for startup
  digitalWrite(6, LOW);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  int schema = EEPROM.read(511);
  if (schema == 87)
  {
    EEPROM.get(EEPROM_LID_FROM, lidFrom);
    EEPROM.get(EEPROM_SWITCH_TO, switchTo);
  }
  else
  {
    // If we haven't run on the MCP before, set the end points
    // of the motor travel equal to the start point and the user
    // will need to do the adjust routine to set them so there's motion.
    lidFrom = lidTo;
    EEPROM.put(EEPROM_LID_FROM, lidFrom);
    switchTo = switchFrom;
    EEPROM.put(EEPROM_SWITCH_TO, switchTo);
    EEPROM.write(511, 87);
  }

  switchServo.attach(switchServoPin);
  lidServo.attach(lidServoPin);
  switchServo.write(switchFrom);
  lidServo.write(lidFrom);
  
  pinMode(buttonPin, INPUT_PULLUP);     
  modeButton.attach(buttonPin);
  modeButton.interval(100);
    
  randomSeed(getSeedFromAnalogPin(A5));

  digitalWrite(6, HIGH);  // turn on motor power
}  

unsigned long getSeedFromAnalogPin(int pin)
{
  // From http://www.utopiamechanicus.com/article/better-arduino-random-numbers/
  unsigned long seed=0, count=32;
  while (--count)
    seed = (seed<<1) | (analogRead(pin)&1);
  return seed;
}

void loop() 
{ 
  if (modeButton.update() && modeButton.read() == LOW)
  {
    mode = ++mode % 3;

    if (mode == MODE_NORMAL)
    {
      // Leaving MODE_ADJUST_SWITCH
      EEPROM.put(EEPROM_SWITCH_TO, switchTo);
      
      digitalWrite(ledPin, LOW);
    }
    
    if (mode == MODE_ADJUST_LID)
    {
      digitalWrite(ledPin, HIGH);
    }

    if (mode == MODE_ADJUST_SWITCH)
    {
      // Leaving MODE_ADJUST_LID
      EEPROM.put(EEPROM_LID_FROM, lidFrom);

      // Move the lid arm off the switch so we don't shut ourselves off
      lidServo.writeMicroseconds(lidFrom + 200);
      
      digitalWrite(ledPin, LOW);
      delay(500);
      digitalWrite(ledPin, HIGH);
    }
  }
  
  if (mode == MODE_NORMAL)
  {
    RunSequence();
  }
  
  if (mode == MODE_ADJUST_LID)
  {
    AdjustLid();
  }

  if (mode == MODE_ADJUST_SWITCH)
  {
    AdjustSwitch();
  }
}

void RunSequence()
{
  int sequence = random(0, 20);
  
  if (sequence == 0) Sequence0();
  if (sequence == 1) Sequence1();
  if (sequence == 2) Sequence2();
  if (sequence == 3) Sequence3();
  if (sequence == 4) Sequence4();
  if (sequence == 5) Sequence5();
  if (sequence == 6) Sequence6();
  if (sequence == 7) Sequence7();
  if (sequence == 8) Sequence8();
  if (sequence == 9) Sequence9();
  if (sequence == 10) Sequence10();
  if (sequence > 10) DefaultSequence();
    
  delay(3000);
}

void AdjustLid()
{
  lidFrom = lidTo - analogRead(potPin);
  lidServo.writeMicroseconds(lidFrom);
}

void AdjustSwitch()
{
  switchTo = switchFrom - analogRead(potPin);
  switchServo.writeMicroseconds(switchTo);
}

// Plain old open the lid, press the switch, close the lid
void DefaultSequence()
{
  delay(50);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(200);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(300);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(150);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence0()
{
  int lidMid = lidFrom  + lidRange * 0.80;
  int lidMid4 = lidFrom  + lidRange * 0.60;
  int switchMid = switchTo + switchRange * 0.57;

  delay(50);
  Sweep(lidServo, lidFrom, lidMid, 3000);
  delay(1000);
  Sweep(lidServo, lidMid, lidMid4, 500);
  delay(1000);
  Sweep(lidServo, lidMid4, lidTo, 1000);
  Sweep(switchServo, switchFrom, switchMid, 1800);
  Sweep(switchServo, switchMid, switchTo, 500);
  delay(100);
  Sweep(switchServo, switchTo, switchFrom, 500);
  Sweep(lidServo, lidTo, lidFrom, 500);
}

void Sequence1()
{
  int lidMid = lidFrom  + lidRange * 0.90;
  int lidMid2 = lidFrom + lidRange * 0.68;
  int lidMid3 = lidFrom + lidRange * 0.77;
  int lidMid4 = lidFrom  + lidRange * 0.60;
  int switchMid = switchTo + switchRange * 0.57;

  delay(50);
  Sweep(lidServo, lidFrom, lidMid2, 3000);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(120);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(120);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(120);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(120);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(120);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(120);
  Sweep(lidServo, lidMid2, lidMid4, 3000);
  Sweep(lidServo, lidMid4, lidMid, 3000);
  delay(1000);
  Sweep(lidServo, lidMid, lidTo, 1000);
  Sweep(switchServo, switchFrom, switchMid, 1800);
  Sweep(switchServo, switchMid, switchTo, 500);
  delay(100);
  Sweep(switchServo, switchTo, switchFrom, 500);
  Sweep(lidServo, lidTo, lidFrom, 500);
}

void Sequence2()
{
  int switchMid2 = switchTo + switchRange * 0.28;   // Almost on the switch

  delay(50);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(1);
  Sweep(switchServo, switchFrom, switchMid2, 1);
  delay(450);
  Sweep(switchServo, switchMid2, switchTo, 30000);
  delay(1);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence3()
{
  int switchMid2 = switchTo + switchRange * 0.05;   // Almost on the switch

  delay(50);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(200);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(50);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(50);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence4()
{
  int lidMid4 = lidFrom  + lidRange * 0.85;

  delay(50);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(200);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(450);
  Sweep(lidServo, lidTo, lidMid4, 1000);
  delay(2000);
  Sweep(lidServo, lidMid4, lidTo, 1000);
  delay(2000);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence5()
{
  int lidMid = lidFrom + lidRange * 0.90;
  int lidMid2 = lidFrom + lidRange * 0.68;
  int lidMid4 = lidFrom  + lidRange * 0.50;

  delay(50);
  Sweep(lidServo, lidFrom, lidMid, 1);
  delay(200);
  Sweep(lidServo, lidMid, lidMid2, 1);
  delay(100);
  Sweep(lidServo, lidMid2, lidMid, 1);
  delay(100);
  Sweep(lidServo, lidMid, lidMid2, 1);
  delay(100);
  Sweep(lidServo, lidMid2, lidMid, 1);
  delay(100);
  Sweep(lidServo, lidMid, lidMid4, 1);
  delay(200);
  Sweep(lidServo, lidMid4, lidTo, 1);
  delay(1);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(450);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence6()
{
  int lidMid = lidTo;
  int lidMid2 = lidFrom + lidRange * 0.68;
  int lidMid3 = lidFrom + lidRange * 0.77;
  int lidMid4 = lidFrom  + lidRange * 0.50;

  delay(50);
  Sweep(lidServo, lidFrom, lidMid, 1);
  delay(200);
  Sweep(lidServo, lidMid, lidMid2, 1);
  delay(100);
  Sweep(lidServo, lidMid2, lidMid, 1);
  delay(100);
  Sweep(lidServo, lidMid, lidMid2, 1);
  delay(100);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid4, 1);
  delay(200);
  Sweep(lidServo, lidMid4, lidTo, 1);
  delay(1);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(450);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence7()
{
  int lidMid = lidTo;
  int lidMid2 = lidFrom + lidRange * 0.68;
  int lidMid3 = lidFrom + lidRange * 0.77;

  delay(50);
  Sweep(lidServo, lidFrom, lidMid, 2000);
  delay(500);
  Sweep(lidServo, lidMid, lidMid2, 1000);
  delay(1);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(50);
  Sweep(lidServo, lidMid2, lidMid3, 1);
  delay(50);
  Sweep(lidServo, lidMid3, lidMid2, 1);
  delay(500);
  Sweep(lidServo, lidMid2, lidMid, 5000);
  delay(1);
  Sweep(lidServo, lidMid, lidTo, 1000);
  delay(1);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(450);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence8()
{
  delay(1000);
  Sweep(lidServo, lidFrom, lidTo, 30000);
  delay(1);
  Sweep(switchServo, switchFrom, switchTo, 5000);
  delay(1);
  Sweep(switchServo, switchTo, switchFrom, 5000);
  delay(500);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(300);
}

void Sequence9()
{
  int switchMid2 = switchTo + switchRange * 0.28;

  delay(100);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(200);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(1000);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(110);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(500);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(110);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(500);
  Sweep(switchServo, switchTo, switchMid2, 1);
  delay(110);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(110);
  Sweep(switchServo, switchTo, switchFrom, 1);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sequence10()
{
  int switchMid2 = switchTo + switchRange * 0.35;

  delay(100);
  Sweep(lidServo, lidFrom, lidTo, 1);
  delay(200);
  Sweep(switchServo, switchFrom, switchTo, 1);
  delay(2000);
  Sweep(switchServo, switchTo, switchMid2, 6000);
  delay(500);
  Sweep(switchServo, switchMid2, switchTo, 1);
  delay(2000);
  Sweep(switchServo, switchTo, switchFrom, 2000);
  delay(200);
  Sweep(lidServo, lidTo, lidFrom, 1);
  delay(400);
}

void Sweep(Servo& servo, int from, int to, int usec)  
{
  if (from <= to)
  {
    for (int pos = from; pos < to; pos++)
    {
      servo.writeMicroseconds(pos);
      delayMicroseconds(usec);
    }
  }
  else
  {
    for (int pos = from; pos >= to; pos--)
    {
      servo.writeMicroseconds(pos);
      delayMicroseconds(usec); 
    }
  }
}
