
#define EPSON_BB_SLEEP_PIN      	pinMode(25, OUTPUT)
#define EPSON_BB_SLEEP_PIN_input   	pinMode(25, INPUT)
#define EPSON_BB_SLEEP_H        	digitalWrite(25, HIGH)
#define EPSON_BB_SLEEP_L        	digitalWrite(25, LOW)

#define EPSON_RESET_PIN         	pinMode(13, OUTPUT)
#define EPSON_RESET_PIN_input       pinMode(13, INPUT)
#define EPSON_RESET_H           	digitalWrite(13, HIGH)
#define EPSON_RESET_L           	digitalWrite(13, LOW)

#define EPSON_CS_PIN            	pinMode(21, OUTPUT)
#define EPSON_CS_PIN_input      	pinMode(21, INPUT)
#define EPSON_CS_H              	digitalWrite(21, HIGH)
#define EPSON_CS_L              	digitalWrite(21, LOW)

#define EPSON_HDC_PIN           	pinMode(22, OUTPUT)
#define EPSON_HDC_PIN_input         pinMode(22, INPUT)
#define EPSON_HDC_H             	digitalWrite(22, HIGH)
#define EPSON_HDC_L             	digitalWrite(22, LOW)

// Power management IC Pins
#define PMIC_EN_PIN             	pinMode(12, OUTPUT)
#define PMIC_EN_PIN_input           pinMode(12, INPUT)
#define PMIC_EN_H               	digitalWrite(12, HIGH)
#define PMIC_EN_L               	digitalWrite(12, LOW)

#define HVSW_CTRL_PIN           	pinMode(2, OUTPUT)
#define HVSW_CTRL_PIN_input         pinMode(2, INPUT)
#define HVSW_CTRL_H             	digitalWrite(2, HIGH)
#define HVSW_CTRL_L             	digitalWrite(2, LOW)

#define PMIC_POK_PIN            	pinMode(14, INPUT_PULLUP)
#define PMIC_POK                	digitalRead(14)

#define I2C_SCL_PIN             	5
#define I2C_SDA_PIN             	4

// General GPOIs

#define SPI_CLK                 	18
#define SPI_MOSI                	23
#define SPI_CS                  	21
#define SPI_MISO                	19

#define PWR_LED_PIN                 pinMode(33, OUTPUT)
#define STAT_LED_PIN                pinMode(32, OUTPUT)
#define PWR_LED_ON                  digitalWrite(33, HIGH)
#define PWR_LED_OFF                 digitalWrite(33, LOW)
#define STAT_LED_ON                 digitalWrite(32, LOW)
#define STAT_LED_OFF                digitalWrite(32, HIGH)

#define BATT_CHARGE_PIN				pinMode(35, INPUT)
#define BATT_CHARGE 				digitalRead(35)

#define ON_BTN_SIG_PIN				pinMode(34, INPUT_PULLDOWN)
#define ON_BTN_SIG	 				digitalRead(34)
//#define PWR_SW_PIN_no 				34


//PCB V5.0
#define USB_DETECT_SIG_PIN 			pinMode(15, INPUT_PULLDOWN)
#define USB_DETECT_SIG 				digitalRead(15)

#define U0TXD	                    pinMode(1, INPUT_PULLUP)
#define U0RXD	                    pinMode(3, INPUT_PULLUP)

#define PWR_EN_PIN					pinMode(27, OUTPUT)
#define PWR_DIS 					digitalWrite(27, LOW)
#define PWT_EN						digitalWrite(27, HIGH)

#define EPD_VCC_EN_PIN				pinMode(26, OUTPUT)
#define EPD_VCC_EN					digitalWrite(26, HIGH)
#define EPD_VCC_DIS					digitalWrite(26, LOW)

