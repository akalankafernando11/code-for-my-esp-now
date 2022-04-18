
#ifndef UTILS_H
#define UTILS_H

unsigned long string_to_long(String rx_line)
{
    uint64_t tmp = 0;
    uint64_t return_val = 0;

    tmp = rx_line.substring(16, 12).toInt();
    return_val += tmp;

    tmp = rx_line.substring(12, 8).toInt();
    return_val += tmp * 100;

    tmp = rx_line.substring(8, 4).toInt();
    return_val += tmp * 10000;

    tmp = rx_line.substring(4, 0).toInt();
    return_val += tmp * 1000000;

    return return_val;
}

void general_gpio_Init ()
{
    U0TXD;
    U0RXD;
    
    ON_BTN_SIG_PIN;

    PWT_EN;
    PWR_EN_PIN;

    BATT_CHARGE_PIN;

    EPD_VCC_DIS;
    EPD_VCC_EN_PIN;

    USB_DETECT_SIG_PIN;

    PWR_LED_PIN;
    STAT_LED_PIN;
}

// Range : low/medium/high
// void cpu_freq_set(String range) 
// {
//     delay(100);
//     if(range == "low") {
//         rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
//     } else if (range == "medium") {
//         rtc_clk_cpu_freq_set(RTC_CPU_FREQ_160M);
//     } else {
//         rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
//     }
//     delay(100);
// }

// void update_status_code (uint16_t status_code_val) 
// {
//   if ( status_code < status_code_val) {
//     status_code = status_code_val;
//     log_message(DEBUG_MSG, "status_code changed to: " + (String)status_code);
//   }
// }

void pwr_led(bool status) {
    if (status) {
        PWR_LED_ON;
    } else {
        PWR_LED_OFF;
    }
}

void stat_led(bool status) {
    if (status) {
        STAT_LED_ON;
    } else {
        STAT_LED_OFF;
    }
}

void IRAM_ATTR power_led_blink (int count) {
  uint8_t clock_speed = rtc_clk_cpu_freq_get();
  for (int i = 1; i <= count; i++) {
    pwr_led(0);
    delay(100*clock_speed);
    pwr_led(1);
    delay(100*clock_speed);
  }
}

void IRAM_ATTR stat_led_blink (int count) {
  uint8_t clock_speed = rtc_clk_cpu_freq_get();
  for (int i = 1; i <= count; i++) {
    stat_led(0);
    delay(100*clock_speed);
    stat_led(1);
    delay(100*clock_speed);
  }
}

void power_off() {
  power_led_blink(2);
  delay(100);
  PWR_EN_PIN;
  PWR_DIS;
  delay(100);
}

void pmic_gpio_init()
{
    PMIC_EN_L;
    HVSW_CTRL_L;

    PMIC_EN_PIN;
    HVSW_CTRL_PIN;
    PMIC_POK_PIN;
}

void epson_gpio_init()
{
//     log_message(DEBUG_EPD,  "Now: EPSON_GPIO_init");

    EPSON_BB_SLEEP_H;
    EPSON_HDC_H;
    EPSON_RESET_H;
    EPSON_CS_H;

    EPSON_BB_SLEEP_PIN;
    EPSON_CS_PIN;
    EPSON_HDC_PIN;
    EPSON_RESET_PIN;
}



#endif /* UTILS_H */
