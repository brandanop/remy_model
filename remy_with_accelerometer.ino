// This version of the code reads data from an MPU6050 accelerometer and maps it only to Remy's right arm.
// If you want to update the code work for both arms with two accelerometers, send me a PR!
// Note that the accelerometer uses I2C protocol with the Wire.h library.
#include <Servo.h>
#include <Wire.h>

Servo rightServo, leftServo;
float x_axis_acceleration;

void setup() {
  // Use a PWM pin on your board; I'm using a Metro Mini
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  leftServo.attach(5);
  rightServo.attach(7);
  rightServo.write(100);
  // Play around to find the appropriate angle range for your servo.
  // You can reattach the arm in a new position if necessary.
  // I found that a range from 90-180 made sense for the left arm,
  // and a range of 100-160 for the right arm.
  //  Use I2C
  Serial.begin(19200);
  Wire.begin();                      
  Wire.beginTransmission(0x68);       
  Wire.write(0x6B);                  
  Wire.write(0x00);                  
  Wire.endTransmission(true);     
}

void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);
  // Only measure vertical arm movement. You'll need to orient your accelerometer to match, or switch this to read from y/z. 
  x_axis_acceleration = (Wire.read() << 8 | Wire.read()) / 16384.0;


  // Set arbitrary thresholds to trigger movement based on observed Serial Monitor readings.
  Serial.println(x_axis_acceleration);
  if (x_axis_acceleration > 1.5) {
    // 160 is observed max servo angle
    rightServo.write(160);
    delay(400);
  }
  if (x_axis_acceleration < 0.4) {
    // 40 is observed min servo angle
    rightServo.write(40);
    delay(400);
  }
}