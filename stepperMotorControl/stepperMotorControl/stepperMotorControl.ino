/*
  <-------------------------------------->
  'S'--> Frequency of Stepper Motor
  'A' --> Acceleration of Stepper Motor
  'X' --> Sets the default fixed value to Motor (frequency and Aceeleration)
  'G' --> Position of Motors
  <-------------------------------------->
*/

#include<AccelStepper.h>
#include <MultiStepper.h> //https://www.airspayce.com/mikem/arduino/AccelStepper/classMultiStepper.html

AccelStepper Stepper1(1, 5, 8); //Pin-5 is for +Pulse and Pin-8 is for Direction; 1 is for using driver interface(if --> 0 is for not direct control from arduino)
AccelStepper Stepper2(1, 6, 9); //Pin-6 is for +Pulse and Pin-9 is for Direction; 1 is for using driver interface(if --> 0 is for not direct control from arduino)
AccelStepper Stepper3(1, 7, 10); //Pin-7 is for +Pulse and Pin-10 is for Direction; 1 is for using driver interface(if --> 0 is for not direct control from arduino)

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;

double step1 = 0, step2 = 0, step3 = 0; //Rotation steps
long setFrq = 0; //Frequency of Pulses
long setAccl = 0; //Acceleration of Motor
long steps[3] = {}; // Array of desired stepper positions

#define MICROSTEP 6400.0 //!!! Stepper driver switch position: 1 to 7 : (off, off, off, on, on, on, off, on)

//Interrupt pins and initial values
int motorInt1 = 18, motorInt2 = 19, motorInt3 = 2; //pin 18,19,2 are dedicated interrupt pins on Arduino Mega
long stepss[3] = {MICROSTEP, MICROSTEP, MICROSTEP};
int flag = 0;

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); // opens Serial3 port for Stepper Motor, sets data rate to 115200 bps

  while (Serial3) {
    //Serial3.println("Stepper Motor Controls Available:");
    break;
  }

  //clear interrupt
  //EIFR |= (1 << INTF3); //pin18
  //EIFR |= (1 << INTF2); //pin19
  //EIFR |= (1 << INTF1); //pin20

  attachInterrupt(digitalPinToInterrupt(motorInt1), motorInterrupt1, LOW); //interrupt occured on HIGH to LOW
  attachInterrupt(digitalPinToInterrupt(motorInt2), motorInterrupt2, LOW);
  attachInterrupt(digitalPinToInterrupt(motorInt3), motorInterrupt3, LOW);

  //MultiStepper to manage, adding number of stepper motors
  steppers.addStepper(Stepper1);
  steppers.addStepper(Stepper2);
  steppers.addStepper(Stepper3);

}

void loop() {
  // reply only when you receive data:
  if (Serial3.available()) {
    Serial3Call();
  }
}

