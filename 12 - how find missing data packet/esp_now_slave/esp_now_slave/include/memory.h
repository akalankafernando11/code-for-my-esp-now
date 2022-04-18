#ifndef MEMORY_H
#define MEMORY_H

void update_status_code (uint16_t status_code_val) 
{
  if ( status_code < status_code_val) {
    status_code = status_code_val;
    log_message(DEBUG_MSG, "status_code changed to: " + (String)status_code);
  }
}

String number_to_string(uint8_t number)  {
    String string ((char *)0);
    string.concat("000");

    string.concat(number);

    string = string.substring(string.length()-3, string.length());

    return string;
}

bool initialize_spiffs()
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    log_message(DEBUG_MSG, "SPIFFS starting.. Please wait...");
    if (!SPIFFS.begin(true)) {
        //update_status_code(3300);
        log_message(DEBUG_ERROR,  "spiffs mount failed");
        return false;
    } 
    return true;
}

bool format_spiffs()
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if (!SPIFFS.format()) {
        //update_status_code(3305);
        log_message(DEBUG_ERROR, "Fail to Format SPIFFS");
        return false;
    }
    log_message(DEBUG_MEM,  "Format is Success");
    return true;
}

int count_files(fs::FS &fs, String dirname) 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    int file_count = 0;

    File root = fs.open(dirname);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return 0;
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return 0;
    }

    File file = root.openNextFile();

    while (file) {
        file_count++;
        file = root.openNextFile();
    }

    return file_count;
}

bool list_dir(fs::FS &fs, String dirname, uint8_t levels)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    log_message(DEBUG_MEM,  "SPIFFS:listing directory: " + (String)dirname);

    File root = fs.open(dirname);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return false;
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return false;
    }

    File file = root.openNextFile();

    while (file) {
        if (file.isDirectory()) {
            log_message(DEBUG_MEM,  "SPIFFS:dir : ");
            log_message(DEBUG_MEM,  "SPIFFS:" + (String)file.name() + ", ");

            if (levels) {
                list_dir(fs, file.name(), levels -1);
            }

        } else {
            log_message(DEBUG_MEM,  "SPIFFS:file :" + String(file.name()) + " > size: " + (String)file.size() +"");
        }

        file = root.openNextFile();
    }

    return true;
}

String find_file(fs::FS &fs, String dirname, String file_name_s, uint8_t levels)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    log_message(DEBUG_MEM,  "SPIFFS:listing directory: " + String(dirname));
    log_message(DEBUG_MEM,  "SPIFFS:looking for a file: " + file_name_s);
    String file_name = "";
    File root = fs.open(dirname);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return "0";
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return "0";
    }

    File file = root.openNextFile();

    while (file) {
        if (file.isDirectory()) {
            if (levels) {
                // list_dir(fs, file.name(), levels -1);
            }
        } else {
            file_name = (String)file.name();
            
            if (file_name.endsWith(file_name_s)){
                log_message(DEBUG_MEM,  "SPIFFS: File found = " + file_name + ", ");
                return file_name;
            }
        }
        file = root.openNextFile();
    }

    log_message(DEBUG_ERROR,  "File not found");
    return "0";
}

String find_file_by_number(fs::FS &fs, String dirname, uint8_t file_number)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    String file_name ((char *)0);
    String file_number_str ((char *)0);

    file_number_str += number_to_string(file_number);

    log_message(DEBUG_MEM,  "SPIFFS:looking for a file: " + String(file_number));

    File root = fs.open(dirname);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return "0";
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return "0";
    }

    File file = root.openNextFile();

    while (file) {
        file_name = (String)file.name();
        String file_name_tmp ((char *)0);
        file_name_tmp += file_name.substring(String(dirname).length()+1, file_name.length()); 

        if (file_name_tmp.startsWith(file_number_str)){
            log_message(DEBUG_MEM,  "SPIFFS: File found = " + file_name);
            return file_name;
        }
        file = root.openNextFile();
    }

    log_message(DEBUG_ERROR,  "File not found");
    return "0";
}

