#include <ESP8266WiFi.h>
#include <ctype.h>

extern "C" {
  #include "user_interface.h"
}

char ssid[] = "yourssidhere";
byte channel;
uint8_t packet[128];

void setup() {
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1);
}

void loop() {
  int bit = 0;
  channel = random(1,12); 
  wifi_set_channel(channel);
  // Packet bauen
  // Frame Control (0-1)
  packet[bit] = 0x80; bit++;
  packet[bit] = 0x00; bit++;
  // Duration (2-3)
  packet[bit] = 0x00; bit++;
  packet[bit] = 0x00; bit++;
  // Ziel MAC (Broadcast FF:FF:FF:FF:FF:FF)
  packet[bit] = 0xff; bit++;
  packet[bit] = 0xff; bit++;
  packet[bit] = 0xff; bit++;
  packet[bit] = 0xff; bit++;
  packet[bit] = 0xff; bit++;
  packet[bit] = 0xff; bit++;
  // Random Quell MAC / BSSID
  packet[bit] = packet[bit + 6] = random(256); bit++;
  packet[bit] = packet[bit + 6] = random(256); bit++;
  packet[bit] = packet[bit + 6] = random(256); bit++;
  packet[bit] = packet[bit + 6] = random(256); bit++;
  packet[bit] = packet[bit + 6] = random(256); bit++;
  packet[bit] = packet[bit + 6] = random(256); bit++;
  bit = bit + 6;
  // Seq Number
  packet[bit] = 0xc0; bit++;
  packet[bit] = 0x6c; bit++;
  // Beacon Timestamp
  packet[bit] = 0x83; bit++;
  packet[bit] = 0x51; bit++;
  packet[bit] = 0xf7; bit++;
  packet[bit] = 0x8f; bit++;
  packet[bit] = 0x0f; bit++;
  packet[bit] = 0x00; bit++;
  packet[bit] = 0x00; bit++;
  packet[bit] = 0x00; bit++;
  // Beacon Interval
  packet[bit] = 0x64; bit++;
  packet[bit] = 0x00; bit++;
  // Capability Information
  packet[bit] = 0x01; bit++;
  packet[bit] = 0x04; bit++;
  // SSID
  packet[bit] = 0x00; bit++;
  packet[bit] = sizeof(ssid); bit++;
  
  int i = 0;
  do {
     char one_char = ssid[i];
     int rand = random(1,3);
     if (rand == 1)
     {
      packet[bit] = toupper(one_char); bit++;
     }
     else if (rand == 2)
     {
      packet[bit] = tolower(one_char); bit++;
     }
  } while (i++ != sizeof(ssid));
  
  // Supported Rates
  packet[bit] = 0x01; bit++;
  packet[bit] = 0x08; bit++;
  packet[bit] = 0x82; bit++;
  packet[bit] = 0x84; bit++;
  packet[bit] = 0x8b; bit++;
  packet[bit] = 0x96; bit++;
  packet[bit] = 0x24; bit++;
  packet[bit] = 0x30; bit++;
  packet[bit] = 0x48; bit++;
  packet[bit] = 0x6c; bit++;
  // ?
  packet[bit] = 0x03; bit++;
  packet[bit] = 0x01; bit++;
  //Channel
  packet[bit] = channel; bit++;

  wifi_send_pkt_freedom(packet, bit, 0);
  wifi_send_pkt_freedom(packet, bit, 0);
  wifi_send_pkt_freedom(packet, bit, 0); 
  
  delay(1);
}
