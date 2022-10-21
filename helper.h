// Start the Serial Monitor and enter: x

/****TESTING DEVICES****/
// led - Test 8x8 
// imu - Test IMU 
// li2 - Test lidar 2m
// dgr - Test digital line sensors
// anr - Test analog line sensors 
// mot - Test motors
// msh - Motor short test
// ser - Test servo 

/*****CALIBRATING DEVICES*****/
// lic - Calibrating lidar
// cal - Calibrating line sensors

/*******MAIN FUNCTIONS********/
// go(left motors speed, right motors speed) - used to run the robot, values from -127 to 127
// line(sensor number) - if the sensor sees the line the value is true
// front() - reads the distance from the front LIDAR sensor
// frontLeft() - reads the distance from the front left LIDAR sensor 
// frontRight() - reads the distance from the front right LIDAR sensor 
// pitch() - reads the inclination of the robot, negative number represents that the robot is climbing and positive that is decending 
// servo(angle, servo number) - 0 is the big servo, 1 is the left finger servo, 2 is the right finger servo
// delayMs(delay in miliseconds) - Used with go to keep the motors runing
// display("string") - Dispalys a string on a 8x8 mrm display
// lineAny() - true if any line sensor detects a line
// end() - ends all the robots behaviour 
// stop() - stops the motors, usually combined with delayMs 

#pragma once
#include <mrm-8x8a.h>
#include <mrm-col-can.h>
#include <mrm-fet-can.h>
#include <mrm-imu.h>
#include <mrm-lid-can-b.h>
#include <mrm-lid-can-b2.h>
#include <mrm-mot4x3.6can.h>
#include <mrm-mot4x10.h>
#include "mrm-robot-line.h"
#include <mrm-servo.h>
#include <mrm-therm-b-can.h>

void RobotLine::wallFollowRight() {
  // Praćenje zida s desne strane
  if (frontRight() < 200)
  {
    if (front() < 130) {
      go(-70, 70);        // Skretanje u lijevu stranu
      delayMs(600);
    }
    if (frontRight() < 100) {
      go(30, 70);
    }
    else
      go(70, 30);
  }
  else
    go(60, 60);
}

void RobotLine::wallFollowLeft() {
  // Praćenje zida s lijeve strane
  if (frontLeft() < 200)
  {
    if (front() < 130) {
      go(70, -70);        // Skretanje u desnu stranu
      delayMs(600);
    }
    if (frontLeft() < 100) {
      go(70, 30);
    }
    else
      go(30, 70);
  }
  else
    go(60, 60);
}

void RobotLine::catchBall() {
  // Rutina za hvatanje lopte
  stop();
  delayMs(50);
  armCatchReady();        // Priprema grippera za hvatanje

  go(40, 40);             // Polako prilaženje loptici
  delayMs(2000);
  go(20, 20);
  delayMs(1000);

  armCatch();             // Hvatanje loptice
  stop();
  delayMs(50);

  go(-40, -40);           // Izlazak iz zone čuvanja loptice vožnjom unazad
  delayMs(1500);
  stop();
  delayMs(50);

  go(70, -70);            // Okretanje u mjestu za 180°
  delayMs(1300);
  stop();
  delayMs(50);

  go(70, 70);             // Prelazak raskrižja
  delayMs(500);
}


/*
        OVAJ DIO MORAJU SAMI NAPRAVITI
*/
void RobotLine::wallFollowRightCarry() {
  // Praćenje zida s desne strane nakon hvatanja lopte
  if (frontRight() < 200)
  {
    if (front() < 130) {
      go(-70, 70);        // Skretanje u lijevu stranu
      delayMs(600);
    }
    if (frontRight() < 160) {
      go(30, 70);
    }
    else
      go(70, 30);
  }
  else
    go(60, 60);
}

void RobotLine::wallFollowLeftCarry() {
  // Praćenje zida s lijeve strane nakon hvatanja lopte
  if (frontLeft() < 200)
  {
    if (front() < 130) {
      go(-70, 70);        // Skretanje u lijevu stranu
      delayMs(600);
    }
    if (frontLeft() < 160) {
      go(70, 30);
    }
    else
      go(30, 70);
  }
  else
    go(60, 60);
}


/*
        OVAJ DIO JE DODATNI ZA EXTRA BOD
*/
void RobotLine::findingBall() {
  // Pakiranje dijela koda kada je lopta == false;
  if (lineAny()) {
    // Priprema robota za praćenje linije
    armClose();

    // Prepoznavanje raskrižja koji označava pronalazak lopte
    if (line(8) and line(4) and front() > 200) {
      catchBall();
      lopta = true;
    }

    // Praćenje linije
    lineFollow();
  }
  else {
    // Priprema robota za praćenje zida
    armOpen();

    // Praćenje zida
    wallFollowLeft();
    wallFollowRight();
  }
}

void RobotLine::carryingBall() {
  // Pakiranje dijela koda kada je lopta == true;
  // Ukoliko robot NOSI loptu ponašat će se prema sljedećim naredbama
  if (lineAny()) {
    // Prepozavanje raskrižja gdje robot ostavlja loptu
    if (line(8) and line(0) and front() < 200) {
      stop();
      armDrop();
      delayMs(100);
      end();
    }

    // Praćenje linije
    lineFollow();
  }
  else {
    // Praćenje zida dok drži loptu u gripperu
    wallFollowLeftCarry();
    wallFollowRightCarry();
  }
}
