#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <rom/rtc.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <FastCRC.h>
#include "FS.h"
#include "SPIFFS.h"
#include <iostream>
#include <string.h>
#include <functional>
#include <bits.h>
#include <bits/stdc++.h>
//#include <conio.h>
// #include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "soc/rtc.h"
#include "esp_bt.h"
#include "sample_message.h"

uint8_t spiffs_read_data[50000];
String spiffs_read_string ((char *)0);
int numberOfPacket; 
hw_timer_t  *timer_w = NULL;
// ++ bool        time_out_trig = 0;
DRAM_ATTR int interruptCounter = 0;

uint8_t wakeup_reason = 0;

String  sqlite3_return_string = "";
int     sqlite_return_val;

//String  esp_now_incoming_message = "";
String  esp_now_incoming_string = "";
bool    esp_now_incoming_trigger = 0;
  uint8_t esp_now_incoming_integer;

int incomingdata;

StaticJsonDocument<200> doc;
StaticJsonDocument<220> doc1;
StaticJsonDocument<220> doc2;

//#include "gpio_init.h"
//#include "utils.h"
//#include "i2c_local.h"
#include "config.h"
#include "serial_debug.h"
#include "device_config.h"
#include "esp_now_local.h"
#include "esp_sleep_local.h"
//#include "LC709203F.h"
#include "crc_local.h"
// #include "memory.h"
// #include "display_options.h"

char buf[60000];

uint8_t ack_send_status = 0;
    uint16_t time1, time2;
    bool esp_now_receiving = 1;
    String Packet;
    String PacketNumber;
    String esp_now_incoming_message;
    String esp_now_incoming_message1;
    String Cnumber;
    int ReceverPacket = 0;
    int sendernumber;
    String Snumber;
    String Mpacket;
    String NRpacket;
    int FileSize;
    //int Snum1;
    //esp_now_incoming_message.reserve(50000);
    //bool status = 0;
    // uint16_t status_update_code = 0;
     int massageLenth;
   // uint16_t status_update_code = 0;
    //String substring((char *)0);

    //char char_temp;
    //uint16_t char_count = 0;

// Watchdog iiteruupt function
/*void IRAM_ATTR resetModule() {
    Serial.println("wdt reboot");
    delay(100);
    ESP.restart();
}*/

void setup() {

// Initialize Serial Monitor
    Serial.begin(115200);

//Initialize the SPIFFS object
    if(!SPIFFS.begin(true)){
      Serial.println("Error initialize the SPIFFS");
      while(true){}
    }

// Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.println("chip_id: " + get_chip_id());

// Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

// Once ESPNow is successfully Init, we will register for Send CB to
// get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
  
// Register peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
// Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
// Register for a callback function that will be called when data is received 
//esp_now_register_recv_cb(OnDataRecv1);
esp_now_register_recv_cb(OnDataRecv);
   
}

