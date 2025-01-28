// ESP32-C3 I2C Master Code
// test code writes and receives

/* Every wifi command needs to increment a counter. 
 * There also needs to be a timer checker that checks for half a second of passed time.
 * Once half a second passes, the counter needs to be sent to I2C and reset to zero.
 * We also need to receive our health. If our health is below zero, everything must cease.
 * The Wifi packets can no longer be read.
 */

/* Every time handleDirection is called, increment numOfPackets.
 * Have a timer that checks for passing of half a second.
 * 
 */

#include <Wire.h>

#define I2C_SLAVE_ADDR 0x28 
#define SDA 5
#define SCL 4

// global variables
uint16_t lastTime = 0, health, numOfPackets = 0;
bool emergencyShutOff;

void measure_time(uint8_t);
void send_packets(uint8_t);
uint8_t receive_health();

void setup() {
  Serial.begin(115200);

  // Initialize I2C master
  // Wire.begin(SDA, SCL, 40000);
  Serial.println("ESP32-C3 I2C Master initialized");
  // Serial.printf("SDA: %d, SCL: %d\n", SDA_PIN, SCL_PIN);
}

void loop() {
  health = receive_health();
  if (health <= 0) emergencyShutOff = 1;
  else emergencyShutOff = 0;
  measure_time(numOfPackets);
}

uint8_t receive_health() {  // data should have space declared from caller
  // Request data from slave
  uint8_t dataReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 1);
  uint8_t health = 0;

  if (dataReceived > 0) {
    Serial.print("Received from slave: ");
    while (Wire.available()) {
      health = Wire.read();
      Serial.printf("0x%02X ", health);
    }
    Serial.println();
  } else return 0;
  return health;  // return number of bytes read
}

void measure_time(uint8_t numOfPackets) {
  Serial.print("Clock Time: ");
  Serial.print(millis());
  Serial.print(" ");

  if ((millis() / 500) > lastTime) {
    send_packets(numOfPackets);
    lastTime = (millis() / 500);
    numOfPackets = 0;
  }

  Serial.print(" Half seconds elapsed: ");
  Serial.print(lastTime);
  Serial.println();
}

void send_packets(uint8_t data) {
  // Send data to slave
  Wire.beginTransmission(I2C_SLAVE_ADDR);
  Wire.write(data);  // Send some test data
  uint8_t error = Wire.endTransmission();

  if (error == 0) {
    Serial.println("Data sent successfully");
    rgbLedWrite(2, 0, 20, 0);  // green
  } else {
    Serial.printf("Error sending data: %d\n", error);
    rgbLedWrite(2, 20, 0, 0);  // red
  }
}