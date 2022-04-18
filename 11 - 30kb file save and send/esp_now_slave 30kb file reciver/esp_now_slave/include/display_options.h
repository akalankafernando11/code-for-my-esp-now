#ifndef DISPLAY_OPTIONS_H
#define DISPLAY_OPTIONS_H

String completed = "=========== COMPLETED ================================";
String line = "===========================================";

bool optFlash = false;
bool optDisplay = false;
bool optMisc = false;

char serial_read_char(){
    char read_char = '~', tmp;
    while(Serial.available()) {
        timerWrite(timer_w, 0);
        tmp = Serial.read();
        if(tmp != '\n') {
            Serial.println("You pressed: " + (String)tmp);
            read_char = tmp;
        }
    }
    if(read_char != '~'){
    }
    return read_char;
}

bool serial_read_check(char inchar1, char inchar2){
    while(1){
        char read_char = serial_read_char();
        if(read_char == '~'){
            // ignore
        } else if((read_char == inchar1) || (read_char == inchar2)) {
            return 1;
        } else {
            return 0;
        }
    }
}

void options_display() {
    Serial.println(line);   // line = "===========================================";
    Serial.println(F("Flash file options                               < F >"));
    Serial.println(F("Display options                                  < D >"));
    Serial.println(F("Misc Settings                                    < M >"));
    Serial.println(F("Turn off device                                  < X >"));
    Serial.println(F("EXIT                                             < 0 >"));
    Serial.println(line);
}

void options_display_EPD() {
    Serial.println(line);   // line = "===========================================";
    Serial.println(F("Pattern Check                                    < P >"));
    Serial.println(F("Fill White                                       < W >"));
    Serial.println(F("Fill Black                                       < B >"));
    Serial.println(F("EPD options Menu                                 < M >"));
    Serial.println(F("Back to Main Menu                                < - >"));
    Serial.println(F("EXIT                                             < 0 >"));
    Serial.println(line);
}

void options_display_memory() {
    Serial.println(line);       // line = "===========================================";
    Serial.println(F("Show all file list                               < 1 >"));
    Serial.println(F("Read log files                                   < 2 >"));
    Serial.println(F("Read all log files at once                       < 3 >"));
    Serial.println(F("Read log_batt files                              < 4 >"));
    Serial.println(F("Read all log_batt files at once                  < 5 >"));
    Serial.println(F("Delete log files                                 < 6 >"));
    Serial.println(F("Delete image files                               < 7 >"));
    Serial.println(F("Format SPIFFS                                    < 8 >"));
    Serial.println(F("Format EEPROM                                    < 9 >"));
    Serial.println(F("Flash file options Menu                          < M >"));
    Serial.println(F("Back to Main Menu                                < - >"));
    Serial.println(F("EXIT                                             < 0 >"));
    Serial.println(line);       // line = "===========================================";
}

void options_display_misc() {
    Serial.println(line);       // line = "===========================================";
    Serial.println(F("Device log all                                  < 1 >"));
    Serial.println(F("Device log memory                               < 2 >"));
    Serial.println(F("Device log EPD                                  < 3 >"));
    Serial.println(F("Device Summary                                  < 4 >"));
    Serial.println(F("Back to Main Menu                               < - >"));
    Serial.println(F("EXIT                                            < 0 >"));
    Serial.println(line);       // line = "===========================================";
}

