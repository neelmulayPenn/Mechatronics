/**
 * Simple sender  to only a receiver ESPNOW with known MAC hard coded to MAC_RECV
 SLAVE 

  RECIEVES FROM THE MASTER - 
  SENDS TO THE MASTER

*/

#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <ESP32Servo.h>
#define CHANNEL 7  // Change to your desired channel


const int servoPin = 18;
Servo myServo;

unsigned long previousMillis = 0;  // Will store the last time the servo position was updated
const long interval = 2;           // Interval at which to update the servo position (in milliseconds)
int servoPos = 0;                  // Current servo position
bool servoDirection = true;  

#define SDA_PIN 8
#define SCL_PIN 9

#define I2C_SLAVE_ADDR 0x28
#define CHANNEL 1                  // channel can be 1 to 14, channel 0 means current channel.  
#define MAC_RECV  {0x68,0x67,0x25,0xB2,0xB5,0xE8} // RECIEVING from (last digit should be even for STA)
//SLAVE1 - 68:67:25:B2:CD:70 WITH THE BLACK DOT
//SLAVE2 - : 68:67:25:B4:B1:40  WIHTOUT THE BLACK DOT
//Car MAC Address - 68:67:25:B2:B5:E8 which is the master

uint16_t lastTime = 0;

esp_now_peer_info_t peer1 = 
{
  .peer_addr = MAC_RECV, 
  .channel = CHANNEL,
  .encrypt = false,
};

typedef struct {
  int x1;     // First data item (e.g., sensor reading)
  int y1;   // Second data item (e.g., temperature)
  int x2;
  int y2;
  int health;
} SentData;

typedef struct {
  int wifi_packet;     // First data item (e.g., sensor reading)
} RecievedData;


// callback when data is sent 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) Serial.println ("Success ");
  else Serial.println("Fail "); 
}

//Slave recieves from master
void OnDataRecv(const esp_now_recv_info *mac_addr, const uint8_t *data, int data_len) {
  // Serial.printf(" Recv from: %02x:%02x:%02x:%02x:%02x:%02x ",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]); 
  Serial.print(" Data: "); 
  if (data_len == sizeof(RecievedData)) {
    RecievedData packet;
    memcpy(&packet, data, sizeof(packet)); // Unpack data

    // Access individual items
    Serial.printf("WiFi Packets: %d\n",packet.wifi_packet);
    send_I2C_byte(packet.wifi_packet);
  } else {
    Serial.println("Invalid data length");
  }
}

uint8_t recieve_I2C_byte() {  // data should have space declared from caller
  // Request data from slave
  uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 1);
  uint8_t byteIn = 0;

  if (bytesReceived > 0) {
    Serial.print("Received from slave: ");
    while (Wire.available()) {
      byteIn = Wire.read();
      Serial.printf("0x%02X ", byteIn);
    }
    Serial.println();
  } else return 0;

  //byteIn = Wire.read();
  //Serial.println(byteIn);
  return byteIn;  // return number of bytes read
}

//Sending wifi packets to the top hat
void send_I2C_byte(uint8_t data) {
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

void measure_time(uint8_t numOfPackets) {
  Serial.print("Clock Time: ");
  Serial.print(millis());
  Serial.print(" ");

  if ((millis() / 500) > lastTime) {
    send_I2C_byte(numOfPackets);
    lastTime = (millis() / 500);
    numOfPackets = 0;
  }
}

void updateServoPosition() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Check if it's time to update the servo position
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Update servo position
    if (servoDirection) {
      if (servoPos < 180) {
        servoPos++;  // Increment position
      } else {
        servoDirection = false;  // Change direction to decrease
      }
    } else {
      if (servoPos > 0) {
        servoPos--;  // Decrement position
      } else {
        servoDirection = true;  // Change direction to increase
      }
    }

    // Move the servo to the new position
    myServo.write(servoPos);
  }
}
//void servo_sweep
void setup() {
  pinMode(servoPin,  OUTPUT);
  myServo.attach(servoPin);
  Serial.begin(115200);

  //I2C setup
  Wire.begin(SDA_PIN, SCL_PIN, 40000);
  Serial.println("ESP32-C3 I2C Master initialized");
  Serial.printf("SDA: %d, SCL: %d\n", SDA_PIN, SCL_PIN);

  WiFi.mode(WIFI_STA);  
  WiFi.setChannel(CHANNEL);
  Serial.print("Sending MAC: "); Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) {
    Serial.println("init failed");
    ESP.restart();
  }
  
  esp_now_register_send_cb(OnDataSent); //optional if you want ack interrupt
    
  if (esp_now_add_peer(&peer1) != ESP_OK) {
    Serial.println("Pair failed");     // ERROR  should not happen
  }

  Serial.print("ESPNow Receiving MAC: ");  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
  esp_now_register_recv_cb(OnDataRecv);   
}

void sendPacket(SentData d1){

  if (esp_now_send(peer1.peer_addr, (uint8_t *)&d1, sizeof(d1))==ESP_OK) 
    Serial.printf("Sent data to %x:%x:%x:%x:%x:%x \n", peer1.peer_addr[0],peer1.peer_addr[1],peer1.peer_addr[2],peer1.peer_addr[3],peer1.peer_addr[4],peer1.peer_addr[5]);   
  else Serial.println("Send failed");
  
  delay(500); // ESPNow max sending rate (with default speeds) is about 50Hz

}

void loop() {
  updateServoPosition();

  int health = recieve_I2C_byte();

  if (health == 0) {
    SentData d1 = {1,2,3,4,health};
    sendPacket(d1);
  }
  while(health==0)
  { yte();
  }
}


