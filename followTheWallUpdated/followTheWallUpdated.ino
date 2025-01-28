#include "Adafruit_VL53L0X.h"

// I2C Setup: set SCL and SDA pins (need to be pulled up)
#define SCL 9
#define SDA 8

// TOF sensor values
// set custom addresses for TOF sensors (between 0x30 and 0x3F if possible)
#define FRONT_TOF_ADDRESS 0x30
#define RIGHT_TOF_ADDRESS 0x31

// set XSHUT pins for assigning addresses (need to be initialized as LOW)
#define FRONT_XSHUT 18
#define RIGHT_XSHUT 19

// PWM and motor values
// Pins for the encoder signals
#define ENCODER_A 5 
#define ENCODER_B 4
// Left Motor
#define DIR_A 6
// Right Motor
#define DIR_B 7
// PWM Setup
#define PWM_PIN_A 0      
#define PWM_PIN_B 1
#define FREQUENCY 200
#define RESOLUTION 14
#define FULL_DUTY_CYCLE ((1 << 14) - 1)
#define ON_SPEED (((1 << 14) - 1) / 6.0)
#define OFF_SPEED 0

// set global objects representing sensors
Adafruit_VL53L0X frontTOF = Adafruit_VL53L0X();
Adafruit_VL53L0X rightTOF = Adafruit_VL53L0X();

// this holds the measurement (required for sensor)
VL53L0X_RangingMeasurementData_t frontMeasure;
VL53L0X_RangingMeasurementData_t rightMeasure;

// placeholder variables
uint16_t frontDistance;
uint16_t rightDistance;

// function inits (C formality)
void setID();
uint16_t readSensor(Adafruit_VL53L0X& tof, VL53L0X_RangingMeasurementData_t& measure);
void followWall(uint16_t rightDistance);
void leftAdjust();
void rightAdjust();
void leftTurn();
void leftDrive();
void frontDrive();
void rightDrive();
void backDrive();
void stop();


void setup() {
  // serial setup
  Serial.begin(115200);
  while (! Serial) { delay(1); }

  // I2C Setup
  pinMode(SCL, INPUT);
  pinMode(SDA, INPUT);
  Wire.begin(SDA, SCL);

  // sensor setup
  pinMode(FRONT_XSHUT, OUTPUT);
  pinMode(RIGHT_XSHUT, OUTPUT);

  // motor setup 
  pinMode(ENCODER_A, INPUT); 
  pinMode(ENCODER_B, INPUT); 
  pinMode(DIR_A, OUTPUT);
  pinMode(DIR_B, OUTPUT);

  // pwm setup
  ledcAttach(PWM_PIN_A, FREQUENCY, RESOLUTION);
  ledcAttach(PWM_PIN_B, FREQUENCY, RESOLUTION);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(FRONT_XSHUT, LOW);
  digitalWrite(RIGHT_XSHUT, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  Serial.println(F("Starting..."));

  setID();
}

void loop() {
  Serial.print(F("Forward: "));
  frontDistance = readSensor(frontTOF, frontMeasure);
  Serial.println(frontDistance);
  //delay(500);
  Serial.print(F("Right: "));
  rightDistance = readSensor(rightTOF, rightMeasure);
  Serial.println(rightDistance);
  delay(100);

  // if (frontDistance <=500) leftTurn();
  followWall(rightDistance);
  
  //Placeholder code - to control the robot using the website 
  //if button on the website is pressed to go into manual mode
  //default mode is autonomous 
  // then go into manual drive
  //Same as lab 4 code
  //if autonomous button is pressed then go into autonomous mode
  // Every command given through WiFi - increment a counter which counts the number of WiFi packets sent

}

void setID() {
  // all reset
  digitalWrite(FRONT_XSHUT, LOW);    
  digitalWrite(RIGHT_XSHUT, LOW);
  delay(10);
  // all unreset
  digitalWrite(FRONT_XSHUT, HIGH);
  digitalWrite(RIGHT_XSHUT, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(FRONT_XSHUT, HIGH);
  digitalWrite(RIGHT_XSHUT, LOW);

  // initing LOX1
  if(!frontTOF.begin(FRONT_TOF_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(RIGHT_XSHUT, HIGH);
  delay(10);

  //initing LOX2
  if(!rightTOF.begin(RIGHT_TOF_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

uint16_t readSensor(Adafruit_VL53L0X& tof, VL53L0X_RangingMeasurementData_t& measure) {
  tof.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  if(measure.RangeStatus != 4) {     // if not out of range
    Serial.print(measure.RangeMilliMeter);
  } else {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  return measure.RangeMilliMeter;
}

void leftTurn() {
  leftDrive();
  delay(500);
  stop();
}

void followWall(uint16_t rightDistance) {

  if (rightDistance <= 200) {
    // turn 30 degrees left
    // move forward for 100ms
    rightAdjust();
  } 
  else if (rightDistance >= 350) {
    // turn 30 degrees right
    // move forward for 100ms
    leftAdjust();
  } 
  else
  forwardDrive();

  //if(forwardDistance <=200 && rightDistance<=200)
  //{
    //stop();
    //leftTurn();
    //stop();
  //}
}
void leftAdjust() {
  leftDrive(6.5);
  delay(25);
  //stop();
}

void rightAdjust() {
  rightDrive(6.5);
  delay(25);
  //stop();
}

void leftDrive(int count) {
  // LEFT
  Serial.println(F("Left")); 
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, HIGH);

  int speed = ON_SPEED*6;
  int speed_given = speed/count;
  ledcWrite(PWM_PIN_A, speed_given);
  ledcWrite(PWM_PIN_B, OFF_SPEED);
}

void backDrive() {
  // FORWARD
  Serial.println(F("Backward")); 
  digitalWrite(DIR_A, HIGH);
  digitalWrite(DIR_B, HIGH);

  ledcWrite(PWM_PIN_A, ON_SPEED);
  ledcWrite(PWM_PIN_B, ON_SPEED);
}

void rightDrive(int count) {
  // RIGHT 
  Serial.println(F("Right")); 
  digitalWrite(DIR_A, HIGH);
  digitalWrite(DIR_B, LOW);

  int speed = ON_SPEED*6;
  int speed_given = speed/count;
  ledcWrite(PWM_PIN_A, OFF_SPEED);
  ledcWrite(PWM_PIN_B, speed_given);
}

void forwardDrive() {
  // BACK
  Serial.println(F("Forward")); 
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);

  ledcWrite(PWM_PIN_A, ON_SPEED);
  ledcWrite(PWM_PIN_B, ON_SPEED);
}

void stop() {
  // STOP
  Serial.println(F("Stop")); 
  digitalWrite(DIR_A, LOW);
  digitalWrite(DIR_B, LOW);

  ledcWrite(PWM_PIN_A, OFF_SPEED);
  ledcWrite(PWM_PIN_B, OFF_SPEED);
}