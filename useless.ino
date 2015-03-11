// Based on code at http://www.lamja.com/?p=451

#include <Servo.h> 
#include <EEPROM.h>

Servo myservo2;  // create servo object to control a servo 
Servo myservo1;  
                 
int pos = 0;    // variable to store the servo position 
int boxon =1;

// Predefined positions for a spesific box/servo
int S1fra = 1200;    // from   s1=Servo1
int S1mid4 = 1400;
int S1mid = 1900;    // Mid
int S1mid2 = 1680;
int S1mid3 = 1740;
int S1til = 1900;    // To
int S2fra = 1800;    //From    s2=Servo2
int S2mid = 1500;
int S2mid2 = 1300;   // Almost on the switch
int S2til = 1100;

int seq = 0;
  
 void setup() 
{ 
  pinMode(6, OUTPUT);  // turn off motor power for startup
  digitalWrite(6, LOW);

  myservo2.attach(8);  // attaches the servo on pin 13 to the servo object (PB5)
  myservo1.attach(7);  // attaches the servo on pin 12 to the servo object (PB4)
  myservo2.write(S2fra);
  myservo1.write(S1fra);
  
  seq = EEPROM.read(0);

  digitalWrite(6, HIGH);  // turn on motor power
}  
  
void loop() 
{ 
    seq++;
    if(seq>17) seq=0;
    EEPROM.write(0, seq);

    if(seq == 0)Sequense3();
    if(seq == 1)Sequense1();
    if(seq == 2)Sequense3();
    if(seq == 3)Sequense9();
    if(seq == 4)Sequense3();
    if(seq == 5)Sequense5();
    if(seq == 6)Sequense3();
    if(seq == 7)Sequense7();
    if(seq == 8)Sequense3();
    if(seq == 9)Sequense2();
    if(seq == 10)Sequense3();
    if(seq == 11)Sequense10();
    if(seq == 12)Sequense3();
    if(seq == 13)Sequense8();
    if(seq == 14)Sequense3();
    if(seq == 15)Sequense6();
    if(seq == 16)Sequense3();
    if(seq == 17)Sequense4();
    
    delay(3000);
}


//Fra=From, Til=To (Sorry for some Norwegian variables)
void Sweep(int srv, int fra, int til, int usec)  
{
  if(srv == 1)
  {
    if(fra <= til)
      for(pos = fra; pos < til; pos += 1)
      {
        myservo1.writeMicroseconds(pos);
        delayMicroseconds(usec);
      }
    else
    {
      for(pos = fra; pos>=til; pos-=1)
      {
         myservo1.writeMicroseconds(pos);
        delayMicroseconds(usec); 
      }
    }
  }
  if(srv == 2)
  {
    if(fra <= til)
      for(pos = fra; pos < til; pos += 1)
      {
        myservo2.writeMicroseconds(pos);
        delayMicroseconds(usec);
      }
    else
    {
      for(pos = fra; pos>=til; pos-=1)
      {
         myservo2.writeMicroseconds(pos);
        delayMicroseconds(usec); 
      }
    }
  }
}

void Sequense1()
{
  delay(700);
  Sweep(1, S1fra, S1mid, 3000);
  delay(1000);
  Sweep(1, S1mid, S1mid4, 500);
  delay(1000);
  Sweep(1, S1mid4, S1til, 1000);
  Sweep(2, S2fra, S2mid, 1800);
  Sweep(2, S2mid, S2til, 500);
  delay(100);
  Sweep(2, S2til, S2fra, 500);
  Sweep(1, S1til, S1fra, 500);
}

void Sequense2()
{
  delay(800);
  Sweep(1, S1fra, S1mid2, 3000);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(120);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(120);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(120);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(120);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(120);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(120);
  Sweep(1, S1mid2, S1mid4, 3000);
  Sweep(1, S1mid4, S1mid, 3000);
  delay(1000);
  Sweep(1, S1mid, S1til, 1000);
  Sweep(2, S2fra, S2mid, 1800);
  Sweep(2, S2mid, S2til, 500);
  delay(100);
  Sweep(2, S2til, S2fra, 500);
  Sweep(1, S1til, S1fra, 500);
}

void Sequense3()
{
  delay(50);
  Sweep(1, S1fra, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense4()
{
  delay(500);
  Sweep(1, S1fra, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2mid2, 1);
  delay(450);
  Sweep(2, S2mid2, S2til, 30000);
  delay(1);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense5()
{
  delay(1000);
  Sweep(1, S1fra, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(2, S2til, S2mid2, 1);
  delay(110);
  Sweep(2, S2mid2, S2til, 1);
  delay(110);
  Sweep(2, S2til, S2mid2, 1);
  delay(110);
  Sweep(2, S2mid2, S2til, 1);
  delay(110);
  Sweep(2, S2til, S2mid2, 1);
  delay(110);
  Sweep(2, S2mid2, S2til, 1);
  delay(110);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense6()
{
  delay(1500);
  Sweep(1, S1fra, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(1, S1til, S1mid4, 1000);
  delay(2000);
  Sweep(1, S1mid4, S1til, 1000);
  delay(2000);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense7()
{
  delay(500);
  Sweep(1, S1fra, S1mid, 1);
  delay(200);
  Sweep(1, S1mid, S1mid2, 1);
  delay(100);
  Sweep(1, S1mid2, S1mid, 1);
  delay(100);
  Sweep(1, S1mid, S1mid2, 1);
  delay(100);
  Sweep(1, S1mid2, S1mid, 1);
  delay(100);
  Sweep(1, S1mid, S1mid4, 1);
  delay(200);
  Sweep(1, S1mid4, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense8()
{
  delay(200);
  Sweep(1, S1fra, S1mid, 1);
  delay(200);
  Sweep(1, S1mid, S1mid2, 1);
  delay(100);
  Sweep(1, S1mid2, S1mid, 1);
  delay(100);
  Sweep(1, S1mid, S1mid2, 1);
  delay(100);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid4, 1);
  delay(200);
  Sweep(1, S1mid4, S1til, 1);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense9()
{
  delay(1000);
  Sweep(1, S1fra, S1mid, 2000);
  delay(500);
  Sweep(1, S1mid, S1mid2, 1000);
  delay(1);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(50);
  Sweep(1, S1mid2, S1mid3, 1);
  delay(50);
  Sweep(1, S1mid3, S1mid2, 1);
  delay(500);
  Sweep(1, S1mid2, S1mid, 5000);
  delay(1);
  Sweep(1, S1mid, S1til, 1000);
  delay(1);
  Sweep(2, S2fra, S2til, 1);
  delay(450);
  Sweep(2, S2til, S2fra, 1);
  delay(200);
  Sweep(1, S1til, S1fra, 1);
  delay(400);
}

void Sequense10()
{
  delay(800);
  Sweep(1, S1fra, S1til, 30000);
  delay(1);
  Sweep(2, S2fra, S2til, 3000);
  delay(1);
  Sweep(2, S2til, S2fra, 3000);
  delay(1);
  Sweep(1, S1til, S1mid, 30000);
  delay(1);
  Sweep(1, S1mid, S1fra, 1);
  delay(300);
}

