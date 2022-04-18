#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <rBase64.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include "SPIFFS.h"   //have access to the methods needed both write and read from a file.
#include <FastCRC.h>

StaticJsonDocument<200> doc;
StaticJsonDocument<220> doc1;
StaticJsonDocument<220> doc2;

rBase64generic<2500> mybase64;
 bool esp_now_receiving = 1;
String  esp_now_incoming_string = "";
bool    esp_now_incoming_trigger = 0;
uint8_t esp_now_receive_address[6];
String  esp_now_incoming_string1 = "";
String  countM = "";
//esp_now_incoming_string1.reserve(50000);
 hw_timer_t  *timer_w = NULL;
//++ hw_timer_t  *timer_1 = NULL;
//++ bool        time_out_trig = 0;

//int count =1;
char buf[60000];
int numberOfPacket;
#include "config.h"
#include "serial_debug.h"
#include "esp_now_local.h"
#include "device_config.h"
#include "crc_local.h"
#include "image.h"
// #include "memory.h"
// #include "display_options.h"

uint16_t cycle_count = 0;
uint8_t number_of_slaves = 10;
uint8_t ack_status[10];
uint8_t ack_send_status = 0;
uint8_t ack_send_status1 = 0;
int inByte = 0;         // incoming serial byte

// Watchdog iiteruupt function
void IRAM_ATTR resetModule() {
    Serial.println("wdt reboot");
    delay(100);
    ESP.restart();
}

void setup() {

// Initialize Serial Monitor
    Serial.begin(115200);

// Set device as a Wi-F Station
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.println("chip_id: " + get_chip_id());
        
// Init ESP-NO
    if (esp_now_init() != ESP_OK) {
        log_message(DEBUG_ERROR, "ESPNOW: Fail to initialize ESP-NOW");
        return;
    }

    esp_now_register_send_cb(OnDataSent);
    
    esp_now_peer_info_t peerInfo;
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
   

    uint8_t slaveAddress_1[6];
    for(uint8_t i = 0; i < 8; i++) {
        for(uint8_t j = 0; j < 6; j++) {
            slaveAddress_1[j] = slaveAddress[i][j]; 
        }
        memcpy(peerInfo.peer_addr, slaveAddress_1, 6);
        Serial.println(slaveAddress_1[5]);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            log_message(DEBUG_ERROR, "ESPNOW: Failed to add peer");
            // return;
        }
    }
    
    esp_now_register_recv_cb(OnDataRecv);

//....................txt file read.................................//  
Serial.println("");
bool result = SPIFFS.begin();
Serial.println("SPIFFS OPEN: " + result);
File file = SPIFFS.open("/Image.txt","r");
if(!file){
Serial.println("Failed to open file for reading");
return;
} 
Serial.print("File size:");
Serial.println(file.size());
while(file.available()){
for (int i = 0; i < file.size(); i++){
buf[i] = file.read();
}
}
}


