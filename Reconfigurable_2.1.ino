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
uint8_t RemoteXY_CONF[] =   // 59 bytes
  { 255,4,0,0,0,52,0,19,0,0,0,82,101,99,111,110,102,105,103,117,
  114,97,98,108,101,0,24,1,106,200,1,1,3,0,4,4,15,20,163,0,
  67,27,4,80,15,20,163,0,67,27,5,22,68,60,60,0,67,27,31 };

struct {
  int8_t x;  // 0 to 100
  int8_t y;  // 0 to 100
  int8_t jx; // -100 to 100
  int8_t jy; // -100 to 100
  uint8_t connect_flag;
} RemoteXY;
#pragma pack(pop)

// Motor driver pins
#define LEFT_FORWARD     25
#define LEFT_BACKWARD    26
#define RIGHT_FORWARD    14
#define RIGHT_BACKWARD   27

void setup() {
  RemoteXY_Init(); 
  left.attach(32);
  right.attach(33);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  Serial.begin(115200);
}

void loop() { 
  RemoteXY_Handler();

  // Servo control
  left.write(RemoteXY.x * 1.8);
  right.write(RemoteXY.y * 1.8);

  // Joystick input
  int jx = RemoteXY.jx;
  int jy = RemoteXY.jy;

  // Differential drive mixing
  int leftMotor = jy + jx;
  int rightMotor = jy - jx;

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

  // Debug output
  Serial.print("jx: "); Serial.print(jx);
  Serial.print(" | jy: "); Serial.print(jy);
  Serial.print(" | L: "); Serial.print(leftMotor);
  Serial.print(" | R: "); Serial.println(rightMotor);
}