uint32_t read_file_size(fs::FS &fs, String directory, String filename)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    const char * path = filename.c_str();
    uint32_t file_size = 0;

    File file = fs.open(path);
    if (file && !file.isDirectory()) {
        file_size = file.size();
    } else {
        log_message(DEBUG_MEM, "File not found: " + directory + filename );
        return 0;
    }

    file.close();

    log_message(DEBUG_MEM, "File size of: " + filename + " : " + String(file_size));

    return file_size;
}

bool check_availability(fs::FS &fs, String directory, String filename)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    const char * path = filename.c_str();

    File file = fs.open(path);

    if (!file || file.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file");
        //update_status_code(3312);
        return false;
    }

    file.close();

    return true;
}

bool read_file(fs::FS &fs, String directory, String filename)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }
    
    const char * path = filename.c_str();

    log_message(DEBUG_MEM,  "SPIFFS:reading file: " + (String)path +"");

    File file = fs.open(path);

    if (!file || file.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for reading");
        //update_status_code(3313);
        return false;
    }

    log_message(DEBUG_MEM,  "SPIFFS:read from file: ");

    while (file.available()) {
        uint8_t read_byte = file.read();
        if ((read_byte >> 4) < 1) {
            //log_message(DEBUG_MEM,  "SPIFFS:0");
        }
            
        Serial.println(file.readString());
    
    }

    if (DEBUG_MEM) {
            Serial.println(F(""));
            log_message(DEBUG_MEM,  "SPIFFS:complete reading file: " + (String)path +"");
        }
    file.close();

    return true;
}

bool read_file_to_string(fs::FS &fs, String directory, String filename, uint32_t max_size)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    spiffs_read_string.remove(0);
    uint32_t read_size = 0;
    const char * path = filename.c_str();

    log_message(DEBUG_MEM,  "SPIFFS:reading file: " + (String)path +"");

    File file = fs.open(path);

    if (!file || file.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for reading");
        //update_status_code(3314);
        spiffs_read_string.remove(0);
        spiffs_read_string += "none";
    }

    while (file.available()) {
        spiffs_read_string += file.readString();
        read_size++;
        if(read_size > max_size) {
            return false;
        }
    }

    file.close();

    return true;
}

bool read_image_file_to_array(fs::FS &fs, String directory, String filename, uint16_t array_size)       // ype:bw|duration:0|blk:
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    int i = 0;
    const char * path = filename.c_str();

    File file = fs.open(path);

    if (!file || file.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for reading");
        //update_status_code(3315);
        return false;
    }

    file.read(spiffs_read_data, array_size);
    file.close();
    
    return true;
}

bool print_dir_individual_files_content(fs::FS &fs, const char * dirname)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    File root = fs.open(dirname);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return false;
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return false;
    }

    File file = root.openNextFile();

    Serial.println(F("==========================================="));
    Serial.println(F("1. Press 'N' to read next file"));
    Serial.println(F("2. Press 'E' to EXIT"));
    Serial.println(F("==========================================="));
    
    while(1) {

        if(Serial.available() > 0) {

            char inByte = Serial.read();
            
            if(inByte == 'n' || inByte == 'N') {

                if (file) {

                    log_message(DEBUG_MEM,  "SPIFFS:file :" + String(file.name()) + " > size: " + (String)file.size() +"");
                    read_file(fs, "/", file.name());
                    log_message(DEBUG_MEM,  " =========== " + String(file.name()) + " : COMPLETED =====================");
                    timerWrite(timer_w, 0);

                    file = root.openNextFile();

                    Serial.println(F(" \n\n\n\n==========================================="));
                    Serial.println(F("1. Press 'N' to read next file"));
                    Serial.println(F("2. Press 'E' to EXIT"));
                    Serial.println(F("==========================================="));
                }
            } else if(inByte == 'e' || inByte == 'E') { 
                return true;
            }
        }

        timerWrite(timer_w, 0);
    }

    return true;
}

/**
* Print content of the all files inside a directory
* This will be useful in logs reading
**/
bool print_dir_all_files_content(fs::FS &fs, const char * dirname)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)
    
    File root = fs.open(dirname);
    
    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3310);
        return false;
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3311);
        return false;
    }

    File file = root.openNextFile();
    
    while (file) {
        timerWrite(timer_w, 0);
        read_file(fs, "/", file.name());
        file = root.openNextFile();
                
        timerWrite(timer_w, 0);
    }

    return true;
}