void display_options_memory(){
    options_display_memory();
    Serial.flush();
    while (optFlash) {
        //timerWrite(timer_w, 0);
        debug_mem_en = 1;
        char read_char = serial_read_char();
        switch (read_char) {
            case '~':
                break;
            case '1':
                Serial.println(F("========= Files list =========="));

                list_dir(SPIFFS, "/", 0);

                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '2':
                Serial.println(F("========= Read individual log files ... =========="));
                print_dir_individual_files_content(SPIFFS, LOG_PATH);
                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '3':
                Serial.println(F("========= Read all log files at once ... =========="));
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, LOG_PATH);
                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '4':
                Serial.println(F("========= Read individual log_batt files ... =========="));
                print_dir_individual_files_content(SPIFFS, BATT_LOG);
                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '5':
                Serial.println(F("========= Read all log_batt files at once ... =========="));
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, BATT_LOG);
                Serial.println(F("completed"));
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '6': 
                Serial.println(F("========= Are you sure you want to DELETE log files? (Y/N) =========="));

                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, String(LOG_PATH));
                    delete_dir(SPIFFS, String(BATT_LOG));
                    Serial.println(F("completed"));
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '7':
                Serial.println(F("========= Are you sure you want to DELETE image files? (Y/N) =========="));
                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, IMG_PATH);
                    Serial.println(F("completed"));
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }

                Serial.println(F("Please enter number again to continue ..\n"));

                break;

            case '8':
                Serial.println(F("========= *** Are you sure you want to FORMAT device? This will remove all logs, images .etc (Y/N) =========="));
                if(serial_read_check('y', 'Y')) {
                    Serial.println(F("SPIFFS is formatting. Please wait..\n"));
                    format_spiffs();

                    timerWrite(timer_w, 0); //reset timer (feed watchdog)
                    
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                
                Serial.println(F("Please enter number again to continue ..\n"));
            case '9':
                Serial.println(F("========= *** Are you sure you want to Format EEPROM? (Y/N) =========="));
                if(serial_read_check('y', 'Y')) {
                    Serial.println(F("EEPROM Formatting. Please wait..\n"));
                    bool erase_status = false;
                    
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, "none");
                //     erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, "none");

                    if ( erase_status ) {
                        Serial.println(F("EEPROM Formatting Success..\n"));
                    } else {

                    }

                    timerWrite(timer_w, 0); //reset timer (feed watchdog)
                    
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                
                Serial.println(F("Please enter number again to continue ..\n"));



                break;

            case 'm':
            case 'M':
                options_display_memory();

                break;

            case '0':
                optFlash = 0;
                DISPLAY_OPTIONS = false;
                // DEBUG_MEM_EN = 0;
                Serial.println(F("============== EXIT =============================\n"));

                break;

            case '-':
                optFlash = 0;
                break;

            default:
                Serial.println(F("You are wrong..!\n"));
                Serial.println(F("Please enter number again to continue ..\n"));
                options_display_memory();
        }
        
    }   
}

void display_options_EPD() {
    options_display_EPD();
    debug_epd_en = 1;
    while (optDisplay) {
        char read_string = serial_read_char();
        switch (read_string) {
            case 'P':
            case 'p':
                Serial.println(F("Pattern check"));
                display_image("pattern");
                Serial.println(F("Pattern check complete"));
                break;
            case 'W':
            case 'w':
                Serial.println(F("Pattern check"));
                display_image("white");
                Serial.println(F("Pattern check complete"));
                break;
            case 'B':
            case 'b':
                Serial.println(F("Pattern check"));
                display_image("black");
                Serial.println(F("Pattern check complete"));
                break;
            case 'M':
            case 'm':
                options_display_EPD();
                break;    
            case '-':
                optDisplay = 0;
                break;
            case '0':
                optDisplay = 0;
                DISPLAY_OPTIONS = 0;
                break;
            case '~':
                break;
            default:
                Serial.println(F("You were wrong..!>>\n"));
                Serial.println(F("Please enter number again to continue ..\n"));
                options_display_EPD();
        }
    }
}

