
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  2        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;


void print_wakeup_reason(){
    
    log_message(DEBUG_MSG, "going to deep sleep for " + String(TIME_TO_SLEEP) + " seconds ");

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_34,1);

    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    //esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    esp_deep_sleep_start();
}

void deep_sleep_for_seconds(uint8_t sleep_time) {
    Serial.println("Sleep for " + String(sleep_time) + "seconds");
    esp_sleep_enable_timer_wakeup(sleep_time * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}
 