void loop() { //1  
    cycle_count = 5500;

    while(cycle_count--) { //*
        
        if (Serial.available() > 0) {
            inByte = Serial.read();
            Serial.print(">>");
            Serial.write(inByte);
            Serial.println("<<");
            inByte = 0;
        } //*
        
        if(esp_now_incoming_trigger) {   //2
            esp_now_incoming_trigger = 0;

           

            String crc_received_str = esp_now_incoming_string.substring(0, 5);
            uint16_t crc_received = crc_received_str.toInt();

while(esp_now_receiving) {  //3

     if(!esp_now_incoming_string.endsWith("1")) {  //**
                    esp_now_receiving = 0;
                } //**
                
//...................found the missing no...........................//
                deserializeJson(doc2, esp_now_incoming_string);
                JsonObject json_obj3 = doc2.as<JsonObject>();
                String read_str4 = json_obj3["c"];
                Serial.print("c: ");
                 Serial.print(read_str4);
                 Serial.print("\t");
                String read_str3 = json_obj3["MP"];
                Serial.print("ms:");
                String read_str5 = json_obj3["NP"];
                countM = read_str5;
                // Serial.print("\t");
                Serial.println(read_str3);
                delay(20);
                esp_now_incoming_string1 += read_str3; // adding missing no packet 
                }  
           // Serial.println(countM);         
           // Serial.println(esp_now_incoming_string1); //show missing no packet

            //....................string data cnvert to int array...........//
     int missingarray[countM.toInt()-1];
    int r=0,t=0;
 
for(int i=0;i<esp_now_incoming_string1.length();i++)
{ //*1
    if(esp_now_incoming_string1.charAt(i) == ',')
    { //*2
    String tex = esp_now_incoming_string1.substring(r,i);
      missingarray[t] = tex.toInt();
        r = (i+1);
      t++;
    } //*2
  } //*1

//.......................slave id no found..............// 
            JsonObject json_obj = doc.as<JsonObject>();
            esp_now_incoming_string.remove(0, 5); //remove first 5 carecter
            uint16_t crc_read = get_crc16(esp_now_incoming_string);
            uint8_t esp_now_incoming_integer;
            uint8_t received_slaveAddress[6];
 
//..........................8 slave send message find this part...............// 
            if(crc_read == crc_received) {  //4
                deserializeJson(doc, esp_now_incoming_string);
                String read_str = json_obj["slave_id"];
                 
                esp_now_incoming_integer = read_str.toInt();
                
                for(uint8_t i = 0; i < 6; i++) {  //***
                    received_slaveAddress[i] = slaveAddress[esp_now_incoming_integer - 1][i];
                }  //***
                
                String message_to_send = "";
                message_to_send.reserve(250);
  
                
                String sqlite3_return = String(buf);  //read file buffer convert to string
                Serial.println(sqlite3_return.length()); //file size

                //.................find the packet numbers........................//
                numberOfPacket =((sqlite3_return.length() % 150)==0)? 0 : 1; 
                numberOfPacket += sqlite3_return.length() /150;
                Serial.println(numberOfPacket);
        
                String sqlite3_return_string = sqlite3_return;
                int count= 1;
                //for(int i = 001; i<numberOfPacket; i++){
                  //  count = i;
              //  }


                //Serial.println( numberOfPacket);
                Serial.println(">>>>>>>>>>>>>>>>>>>>");

                uint16_t send_timer = millis();
                uint16_t message_count = 0; 
 
                while(1) { //5
    //......................file data into json message format............//
                    doc1["P"] = sqlite3_return_string.substring(0, 150);
                    sqlite3_return_string.remove(0, 150);
                    doc1["AP"] = String(numberOfPacket);
                    doc1["PN"] = String(count++);
                    doc1["F"] = String(sqlite3_return.length());
                    String json_message = "";
                    serializeJson(doc1, json_message);
                    message_to_send =json_message;
                    doc1.clear();
                    Serial.println(message_to_send);                  
                    
                    if(sqlite3_return_string.length()) {  //****
                       message_to_send += 1;
                    } else {  //*****
                        message_to_send += 0;
                    }  //*****

                    message_count++;
                   
                    ack_send_status = esp_now_send_message(received_slaveAddress, message_to_send);
                    message_to_send = "";
                    if(!ack_send_status) { //*1
                        break;
                    } //*1
                    if(!sqlite3_return_string.length()) {  //*2
                        break;
                    } //*2
                } //5

                Serial.println(">>>>>>>>>>>>>>>>>>>>" + String(message_count));

                send_timer = millis() - send_timer;
                Serial.print("send_timer ");
                Serial.println(send_timer);

                ack_status[esp_now_incoming_integer] = ack_send_status;
                // Serial.println("CRC > Y");
            //4
            } else {  //*3
                ack_send_status = esp_now_send_message(received_slaveAddress, ">NO 0");
                ack_status[esp_now_incoming_integer] = ack_send_status;
                Serial.println("CRC > N");
            } //*3
           // ack_send_status1 = esp_now_send_message1(received_slaveAddress, numberOfPacket);
        } //3
        delay(2);
        
    }  //2


    for(uint8_t i = 0; i < number_of_slaves; i++) { //*4
        Serial.print(i);
        Serial.print("\t|\t");
    } //*4
    Serial.print("\n");

    for(uint8_t i = 0; i < number_of_slaves; i++) { //*5
        Serial.print(ack_status[i]);
        Serial.print("\t|\t");
        ack_status[i] = 0;
    }  //*5
    Serial.print("\n");
    Serial.print("\n\n\n");
} //1