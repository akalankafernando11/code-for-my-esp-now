#ifndef LC709203F_H
#define LC709203F_H

#define BATTERY_MONITOR_ADDRESS   0x0B

uint16_t getVoltage(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x09);
}

uint16_t getCapacity(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0F);
}

uint16_t getBConst(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x06);
}

uint16_t getCellTemperature(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x08);
}

uint16_t getDirection(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0A);
}

uint16_t getAPT(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0C);
}

uint16_t getRSOC(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0D);
}

uint16_t getICVersion(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x11);
}

uint16_t getProfCode(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x1A);
}

void setAlarm(uint8_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x13, value);
}

void setVoltageAlarm(uint16_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x14, value);
}

void adjust(uint16_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0B, value);
}

void startScan() {

    byte error, address;
    int nDevices;

    nDevices = 0;
    for (address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission(true);

        if (error == 0) {
            Serial.print(F("I2C device found at address 0x"));
            if (address < 16)
                Serial.print(F("0"));
                Serial.print(address, HEX);
                Serial.println(F("  !"));

            nDevices++;
        }
        else if (error == 4) {
            Serial.print(F("Unknown error at address 0x"));
            if (address < 16)
                Serial.print(F("0"));
                Serial.println(address, HEX);
        }
    }
    if (nDevices == 0) {
        Serial.println(F("No I2C devices found\n"));
    } else {
        Serial.println(F("done\n"));
    }
}

void battery_monitor_pre_init() {
    //Vbatt2_ON;
    epson_gpio_init();
    pmic_gpio_init();

    EPD_VCC_EN;
    delay(50);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 100000);
}

void battery_monitor_init_release() {
    EPD_VCC_DIS;
    EPSON_BB_SLEEP_PIN_input;
    EPSON_CS_PIN_input;
    EPSON_HDC_PIN_input;
    EPSON_RESET_PIN_input;
    PMIC_EN_PIN_input;
    HVSW_CTRL_PIN_input;
}

bool initBattMon() {
    if (read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x11) == 0x00)
        return false;
    delay(100);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x04, 0xAA55);        //set initial RSOC (necessary?)
    delay(100);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x07, 0xAA55);        //set initial RSOC (necessary?)
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x15, 0x0001);        //set power mode : Operating
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0B, 0x0014);        //set Adjustment Pack Application Value
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x12, 0x0000);        //choose default battery profile (see datasheet)
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x16, 0x0001);        //Enable thermistor
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x06, 0x0D49);        //Sets B−constant of the thermistor to be measured (0x0D49 = 3401K)
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0A, 0x0000);        //set auto current detection mode
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x13, 0x0005);        //activate alarm if RSOC lower than 5%
    delay(10);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x14, 0x3500);        //activate alarm if voltage under 2,8volts
    delay(10);
    return true;
}



#endif /* LC709203F_H */