bool write_file(fs::FS &fs, String directory, String filename, String temp_str2)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    const char * path = filename.c_str();
    const char * message = temp_str2.c_str();
    uint16_t file_size = read_file_size(SPIFFS, "/", filename);
    

    file_size += temp_str2.length();
    if (file_size > MAX_FILE_SIZE) {
        log_message(DEBUG_ERROR,  "SPIFFS:error: file too long");
        //update_status_code(3316);
        return false;
    }

    log_message(DEBUG_MEM,  "SPIFFS:writing file: " + (String)path +"");
    log_message(DEBUG_MEM,  "SPIFFS:writing data: " + temp_str2 +"");

    uint8_t tmp = 5;
    while(tmp) {
        timerWrite(timer_w, 0); //reset timer (feed watchdog)
        File file = fs.open(path, FILE_WRITE);

        if (!file) {
            log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for writing");
        } else {
            if (file.print(message)) {
                log_message(DEBUG_MEM,  "SPIFFS:file written");
                return true;
            } else {
                log_message(DEBUG_MEM,  "SPIFFS:fail to write file");
            }
        }
        file.close();
        tmp--;
        delay(100);
    }
    if (!tmp) {
        log_message(DEBUG_ERROR, "DEBUG_MEM", "SPIFFS:failed to open file for appending, Retrying..");
        //update_status_code(3318);
        return false;
    }
    return true;
}

bool append_file(fs::FS &fs, String directory, String filename, String temp_str2)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }

    const char * path = filename.c_str();
    const char * message = temp_str2.c_str();
    uint16_t file_size = read_file_size(SPIFFS, "/", filename);
    file_size += temp_str2.length();


    if (file_size > MAX_FILE_SIZE) {
        log_message(DEBUG_ERROR,  "SPIFFS:error: file too long > File size = " + String(file_size) + "Max file size = " + String(MAX_FILE_SIZE));
        //update_status_code(3319);
        return false;
    }

    log_message(DEBUG_MEM,  "SPIFFS:appending to file: " + (String)path);
    log_message(DEBUG_MEM,  "SPIFFS:writing data: ");
    delay(100);
    uint8_t tmp = 5;
    
    while (tmp) {
        File file = fs.open(path, FILE_APPEND);
        timerWrite(timer_w, 0); //reset timer (feed watchdog)

        if (!file) {
            log_message(DEBUG_ERROR, "DEBUG_MEM", "SPIFFS:failed to open file for appending");
            delay(100);
        } else {
            if (file.print(message)) {
                log_message(DEBUG_MEM,  "SPIFFS:message appended");
                delay(100);
                return true;
            } else {
                log_message(DEBUG_MEM,  "SPIFFS:fail to append file, Retrying..");
                delay(100);
            }
        }
        file.close();
        tmp--;
        delay(100);
    }
    if (!tmp) {
        //update_status_code(3320);
        return false;
    }
}

/*
bool rename_file(fs::FS &fs, const char * path1, const char * path2)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    log_message(DEBUG_MEM,  "SPIFFS:renaming file " + (String)path1 + " to " + (String)path2);

    if (fs.rename(path1, path2)) {
        log_message(DEBUG_MEM,  "SPIFFS:file renamed");
    } else {
        log_message(DEBUG_ERROR,  "SPIFFS:Fail to rename file");
        //update_status_code(3322);
        return false;
    }

    return true;
}
*/

bool delete_file(fs::FS &fs, String directory, String filename)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if(directory != "/") {
        filename = directory + "/" + filename;
    }
    const char * path = filename.c_str();
    uint8_t tmp = 5;

    log_message(DEBUG_MEM,  "SPIFFS : Deleting file : " + (String)path);

    if (fs.exists(path)){
        while (tmp) {
            if(fs.remove(path)) {
                log_message(DEBUG_MEM,  "SPIFFS:File deleted"); 
                return true;
            } else {
                log_message(DEBUG_ERROR,  "SPIFFS:fail to delete file");
            }
            delay(100);
            tmp--;
        }
    } else {
        log_message(DEBUG_ERROR,  "SPIFFS:File not exist");
        //update_status_code(3324);
        return false;
    }

    if(!tmp) {
        //update_status_code(3323);
        return false;
    }
}

