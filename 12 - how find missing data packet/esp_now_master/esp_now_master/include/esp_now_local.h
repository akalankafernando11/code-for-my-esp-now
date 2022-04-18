const uint8_t ESPNOW_SEND_TRY_COUNT = 20;

// Variable to store if sending data was successful
int8_t delivery_report = 0;
/// Create a struct_message to hold incoming sensor readings
char incomingReadings[250];

// Must match the receiver structure
/*typedef struct struct_message {
int c;
} struct_message;

// Create a struct_message called myData
struct_message myData;*/

//void OnDataSent1(const uint8_t *mac_addr, esp_now_send_status_t status) {
//  Serial.print("\r\nLast Packet Send Status:\t");
//  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
//}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status ==0){
        delivery_report = 1;
    }
    else{
        delivery_report = 0;
    }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
    
    esp_now_receive_address[1] = *mac;

    // Serial.println(abc[2]);

    esp_now_incoming_string = String(incomingReadings);
    esp_now_incoming_string.remove(len);

    esp_now_incoming_trigger = 1;

}
/*bool esp_now_send_message1(uint8_t broadcastAddress[], int message1) {

   // timerWrite(timer_w, 0); //reset timer (feed watchdog)
    int c;
    c = message1;
    //char send_message[message_string.length()];
    //message_string.toCharArray(send_message, message_string.length() + 1);
 while(delivery_report == 0) {
esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &c, sizeof(c));
  
 if (result == ESP_OK) {
           
            // Serial.print('.');
            if (delivery_report) {
                // Serial.println("Delivery Sucess");
                delivery_report = 0;
                return true;
            }
        }
        else {
            Serial.print('-');
        }
}}*/
//bool esp_now_send_message1(uint8_t broadcastAddress[], int message1) {

bool esp_now_send_message(uint8_t broadcastAddress[], String message_string) {

 //++   timerWrite(timer_w, 0); //reset timer (feed watchdog)
    char send_message[message_string.length()];
    //send_message = (char)message1;
    message_string.toCharArray(send_message, message_string.length() + 1);
    
    //char number[numberOfPacket.length()];
    //numberOfPacket.toCharArray(send_message, message_string.length() + 1);
    //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &c, sizeof(c));
    uint8_t retry_count = 0;
    // for(int8_t retry_count = 0; retry_count < ESPNOW_SEND_TRY_COUNT; retry_count++) {
    // uint64_t message_send_timeout = millis();
    while(delivery_report == 0) {
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_message, sizeof(send_message));
        delay(20);
        if (result == ESP_OK) {
           
             Serial.print('.');
            if (delivery_report) {
                 Serial.println("Delivery Sucess");
                delivery_report = 0;
                return true;
            }
        }
        else {
            Serial.print('-');
        }
 
        // message_send_timeout = millis() - message_send_timeout;

        if(retry_count >= ESPNOW_SEND_TRY_COUNT) {
            Serial.println("Delivery Fail");
            return false;
        }   
        retry_count++;
    }

}