void display_options_misc() {
    options_display_misc();
    while(optMisc) {
        char read_string = serial_read_char();
        bool tmp = true;
        switch (read_string) {
            case '1':
                Serial.println(F("Show Serial log                                 < Y >"));
                Serial.println(F("Don't show Serial log                           < N >"));
                Serial.println(F("Back to Previous Menu                           < - >"));

                while(tmp){
                    read_string = serial_read_char();
                    switch (read_string) {
                        case 'Y':
                        case 'y':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_SERIAL_DEBUG_EN, "1")) {
                                Serial.println(F("Fail to save the changes"));
                            } 
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case 'N':
                        case 'n':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_SERIAL_DEBUG_EN, "0")) {
                                Serial.println(F("Fail to save the changes"));
                            }
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case '-':
                            tmp = 0;
                            break;
                        case '0':
                            optMisc = 0;
                            DISPLAY_OPTIONS = 0;
                            break;
                    }
                }
                break;
            case '2':
                Serial.println(F("Show Memory Serial log                          < Y >"));
                Serial.println(F("Don't show Memory Serial log                    < N >"));
                Serial.println(F("Back to Previous Menu                           < - >"));

                while(tmp){
                    read_string = serial_read_char();
                    switch (read_string) {
                        case 'Y':
                        case 'y':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_MEM_EN, "1")) {
                                Serial.println(F("Fail to save the changes"));
                            } 
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case 'N':
                        case 'n':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_MEM_EN, "0")) {
                                Serial.println(F("Fail to save the changes"));
                            }
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case '-':
                            tmp = 0;
                            break;
                        case '0':
                            optMisc = 0;
                            DISPLAY_OPTIONS = 0;
                            break;
                    }
                }
                break;
            case '3':
                Serial.println(F("Show EPD Serial log                                 < Y >"));
                Serial.println(F("Don't show EPD Serial log                           < N >"));
                Serial.println(F("Back to Previous Menu                               < - >"));

                while(tmp){
                    read_string = serial_read_char();
                    switch (read_string) {
                        case 'Y':
                        case 'y':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_EPD_EN, "1")) {
                                Serial.println(F("Fail to save the changes"));
                            } 
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case 'N':
                        case 'n':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_EPD_EN, "0")) {
                                Serial.println(F("Fail to save the changes"));
                            }
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case '-':
                            tmp = 0;
                            break;
                        case '0':
                            optMisc = 0;
                            DISPLAY_OPTIONS = 0;
                            break;
                    }
                }
                break;
            case '4':
                Serial.println(F("Show device Summary                             < Y >"));
                Serial.println(F("Don't show device Sum                           < N >"));
                Serial.println(F("Back to Previous Menu                           < - >"));

                while(tmp){
                    read_string = serial_read_char();
                    switch (read_string) {
                        case 'Y':
                        case 'y':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_SUM_EN, "1")) {
                                Serial.println(F("Fail to save the changes"));
                            } 
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case 'N':
                        case 'n':
                            if (!write_value_to_eeprom(EEPROM_MEM_ADDR_DEBUG_SUM_EN, "0")) {     //  
                                Serial.println(F("Fail to save the changes"));
                            }
                            Serial.println(F("Your change successfully saved"));
                            tmp = 0;
                            break;
                        case '-':
                            tmp = 0;
                            break;
                        case '0':
                            optMisc = 0;
                            DISPLAY_OPTIONS = 0;
                            break;
                    }
                }
                break;

            case 'M':
            case 'm':
                options_display_misc();
                break;    
            case '-':
                optMisc = 0;
                break;
            case '0':
                optMisc = 0;
                DISPLAY_OPTIONS = 0;
                break;
            case '~':
                break;
            default:
                Serial.println(F("You were wrong..!>>\n"));
                Serial.println(F("Please enter number again to continue ..\n"));
                options_display_misc();
        }
    }
}

void display_options() {
    
    DISPLAY_OPTIONS = false;
    char read_char = serial_read_char();

    if(read_char == 'D' | read_char == 'd') {
        DISPLAY_OPTIONS = true;
    }

    while(DISPLAY_OPTIONS) {
        serial_debug_en = 1;
        options_display();
        bool tmp = 1;
        while (tmp) {
            //Serial.println(F("B");
            //timerWrite(timer_w, 0);

            char read_string = serial_read_char();
        
            switch (read_string) {
                case 'F':
                case 'f':
                    optFlash = 1;
                    display_options_memory();
                    tmp = 0;
                    break;
                case 'D':
                case 'd':
                    optDisplay = 1;
                    display_options_EPD();
                    tmp = 0;
                    break;
                case 'M':
                case 'm':
                    optMisc = 1;
                    display_options_misc();
                    tmp = 0;
                    break;
                case 'X':
                case 'x':
                    Serial.println(F("========= *** Are you sure you want to Turn off device?  (Y/N) =========="));
                    if(serial_read_check('y', 'Y')) {
                        Serial.println(F("Device will Turn off withing 5 seconds. Please unplug the USB"));
                        //delay(200);
                        power_led_blink(10, 100);
                        power_off();
                    } else if(serial_read_check('n', 'N')) {
                        break;
                    }
                    break;
                case '0':
                    DISPLAY_OPTIONS = 0;
                    tmp = 0;
                    break;
                case '~':
                    break;
                default:
                    Serial.println(F("You were wrong..!\n"));
                    Serial.println(F("Please enter number again to continue ..\n"));
                    options_display();
                
            }
        }
    }
}


#endif /* DISPLAY_OPTIONS_H  */