void loop() {  //1
//...............send json format to slave id no master device......................//
    doc["slave_id"] = "001";
    String json_message = "";
    serializeJson(doc, json_message);

// CRC generation    
    uint16_t CRC16_read = get_crc16(json_message);
    char crc_buf[6];
    sprintf(crc_buf, "%5d", CRC16_read);
    String esp_now_send_message_a;
    esp_now_send_message_a += String(crc_buf);
    esp_now_send_message_a += json_message;
    
    Serial.print("esp_now_send_message_a.length > ");
    Serial.println(esp_now_send_message_a.length());
    Serial.println("esp_now_sending>" + esp_now_send_message_a);
    
    
    esp_now_incoming_message1.reserve(50000);
   

    if(esp_now_send_message(broadcastAddress, esp_now_send_message_a)) {  //2
        time1 = millis(); 
        while(esp_now_receiving) {  //3
            while(1) {  //4
                time2 = millis() - time1;
                if(esp_now_incoming_trigger) {   //5
                    // esp_now_incoming_trigger = 0;
                    break;
                } //5
                if(time2 > 10000) { //6
                    break;
                }  //6
            } //4

            if(esp_now_incoming_trigger) { //7
                esp_now_incoming_trigger = 0;
                
                // uint8_t esp_now_incoming_string_length = esp_now_incoming_string.length();
                if(!esp_now_incoming_string.endsWith("1")) { //8
                    esp_now_receiving = 0;
                }  //8

//..........................recever file data slow................................//
                deserializeJson(doc1, esp_now_incoming_string);
                JsonObject json_obj1 = doc1.as<JsonObject>();
                String read_str2 = json_obj1["P"];
                String read_str4 = json_obj1["AP"];
                String read_str3 = json_obj1["PN"];
                String fieSize = json_obj1["F"];
               
       
       
                ReceverPacket++; //count data packet
                Packet = read_str4; //all file size 
                
              Serial.print("RP:");
                Serial.print(ReceverPacket);
                Serial.print("\t");
                Serial.print("SP:");
                Serial.print(read_str3);
                Serial.print("\t");
                Serial.print(read_str2);
                //
                Serial.print("\n");

                 sendernumber = read_str2.toInt();
                 Snumber += read_str3 + ",";
//delay(50);
                 esp_now_incoming_message1 += read_str2;
 FileSize = fieSize.toInt();
//7
            }else { //*
                Serial.println("ESP_NOW not received");
                esp_now_receiving = 0;
            } //*
        }  //3

        //delay(100);
    } //2
             
      int SenderPacket;
    // String convert to int 
    SenderPacket = Packet.toInt();
     Serial.println(SenderPacket); 

//....................all packets string data cnvert to int array...........//
     int repetrarray[SenderPacket-1];
    int r=0,t=0;
  
for(int i=0;i<Snumber.length();i++)
{ //*1
    if(Snumber.charAt(i) == ',')
    { //*2
    String tex = Snumber.substring(r,i);
      repetrarray[t] = tex.toInt();
        r = (i+1);
      t++;
    } //*2
  } //*1

Serial.print("Number of packets received: ");
//send packet array 
for(int k=0; k<=t-1; k++)
{ //*3
    Serial.print(",");
    Serial.print(repetrarray[k]);
}  //*3
  Serial.print("\n");

//remove duplicate packets from send packet 
//......................no repete mising no..............................//
Serial.print("non repeat packet: ");  
// find non repet packet
int k = 0;
while (k<=t-1){ //*4
//int w= 1;
for(int j= k; j <= t-1; j++){ //66
 if(repetrarray[k] != repetrarray[j+1]){
  NRpacket += String(repetrarray[k]) + ",";
  }
  k++;
 }}
 Serial.print(NRpacket);
Serial.print("\n");
//....................remove duplicate packets string data cnvert to int array...........//
     int missigarray[SenderPacket-1];
    int x=0,z=0;
  
for(int i=0;i<NRpacket.length();i++)
{ //*1
    if(NRpacket.charAt(i) == ',')
    { //*2
    String tex1 = NRpacket.substring(x,i);
      missigarray[z] = tex1.toInt();
        x = (i+1);
      z++;
    } //*2
  } //*1

  Serial.print("Number of on repete packets received: ");
//send packet array 
for(int k1=0; k1<=z-1; k1++)
{ //*3
    Serial.print(",");
    Serial.print(missigarray[k1]);
}  //*3
  Serial.print("\n");

  //......................find mising no..............................//
Serial.print("Missing packet: ");  
// find missing packet*/
int k1 = 0;
while (k1<=z-1){ //*4
//for(int k1 = 0; k1<=z-1; k++){
 for(int w= 1; w <= SenderPacket; w++){ //*5
     if(missigarray[k1] == w){  //*6
         k1++; //*6
     }else{  //*7
      //   Serial.print(",");
      //   Serial.print(w);
            Mpacket += String(w) + ",";
     }  //*7  
      
    } //*5

    } //*4 
//delay(200);

//........................find all missing no convert to string..............//
    Serial.print("\n");
        Serial.print(Mpacket);
        Serial.print("\n");
        Serial.println(Mpacket.length());
    //int Snum = Cnumber.toInt();
   
    Serial.print("Recever message Size: ");
    massageLenth = esp_now_incoming_message1.length();
    Serial.println(massageLenth);
    Serial.print("Recever Packets: ");
    Serial.println(ReceverPacket);
    Serial.print("sender Packets: ");
    Serial.println(SenderPacket);
    Serial.print("Recever loss Data packet: ");
    double lossPackets = SenderPacket - ReceverPacket;
    Serial.println(lossPackets);

    deep_sleep_for_seconds(10);
}//1
//void loop() { }