bool delete_dir(fs::FS &fs, String directory)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    log_message(DEBUG_MEM,  "SPIFFS:deleting directory: " + (String)directory);

    const char * path = directory.c_str();

    File root = fs.open(path);

    if (!root) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open directory");
        //update_status_code(3325);
        return false;
    }

    if (!root.isDirectory()) {
        log_message(DEBUG_ERROR,  "SPIFFS:not a directory");
        //update_status_code(3326);
        return false;
    }

    File file = root.openNextFile();
    String filename ((char *)0);
    filename.concat(String(file.name()));
    
    while (file) {
        // file.close();
        // delay(100);
        if(SPIFFS.remove(filename)) {
            log_message(DEBUG_MEM,  "SPIFFS:File deleted: " + String(filename)); 
        } else {
            log_message(DEBUG_ERROR,  "SPIFFS:fail to delete file: " + String(filename));
            //update_status_code(3324);
        }
        file = root.openNextFile();
        filename.remove(0);
        filename.concat(String(file.name()));
        delay(50);
    }

    return true;
}

bool validate_file_size(fs::FS &fs, String directory, String filename, uint16_t file_size, bool delete_file = false)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)
   
    if(directory != "/") {
        filename = directory + "/" + filename;
    }
    const char * path = filename.c_str();
    File file = fs.open(path);

    if (file && !file.isDirectory()) {
        // log_message(DEBUG_MSG, ">>>>>>>>>>>>>>>>>>>>>>>>>> " + String(file_size) + ">>>>>>>>>>>>>>>>>" + String(file.size()));
        if (file_size == file.size()) {
            return true;
        }
    }
    log_message(DEBUG_MEM, "File size validation failed");
    
    if(delete_file) {
        if(fs.remove(path)) {
            log_message(DEBUG_MEM,  "SPIFFS:File deleted"); 
            return false;  
            // return true;
        } else {
            log_message(DEBUG_ERROR,  "SPIFFS:fail to delete file");
        }
    }
    return false;  
    // return true;
}

/*
bool test_file_io(fs::FS &fs, String temp_str)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    const char * path = temp_str.c_str();

    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;

    if (file && !file.isDirectory()) {

        len = file.size();
        size_t flen = len;
        start = millis();

        while (len) {
            size_t toRead = len;

            if (toRead > 512) {
                toRead = 512;
            }

            file.read(buf, toRead);
            len -= toRead;
        }

        end = millis() - start;
        log_message(DEBUG_MEM,  "SPIFFS:" + (String)flen + "bytes read for " + (String)end  + " ms");
        file.close();
    } else {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for reading");
        //update_status_code(3327);
        return false;
    }

    file = fs.open(path, FILE_WRITE);
    if (!file) {
        log_message(DEBUG_ERROR,  "SPIFFS:failed to open file for writing");
        //update_status_code(3328);
        return false;
    }

    size_t i;
    log_message(DEBUG_MEM,  "SPIFFS:" + (String)(2048 * 512) + "bytes read for " + (String)end  + " ms");
    file.close();
    return true;
}
*/

/*
void print_memory_usage() 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); 
    log_message(DEBUG_MEM,  "Free Stack : " + (String)uxHighWaterMark + " | Free Heap " + (String)esp_get_free_heap_size());
}
*/