void Serial3Call() {
  String recData, temp;

  long num;
  recData = Serial3.readString();
  //***********Read user data (debug purpose)
  //Serial.print("Received Data: ");
  //Serial.println(recData);
  //int l = recData.length();
  //Serial.print("Legth Data: ");
  //Serial.println(l);
  //******************************************
  
  //SET VALUE TO MOTOR (WRITE)
  if ((recData.length()) > 1) {
    if ((recData[0] == 'M') || (recData[0] == 'm')) {
      //Serial3.print("M VALUE: ");
      recData.remove(0, 1);
      num = recData.toInt();
      //Serial3.println(num);
    }

    //setting acceleration
    else if ((recData[0] == 'A') || (recData[0] == 'a')) {
      //Serial3.print("Set Acceleration: ");
      recData.remove(0, 1); //remove white spaces?
      setAccl = recData.toInt(); //String to integer conversion
      //Serial3.print(setAccl);
      Stepper1.setAcceleration(setAccl);
      Stepper2.setAcceleration(setAccl);
      Stepper3.setAcceleration(setAccl);
    }

    //Stepper_motor publish data on arduino in "G100A100B-490C" pattern
    else if ((recData[0] == 'G') || (recData[0] == 'g')) {
      //Extracting input string and getting degree of 3 motors
      int GI = recData.indexOf('G');
      int AI = recData.indexOf('A');
      int BI = recData.indexOf('B');
      int CI = recData.indexOf('C');

      String s1 = recData.substring(GI + 1, AI);
      String s2 = recData.substring(AI + 1, BI);
      String s3 = recData.substring(BI + 1, CI);

      //individual stepper motor value extracted
      step1 = s1.toInt();
      step2 = s2.toInt();
      step3 = s3.toInt();

      Serial.print("Step1: ");
      Serial.println(step1);
      Serial.print("Step2: ");
      Serial.println(step2);
      Serial.print("Step3: ");
      Serial.println(step3);

      steps[0] = step1;
      steps[1] = step2;
      steps[2] = step3;

      controlStepper(steps);
    }

    //set frequency
    else if ((recData[0] == 'S') || (recData[0] == 's')) {
      //Serial3.print("Set Frequency: ");
      recData.remove(0, 1);
      setFrq = recData.toInt();
      Stepper1.setMaxSpeed(setFrq);
      Stepper2.setMaxSpeed(setFrq);
      Stepper3.setMaxSpeed(setFrq);
    }
    
    //TEST triangle motion (fixed value), send integer value from Jetson nano on testTriangle topic
    else if ((recData[0] == 'T') || (recData[0] == 't')) {
      //Extracting input string and getting degree of 3 motors
      int len = recData.length();
      String s1 = recData.substring(1, len);

      int loopCount = s1.toInt();

      Serial.print("Number of Times ");
      Serial.println(loopCount);

      steps[0] = round((-65)*(MICROSTEP/360));
      steps[1] = round((-65)*(MICROSTEP/360));
      steps[2] = round((-65)*(MICROSTEP/360));
      controlStepper(steps);
      delay(1000);

      controlStepper(steps);
    }
  }

  //GET VALUE FROM MOTOR (READ)
  else if ((recData.length()) == 1) {
    char label = 'X';
    label = recData[0];

    if (label == 'M') {
      Serial3.print("in case M");
    }
    else if (label == 'A') {
      String valA;
      valA = 'A' + setAccl;
      Serial3.print(valA);
    }
    else if (label == 'S') {
      String valS;
      valS = 'S' + setFrq;
      Serial3.print(valS);
    }
    //Jetson nano send 'X' first while launching ros2 to set default speed and position setting
    else if (label == 'X') {
      //Serial.println("in X");
      defaultValue();
    }
    else if (label == 'G') {
      Serial3.print('G');
      Serial3.print(steps[0]);
      Serial3.print(steps[1]);
      Serial3.println(steps[2]);
    }
  }  
}

void defaultValue() {
  setFrq = 800;
  setAccl = 1;

  Stepper1.setMaxSpeed(setFrq);
  Stepper2.setMaxSpeed(setFrq);
  Stepper3.setMaxSpeed(setFrq);

  Stepper1.setAcceleration(setAccl);
  Stepper2.setAcceleration(setAccl);
  Stepper3.setAcceleration(setAccl);

  //stepper motor start moving upwards to trigger LIMIT SWITCH 
  //and getting initial position of stepper motor 
  while (1) {
    steppers.moveTo(stepss);
    steppers.run();
    if (flag == 3) {
      Serial.println("Flag set!!");
      //flag = 0;
      break;
    }
  }
}

void controlStepper(long steps)
{
  /*
    Stepper1.moveTo(-800);
    while(Stepper1.isRunning())
    {
    Stepper1.run();
    }
    Stepper1.setCurrentPosition(0);

    Serial3.println("Motor 1 running");
  */
  /*
      Serial3.print("Step1: ");
      Serial3.println(step1);
      Serial3.print("Step2: ");
      Serial3.println(step2);
      Serial3.print("Step3: ");
      Serial3.println(step3);
  */
  /*long stepss[3] = {};
    stepss[0] = 800.0;
    stepss[1] = 1600.0;
    stepss[2] = 400.0;
    //stepss = {800.0, 800.0, 800.0};*/

  //sending pulses to stepper motor driver
  steppers.moveTo(steps);
  steppers.runSpeedToPosition();
  //Serial3.print("T");
  //Stepper1.setCurrentPosition(0);
  //Stepper2.setCurrentPosition(0);
  //Stepper3.setCurrentPosition(0);
}


void motorInterrupt1() {
  Serial.println("In interrupt 1");
  stepss[0] = 0;
  Stepper1.setCurrentPosition(0);
  flag++;
  detachInterrupt(digitalPinToInterrupt(motorInt1));
}

void motorInterrupt2() {
  Serial.println("In interrupt 2");
  stepss[1] = 0;
  Stepper2.setCurrentPosition(0);
  flag++;
  detachInterrupt(digitalPinToInterrupt(motorInt2));
}

void motorInterrupt3() {
  Serial.println("In interrupt 3");
  stepss[2] = 0;
  Stepper3.setCurrentPosition(0);
  flag++;
  detachInterrupt(digitalPinToInterrupt(motorInt3));
}
