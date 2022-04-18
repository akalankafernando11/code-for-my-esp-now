byte read_i2c(uint8_t addr, uint8_t reg)
{
    byte i2c_read_byte;
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(addr, 1);
    
    if (Wire.available()<=1) {
        i2c_read_byte = Wire.read();
    }
    
    return i2c_read_byte;
}

bool write_i2c(uint8_t addr, uint8_t reg, uint8_t val)
{
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val);
    bool rtnVal = Wire.endTransmission();
    return rtnVal;
}

uint16_t read_i2c_16(uint8_t addr, uint8_t reg) {
  uint16_t result; // returned value
  uint16_t r;
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, 2); // now get the byte of data...
  result = Wire.read() | (Wire.read() << 8);  //LSB first

  return result; // and return it as a result of the function readData
}

void write_i2c_16(uint8_t addr, uint8_t reg, uint8_t val) {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val & 0xFF);       //LSB first
    Wire.write(val >> 8);
    Wire.endTransmission();
}
