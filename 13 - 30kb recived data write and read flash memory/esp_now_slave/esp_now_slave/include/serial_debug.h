#define DEBUG_ERROR     1
#define DEBUG_MSG       2
#define DEBUG_WIFI      3
#define DEBUG_MEM	4
#define DEBUG_SQLITE	5
#define DEBUG_ESPNOW	6


void log_message(uint8_t debug_type, String str="", bool no_prefix=false)
{
    String prefix = "";
	
    bool log_en = false;

    switch(debug_type){
    	case DEBUG_ERROR:
			log_en = DEBUG_ERROR_EN;
    		prefix += "DEBUG_ERROR";
    		break;
    	case DEBUG_MSG:
			log_en = DEBUG_MSG_EN;
    		prefix += "DEBUG_MSG";
    		break;
    	case DEBUG_WIFI:
			log_en = DEBUG_WIFI_EN;
 			prefix += "DEBUG_WIFI";
 			break;
		case DEBUG_MEM:
			log_en = DEBUG_MEM_EN;
 			prefix += "DEBUG_MEM";
 			break;
		case DEBUG_SQLITE:
			log_en = DEBUG_SQLITE_EN;
 			prefix += "DEBUG_SQLITE";
 			break;
		case DEBUG_ESPNOW:
			log_en = DEBUG_ESPNOW_EN;
 			prefix += "DEBUG_ESPNOW";
 			break;
    }

	if(log_en) {
		if(no_prefix == 0) {
			String logMsg = prefix + "\t:\t" + str;
    		Serial.println(logMsg);
		} else {
			String logMsg = str;
    		Serial.println(logMsg);
		}
	}
    

}