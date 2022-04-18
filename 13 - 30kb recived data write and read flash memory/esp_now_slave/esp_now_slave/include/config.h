#define UART2_WRITE_WAIT_CYCLES 400

int wdtTimeout  = 30000;  //time in ms to trigger the watchdog
int workTimeout = 20000;  //time in ms to trigger the watchdog

#define FORMAT_SPIFFS_IF_FAILED true

const uint16_t MAX_FILE_SIZE = 30000;       // File size => Bytes or Number of characters

const bool DEBUG_ERROR_EN   = 1;
const bool DEBUG_MSG_EN     = 1;
const bool DEBUG_WIFI_EN    = 1;
const bool DEBUG_MEM_EN     = 1;
const bool DEBUG_SQLITE_EN  = 0;
const bool DEBUG_ESPNOW_EN  = 1;


const int VBATT_MIN = 3400;                 // Voltage in miliVolts
