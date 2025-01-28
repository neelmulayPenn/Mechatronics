//Team 23 
//Neel Mulay, Brian Fok, Joseph Masica
//Code for the website and the PID control is here in these files
#include <WiFi.h> 
#include <WiFiUDP.h>
#include "html510.h"
#include "body.h"
// encoders
#define ENCODER_A 5  
#define ENCODER_B 4
// PWM signals
#define PWM_A 0     
#define PWM_B 1
// direction of motor drive
#define DIR_A 6
#define DIR_B 7
// PWM Setup
#define FREQUENCY 200
#define RESOLUTION 14
#define FULL_DUTY_CYCLE ((1 << 14) - 1)
#define ON_SPEED (((1 << 14) - 1) / 6)
#define TURN_SPEED (((1 << 14) - 1) / 4)
#define OFF_SPEED 0

// PID control constants
#define KP 800         
#define KI 0

const char* ssid = "FastCar";
float errorIntegralA = 0, errorIntegralB = 0;
int directionFlag = -1;
HTML510Server h(80);
WiFiServer server(80);   

void handleRoot();
void handleDirection();
int number_of_counts(uint8_t encoderPin);
int p_control(uint8_t desiredCount, uint8_t encoderPin, float& errorIntegral);

void setup() {

  Serial.begin(115200);   

  pinMode(ENCODER_A, INPUT); 
  pinMode(ENCODER_B, INPUT); 
  pinMode(DIR_A, OUTPUT);
  pinMode(DIR_B, OUTPUT);

  ledcAttach(PWM_A, FREQUENCY, RESOLUTION);
  ledcAttach(PWM_B, FREQUENCY, RESOLUTION);

  WiFi.mode(WIFI_AP);          
  WiFi.softAP(ssid, ""); // no password   

  Serial.printf("Connecting to %s\n", ssid);
  Serial.print(F("AP IP address: "));  
  Serial.println(WiFi.softAPIP());

  h.begin();
  h.attachHandler("/", handleRoot);
  h.attachHandler("/sendDirection?value=", handleDirection);
}

void loop() {
  h.serve();

  // Serial.print("Left: ");
  // number_of_counts(ENCODER_A);
  // Serial.print(" Right: ");  
  // number_of_counts(ENCODER_B);
  // Serial.println();

  if (directionFlag == 1 || directionFlag == 4) {
    // Serial.print("Left | ");
    ledcWrite(PWM_A, p_control(60, ENCODER_A, errorIntegralA));
    // Serial.print("Right | ");  
    ledcWrite(PWM_B, p_control(62, ENCODER_B, errorIntegralB));
    // Serial.println();
  }

  // delay(10);
}



void handleRoot() {
  h.sendhtml(body);
}

void handleDirection() {
  // change the duty - the new duty cycle is read from the website's slider input
  int direction = h.getVal();
  directionFlag = direction;
  // The duty cycle is mapped because at lower duty cycle values, the motor does not start because it has to overcome friction.
  if (direction == 1) {
    Serial.println(F("Forward")); 

    digitalWrite(DIR_A, LOW);
    digitalWrite(DIR_B, LOW);

    ledcWrite(PWM_A, ON_SPEED);
    ledcWrite(PWM_B, ON_SPEED);
  } else if (direction == 4) {
    Serial.println(F("Backward")); 

    digitalWrite(DIR_A, HIGH);
    digitalWrite(DIR_B, HIGH);

    ledcWrite(PWM_A, ON_SPEED);
    ledcWrite(PWM_B, ON_SPEED);
  } if (direction == 3) {
    Serial.println(F("Right")); 

    digitalWrite(DIR_A, LOW);
    digitalWrite(DIR_B, LOW);

    ledcWrite(PWM_A, TURN_SPEED);
    ledcWrite(PWM_B, OFF_SPEED);
  } if (direction == 2) {
    Serial.println(F("Left")); 

    digitalWrite(DIR_A, LOW);
    digitalWrite(DIR_B, LOW);

    ledcWrite(PWM_A, OFF_SPEED);
    ledcWrite(PWM_B, TURN_SPEED);
  } if (direction == 0) {
    Serial.println(F("Stop"));

    digitalWrite(DIR_A, HIGH);
    digitalWrite(DIR_B, LOW);

    ledcWrite(PWM_A, OFF_SPEED);
    ledcWrite(PWM_B, OFF_SPEED);
  }
  errorIntegralA = 0;
  errorIntegralB = 0;
  delay(10);
}

int number_of_counts(uint8_t encoderPin) {
  int lastTime = millis(), pulseCount = 0;
  bool currentState, lastState;

  while ( ( millis() - lastTime ) < 50) {
    currentState = digitalRead(encoderPin); 
    if (currentState != lastState) { 
      pulseCount++; 
    }
    lastState = currentState; 
  }

  // Serial.print(pulseCount);
  return pulseCount;
}

int p_control(uint8_t desiredCount, uint8_t encoderPin, float& errorIntegral) { 
  int error, dutyCycle;

  error = desiredCount - number_of_counts(encoderPin);
  errorIntegral += error;

  // Serial.print(F("Counts: "));
  // Serial.print(number_of_counts(encoderPin));
  // Serial.print(F(" Error: "));
  // Serial.print(error);

  dutyCycle += ( ( KP * error ) + ( KI * errorIntegral ) + ON_SPEED);
  dutyCycle = constrain(dutyCycle, 2500, 15000);

  // Serial.print(F(" New Duty Cycle: "));
  // Serial.print(dutyCycle);
  // Serial.println();

  return (int)dutyCycle;
}