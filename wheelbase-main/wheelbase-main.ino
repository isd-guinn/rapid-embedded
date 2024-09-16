#include <ESP32Servo.h>
#include "lib/isd-dev-api/pinout.hpp"

#define ENA1_PIN 36
#define IN1_PIN 37
#define IN2_PIN 38

#define ENA2_PIN 12
#define IN3_PIN 13
#define IN4_PIN 14

#define RIGHT_PIN 48
#define LEFT_PIN 47

#define FOR_PIN 21
#define BACK_PIN 11

#define BRUSH_PIN 35

// global counter 
unsigned int g_counter = 0;
unsigned int g_counter_max = 30;

unsigned int drive_stack = 0;

bool isBrush = false;

/* 
    Control the driving state of one side of the wheels with this function.

    @param opt 
        Option of the state { 0:Stop; 1:Positive Direction; 2:Negative Direction }.
        "I have no idea which direction is witch physically :( . Don't Judge!!"

    @param ena
        Enable Pin of the side of the wheels.
    
    @param pin1
        Corrisponding IN1 / IN3 pin of the Relay Module.

    @param pin2
        Corrisponding IN2 / IN4 pin of the Relay Module.

*/
void drive_dir(int opt, int ena, int pin1, int pin2){

  // Do a truth table and you shall find out the pattern for the code to work. :) 
  digitalWrite( ena,  (opt==0)? LOW : HIGH);
  digitalWrite( pin1, (opt==1)? HIGH: LOW );
  digitalWrite( pin2, (opt==2)? HIGH: LOW );

}

void setup() {

  Serial.begin(115200);

  pinMode(LED_BUILTIN,OUTPUT);

  pinMode(ENA1_PIN,OUTPUT);
  pinMode(IN1_PIN,OUTPUT);
  pinMode(IN2_PIN,OUTPUT);

  pinMode(ENA2_PIN,OUTPUT);
  pinMode(IN3_PIN,OUTPUT);
  pinMode(IN4_PIN,OUTPUT);

  pinMode(FOR_PIN,INPUT);
  pinMode(BACK_PIN,INPUT);
  pinMode(LEFT_PIN,INPUT);
  pinMode(RIGHT_PIN,INPUT);

  pinMode(BRUSH_PIN,INPUT);

}

void loop() {

  while(digitalRead(FOR_PIN)) // Loop trigger whenever the button is pushed. Escaped when only the button is pulled.
  {
    g_counter++; // Counter Added for timing

    // When desired counter value/timing (g_counter_max) is reached, Change the state variable (drive_stack) of the driving motion:
    // When drive_stack==0 i.e. Stop, change the motion to desired motion.
    if (g_counter == g_counter_max) drive_stack = (drive_stack==0)?1:0 ; 
  }
  g_counter=0;

  while(digitalRead(BACK_PIN)){
    g_counter++;
    if (g_counter == g_counter_max) drive_stack = (drive_stack==0)?2:0 ;
  }
  g_counter=0;

  while(digitalRead(LEFT_PIN)){
    g_counter++;
    if (g_counter == g_counter_max) drive_stack = (drive_stack==0)?3:0 ;
  }
  g_counter=0;

  while(digitalRead(RIGHT_PIN)){
    g_counter++;
    if (g_counter == g_counter_max) drive_stack = (drive_stack==0)?4:0 ;
  }
  g_counter=0;

  switch(drive_stack){

    case 0:
      drive_dir(0,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(0,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("stop");
    break;

    case 1:
      drive_dir(1,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(2,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("forward");
    break;

    case 2:
      drive_dir(2,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(1,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("back");
    break;

    case 3:
      drive_dir(1,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(1,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("left");
    break;

    case 4:
      drive_dir(2,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(2,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("right");
    break;

    default:
      drive_dir(0,ENA1_PIN,IN1_PIN,IN2_PIN);
      drive_dir(0,ENA2_PIN,IN3_PIN,IN4_PIN);
      Serial.println("stop");
  }

  delay(100);
}
