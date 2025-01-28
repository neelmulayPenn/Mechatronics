/**
 * Simple sender  to only a receiver ESPNOW with known MAC hard coded to MAC_RECV
 SLAVE 

  RECIEVES FROM THE MASTER - 
  SENDS TO THE MASTER

*/


#include <esp_now.h>
#include <WiFi.h>

#define CHANNEL 1                  // channel can be 1 to 14, channel 0 means current channel.  
#define MAC_RECV  {0x68,0x67,0x25,0xB2,0xB5,0xE8} // RECIEVING from (last digit should be even for STA)
//SLAVE1 - 68:67:25:B2:CD:70 WITH THE BLACK DOT
//SLAVE2 - : 68:67:25:B4:B1:40  WIHTOUT THE BLACK DOT
//Car MAC Address - 68:67:25:B2:B5:E8 which is the master

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
  Serial.printf(" Recv from: %02x:%02x:%02x:%02x:%02x:%02x ",mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]); 
  Serial.print(" Data: "); 
  //Recieved number is stored in data
  if (data_len == sizeof(RecievedData)) {
    RecievedData packet;
    memcpy(&packet, data, sizeof(packet)); // Unpack data

    // Access individual items
    Serial.printf("WiFi Packets: %d\n", packet.wifi_packet);
    
    // ************************************* Send to Top Hat here
  } else {
    Serial.println("Invalid data length");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); 

  // ************************** Attacking using Servo

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
  // ************************************* GET HEALTH from Top Hat
  // ************************************* SEND HEALTH to Master
  SentData d1 = {1,2,3,4,100};
  sendPacket(d1);
   
}
