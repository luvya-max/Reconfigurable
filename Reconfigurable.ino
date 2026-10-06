#define REMOTEXY_MODE__WIFI_POINT

#include <WiFi.h>

#define REMOTEXY_MODE__WIFI_POINT
#define REMOTEXY_WIFI_SSID "Reconfigurable"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#include <RemoteXY.h>
#include <ESP32Servo.h>

Servo left;
Servo right;

// RemoteXY GUI configuration  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 98 bytes
  { 255,4,0,0,0,91,0,19,0,0,0,82,101,99,111,110,102,105,103,117,
  114,97,98,108,101,0,24,1,106,200,1,1,6,0,4,2,9,19,171,32,
  162,166,4,85,9,19,171,32,162,166,129,82,182,21,9,192,31,82,105,103,
  104,116,0,129,2,181,16,9,192,31,76,101,102,116,0,5,23,65,60,60,
  32,162,166,31,129,37,126,33,12,192,31,76,105,110,101,97,114,0 };

struct {
  int8_t l;  // 0 to 100
  int8_t r;  // 0 to 100
  int8_t x; // -100 to 100
  int8_t y; // -100 to 100
  uint8_t connect_flag;
} RemoteXY;
#pragma pack(pop)

// Motor driver pins
#define LEFT_FORWARD     33
#define LEFT_BACKWARD    32
#define RIGHT_FORWARD    14
#define RIGHT_BACKWARD   27

void setup() {
  RemoteXY_Init(); 
  left.attach(25);
  right.attach(26);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  Serial.begin(115200);
}

void loop() { 
  RemoteXY_Handler();

  // Servo control
  left.write((RemoteXY.l+100)*1.8/2);
  right.write((RemoteXY.r+100)*1.8/2);

  // Joystick input
  int x = RemoteXY.x;
  int y = RemoteXY.y;

  // Differential drive mixing
  int leftMotor = y + x;
  int rightMotor = y - x;

  // Clamp motor speeds
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
}