/*
bool initialize_eeprom() 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    int count = 0;
    while(count < EEPROM_FAIL_COUNT) {
        
        if (!EEPROM.begin(EEPROM_SIZE)) {
            //count++;
            log_message(DEBUG_ERROR,  "Failed to initialise EEPROM. Reinitializing....");
            delay(1000);
        } else {
            return true;
        }
    }

    if (count >= EEPROM_FAIL_COUNT) {
        log_message(DEBUG_ERROR,  "Fail to initialise EEPROM");
        //update_status_code(3501);
        return false;
    }

    return true;
}

bool write_value_to_eeprom(uint16_t eeprom_addr, String content)
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    content.trim();
    int content_len = content.length() + 1;
    char content_arr[content_len];

    log_message(DEBUG_EEPROM,  "Data writing on eeprom: " + (String)eeprom_addr + ": "+ content);

    if (content_len > EEPROM_MAX_CHAR_LENGTH) {
 
        log_message(DEBUG_EEPROM,  "content is too long to write to eeprom: " + (String)eeprom_addr);

        //update_status_code(3502);

        return false;
    }

    log_message(DEBUG_EEPROM,  "Saving data to EEPROM Address:" + (String)eeprom_addr + ", Content:" + content);
    content.toCharArray(content_arr, content_len);

    int tmp = 5;

    while (tmp) {
        if(EEPROM.writeString(eeprom_addr, content_arr)) {
            EEPROM.commit();
            return true;
        }
        tmp--;
    }

    if ( tmp == 0){
        log_message(DEBUG_ERROR,  "Fail to write to eeprom: " + (String)eeprom_addr);
        //update_status_code(3502);
    }

    return false;
}

String read_value_from_eeprom(int eeprom_addr) 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    String ret_value  ((char *)0);
    if (eeprom_addr >= 0 && eeprom_addr < EEPROM_SIZE) {
        String value = EEPROM.readString(eeprom_addr); 
        delay(100);

        if(isgraph(value.charAt(0)) && isgraph(value.charAt(value.length()-1))) {
            ret_value = value;
        } else if(value == "0" || value == "" || value.endsWith("none")) {
            ret_value = "none";
        } else if(value.endsWith("change")){
            ret_value = "change";
        } else if(value.endsWith("download")){
            ret_value = "download";
        } else {
            write_value_to_eeprom(eeprom_addr, "value_reset");
            ret_value = "value_corrupted";
        }

        log_message(DEBUG_EEPROM,  "EEPROM_READ: " + (String)eeprom_addr +" | length:"+ (String)value.length());

        if (value.length() > 50) {
            write_value_to_eeprom(eeprom_addr, "value_reset");
            ret_value = "value_corrupted";
        }

        return ret_value;
    }

    log_message(DEBUG_ERROR,  "EEPROM Mem Address can't be NULL or higher than EEPROM size");
    //update_status_code(3503);
    return "no";
}

bool write_pre_timelines_eeprom(int timelines[]) 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    String data  ((char *)0);
    if( sizeof(timelines) > 0) {
        for (int i = 0; i < sizeof(timelines); ++i)
        {
            data += (String)timelines[i];
            data += (i < sizeof(timelines)-1) ? "-" : "";
        }
    } else {
        data = "";
    }

    return write_value_to_eeprom(EEPROM_PRE_TIMELINES, data);
}

String get_memory_free_stack() 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    return (String)uxHighWaterMark;
}

String get_memory_free_heap() 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
    return (String)esp_get_free_heap_size();
}

bool save_log_file(String path, String log_line) 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    String log_name ((char *)0);
    time_t log_name_var = current_time / 1000;
    if ((log_name_var % 10) >= 5) {
        log_name_var = log_name_var/10;
        log_name_var = log_name_var*10 + 5;
    } else {
        log_name_var = log_name_var/10;
        log_name_var = log_name_var*10;
    }
    log_name += String(log_name_var);

    uint8_t tmp = 0;

    while (1) { 
        if ((MAX_FILE_SIZE - 200) < read_file_size(SPIFFS, path, log_name)) {
            tmp++;
            log_name += "_";
            log_name += String(tmp);
            log_message(DEBUG_MSG, path + ": " + log_name );
        } else {
            break;
        }
    }

    log_message(DEBUG_MSG, path + ": " + log_name + ", log_line: " + log_line);
    
    if (!append_file(SPIFFS, path, log_name, log_line)) {
        log_message(DEBUG_ERROR,  "Log saving fail");
        return false;
    }
    return true;
}

void save_battery_log(int battery_volt, float battery_cap) 
{
    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    String log_line ((char *)0); 

    current_time = time_stamp + (timer_stop(millis_start)/1000);
    
    log_line += String(time_stamp); 
    log_line += ",";
    log_line += String(battery_volt); 
    log_line += ",";
    log_line += String(battery_cap); 
    log_line += ",";
    log_line += battery_status;
    log_line += "\n";

    save_log_file(BATT_LOG, log_line);
}
*/

#endif /* MEMORY_H */
