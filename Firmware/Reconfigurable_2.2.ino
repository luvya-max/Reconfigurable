/*
   -- Reconfigurable_2 --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.15.01 or later version;
     - for iOS 1.12.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__WIFI_POINT

#include <WiFi.h>

// RemoteXY connection settings
#define REMOTEXY_WIFI_SSID "Reconfigurable_2"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


#include <RemoteXY.h>
#include <ESP32Servo.h>

Servo left;
Servo right;

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 128 bytes
  { 255,5,0,0,0,121,0,19,0,0,0,82,101,99,111,110,102,105,103,117,
  114,97,98,108,101,95,50,0,24,1,106,200,1,1,8,0,4,2,9,19,
  171,32,162,166,4,85,9,19,171,32,162,166,129,82,182,21,9,192,31,82,
  105,103,104,116,0,129,2,181,16,9,192,31,76,101,102,116,0,5,23,95,
  60,60,32,162,166,31,10,40,37,24,24,48,162,26,166,79,78,0,31,79,
  70,70,0,129,42,62,23,12,192,31,72,111,112,0,129,37,159,33,12,192,
  31,76,105,110,101,97,114,0 };

#define LEFT_FORWARD 26
#define LEFT_BACKWARD 25
#define RIGHT_FORWARD 14
#define RIGHT_BACKWARD 27

// this structure defines all the variables and events of your control interface
struct {

  // input variables
  int8_t l;       // from -100 to 100
  int8_t r;       // from -100 to 100
  int8_t x;       // from -100 to 100
  int8_t y;       // from -100 to 100
  uint8_t hop;  // =1 if state is ON, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() {
  RemoteXY_Init();
  left.attach(33);
  right.attach(32);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  Serial.begin(115200);


  // TODO you setup code
}

void loop() {
  RemoteXY_Handler();
  if (RemoteXY.hop == 0) {
    left.write((RemoteXY.l + 100) * 1.8 / 2);
    right.write((RemoteXY.r + 100) * 1.8 / 2);
  } 
  else {
    right.write(0);
    left.write(0);
    left.write(90);
    for(int i=90;i>=0;i--){
      left.write(i);
      delay(10);
    }
  }



  // Joystick inputd
  int x = RemoteXY.x;
  int y = RemoteXY.y;

  // Differential drive mixing
  int leftMotor = y - x;
  int rightMotor = y + x;
  leftMotor = constrain(leftMotor, -100, 100);
  rightMotor = constrain(rightMotor, -100, 100);

  // Convert -100~100 to 0~255 PWM
  if (leftMotor >= 0) {
    analogWrite(LEFT_BACKWARD, 0);
    analogWrite(LEFT_FORWARD, int(2.55 * leftMotor));
  } else {
    analogWrite(LEFT_FORWARD, 0);
    analogWrite(LEFT_BACKWARD, int(2.55 * -leftMotor));
  }

  if (rightMotor >= 0) {
    analogWrite(RIGHT_BACKWARD, 0);
    analogWrite(RIGHT_FORWARD, int(2.55 * rightMotor));
  } else {
    analogWrite(RIGHT_FORWARD, 0);
    analogWrite(RIGHT_BACKWARD, int(2.55 * -rightMotor));
  }


  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay()
}