
#define K_ADDRESS 83
#define C_ADDRESS 82
#define M_ADDRESS 81
#define Y_ADDRESS 80
#define DATA_SIZE 128

#include <Wire.h>

// Custom data size should be 1 to 128 bytes.
const byte CUSTOM_DATA[] = {168,0,1,3,18,1,1,255,0,0,52,48,55,53,52,51,20,2,65,66,22,0,24,57,0,0,0,0,0,0,0,0,89,56,53,56,80,57,48,48,52,51,49,0,40,0,0,0,0,0,0,100,0,0,2,23,0,0,0,0,0,0,0,0,0,0,3,110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,218,0,37,12,211,0,24,41,66,50,0,26,1,21,79,154,17,14,36,100,150,7,7,7,7,0,24,41,66}; 

const byte CHIP_DATA_TEMPLATE[] = {168,0,1,3,18,1,1,255,100,0,52,48,55,53,52,50,20,9,65,66,22,0,7,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,100,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0};

void setup() {
    byte error=0, address=0, aDevices[128];
    uint8_t nDevices=0, op=0;
    char ch, target;
    char* message = new char[50];
    bool k=false, c=false, m=false, y=false;
    Wire.begin();
    Serial.begin(115200);
    while (!Serial);
    delay(500);
    Serial.println(F("\nScanning I2C devices"));
    for(address=1; address<127; address++) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print(F("I2C device found at address 0x"));
            if (address<16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println(F("  !"));
            aDevices[nDevices] = address;
            nDevices++;
        } else if (error==4) {
            Serial.print(F("Unknown error at address 0x"));
            if (address<16) Serial.print("0");
            Serial.println(address,HEX);
        }   
    }
    if (nDevices == 0) {
        Serial.println(F("No I2C devices found, try again by press reset button.\n"));
        return;
    } else {
        Serial.println(F("done\n"));
        for (uint8_t n=0; n<nDevices; n++) {
            if ((aDevices[n] == Y_ADDRESS) && headerCheck(Y_ADDRESS)) {
                y = true;
                Serial.println(F("Yellow(Y) toner cartridge found!"));
            } else if ((aDevices[n] == M_ADDRESS) &&  headerCheck(M_ADDRESS)) {
                m = true;
                Serial.println(F("Magenta(M) toner cartridge found!"));
            } else if ((aDevices[n] == C_ADDRESS) &&  headerCheck(C_ADDRESS)) {
                c = true;
                Serial.println(F("Cyan(C) toner cartridge found!"));
            } else if ((aDevices[n] == K_ADDRESS) &&  headerCheck(K_ADDRESS)) {
                k = true;
                Serial.println(F("Black(K) toner cartridge found!"));
            }
        }
        Serial.println("");
        if (y || m || c || k) {
            Serial.println(F("Reading current data from present toner cartridge chip. (You should backup these data incase write fail or invalid reset data.)"));
            if (c) {
                Serial.print(F("Cyan(C): {"));
                for(byte i=0;i<128;i++){
                    byte readVale = i2cRead(C_ADDRESS, i);
                    Serial.print(readVale);
                    if(i != 127) Serial.print(",");
                }
                Serial.println(F("}"));
            }
            if (m) {
                Serial.print(F("Magenta(M): {"));
                for(byte i=0;i<128;i++){
                    byte readVale = i2cRead(M_ADDRESS, i);
                    Serial.print(readVale);
                    if(i != 127) Serial.print(",");
                }
                Serial.println(F("}"));
            }
            if (y) {
                Serial.print(F("Yellow(Y): {"));
                for(byte i=0;i<128;i++){
                    byte readVale = i2cRead(Y_ADDRESS, i);
                    Serial.print(readVale);
                    if(i != 127) Serial.print(",");
                }
                Serial.println(F("}"));
            }
            if (k) {
                Serial.print(F("Black(K): {"));
                for(byte i=0;i<128;i++){
                    byte readVale = i2cRead(K_ADDRESS, i);
                    Serial.print(readVale);
                    if(i != 127) Serial.print(",");
                }
                Serial.println(F("}"));
            }
            Serial.println(F("\n***WARNING: After operation all data likes stats, serial number and etc will be erase!***"));
            Serial.println(F("Please select operation: f-set to full, e-set to empty, c-write custom data"));
            while(!Serial.available());
            while(Serial.available()) {
                ch = Serial.read();
            }
            switch (ch) {
                case 'f':
                    op = 0;
                    Serial.println(F("Selection: f-set to full"));
                    strcpy(message, "Set full to ");
                    break;
                case 'e':
                    op = 1;
                    Serial.println(F("Selection: e-set to empty"));
                    strcpy(message, "Set empty to ");
                    break;
                case 'c':
                    Serial.println(F("Selection: c-write custom data"));
                    if ((sizeof(CUSTOM_DATA) > 0) && (sizeof(CUSTOM_DATA) < 129)) {
                        op = 2;
                        strcpy(message, "Write custom data to ");
                    } else {
                        Serial.println(F("Error: Custom data size must be 1 to 128 bytes, aborted."));
                        return;
                    }
                    break;
                default:
                    Serial.print(F("Selection: "));
                    Serial.print(ch);
                    Serial.println(F("-unknown operation, aborted. Try again by press reset button."));
                    return;
            }
            Serial.print(F("\nPlease select target: "));
            if (c) Serial.print(F("c-cyan, "));
            if (m) Serial.print(F("m-magenta, "));
            if (y) Serial.print(F("y-yellow, "));
            if (k && op != 2) Serial.print(F("k-black, "));
            else if (k) Serial.println(F("k-black"));
            if (op != 2) Serial.println(F("a-all present toner cartridge"));
            while(!Serial.available());
            while(Serial.available()) {
                ch = Serial.read();
            }
            switch (ch) {
                case 'c':
                    if (c) {
                        target = ch;
                        address = C_ADDRESS;
                        Serial.println(F("Selection: c-cyan"));
                        strcat(message, "c-cyan");
                    } else {
                        Serial.println(F("Selection: c-cyan, cartridge not found, aborted. Try again by press reset button."));
                        return;
                    }
                    break;
                case 'm':
                    if (m) {
                        target = ch;
                        address = M_ADDRESS;
                        Serial.println(F("Selection: m-magenta"));
                        strcat(message, "m-magenta");
                    } else {
                        Serial.println(F("Selection: m-magenta, cartridge not found, aborted. Try again by press reset button."));
                        return;
                    }
                    break;
                case 'y':
                    if (y) {
                        target = ch;
                        address = Y_ADDRESS;
                        Serial.println(F("Selection: y-yellow"));
                        strcat(message, "y-yellow");
                    } else {
                        Serial.println(F("Selection: y-yellow, cartridge not found, aborted. Try again by press reset button."));
                        return;
                    }
                    break;
                case 'k':
                    if (k) {
                        target = ch;
                        address = K_ADDRESS;
                        Serial.println(F("Selection: k-black"));
                        strcat(message, "k-black");
                    } else {
                        Serial.println(F("Selection: k-black, cartridge not found, aborted. Try again by press reset button."));
                        return;
                    }
                    break;
                case 'a':
                    Serial.println(F("Selection: a-all present toner cartridge"));
                    if (op == 2) {
                        Serial.println(F("Error: a-all present toner cartridge can not use with custom data. Try again by press reset button."));
                        return;
                    } else {
                        target = ch;
                        strcat(message, "all present toner cartridge");
                    }
                    break;
                default:
                    Serial.print(F("Selection: "));
                    Serial.print(ch);
                    Serial.println(F("-unknown target, aborted. Try again by press reset button."));
                    return;
            }
            Serial.print(F("\nOperation: "));
            Serial.println(message);
            Serial.println(F("Please confirm to begin the operation: y-confirm, n-cancel"));
            while(!Serial.available());
            while(Serial.available()) {
                ch = Serial.read();
            }
            if (ch == 'n') {
                Serial.println(F("Selection: n-cancel, operation aborted. Press reset button to try again."));
            } else if (ch == 'y') {
                Serial.println(F("Selection: y-confirm, operation begin.\n"));
                if (target != 'a') {
                    if (op != 2) Serial.print(F("Preparing data..."));
                    byte buffer[128];
                    memcpy(buffer, CHIP_DATA_TEMPLATE, 128);
                    if (op != 2) {  // set to full or empty
                        if (op == 0) buffer[8] = 100;
                        else buffer[8] = 0;
                        if (target == 'k') {
                            buffer[5] = 1;
                            buffer[15] = 50;
                            buffer[20] = 22;
                        } else if (target == 'c') {
                            buffer[5] = 2;
                            buffer[15] = 51;
                            buffer[20] = 23;
                        } else if (target == 'm') {
                            buffer[5] = 3;
                            buffer[15] = 52;
                            buffer[20] = 24;
                        } else if (target == 'y') {
                            buffer[5] = 4;
                            buffer[15] = 53;
                            buffer[20] = 25;
                        }
                        Serial.println(F("done"));
                        Serial.print(F("Writing..."));
                        for (byte i=0; i<128; i++) {
                            i2cWrite(address, i, buffer[i]);
                        }
                        Serial.println(F("done"));
                        Serial.print(F("Verifying..."));
                        byte buffer2[128];
                        for(byte i=0;i<128;i++) {
                            buffer2[i] = i2cRead(address, i);
                        }
                        if (memcmp(buffer, buffer2, 128) == 0) {
                            Serial.println(F("valid"));
                            Serial.println(F("Operation: completed"));
                        } else {
                            Serial.println(F("invalid"));
                            Serial.println(F("Operation: failed"));
                        }
                    } else {    // write custom data
                        Serial.print(F("Writing..."));
                        size_t dataSize = sizeof(CUSTOM_DATA);
                        for (byte i=0; i<dataSize; i++) {
                            i2cWrite(address, i, CUSTOM_DATA[i]);
                        }
                        Serial.println(F("done"));
                        if (dataSize < 128) {
                            Serial.print(F("Filling remain addresses with zero..."));
                            for (byte i=dataSize; i<128; i++) {
                                i2cWrite(address, i, 0);
                            }
                            Serial.println(F("done"));
                        }
                        Serial.print(F("Verifying..."));
                        byte buffer2[dataSize];
                        for(byte i=0;i<dataSize;i++) {
                            buffer2[i] = i2cRead(address, i);
                        }
                        if (memcmp(CUSTOM_DATA, buffer2, dataSize) == 0) {
                            Serial.println(F("valid"));
                            Serial.println(F("Operation: completed"));
                        } else {
                            Serial.println(F("invalid"));
                            Serial.println(F("Operation: failed"));
                        }
                    }
                    
                } else {
                    for (uint8_t i=0; i<nDevices; i++) {
                        Serial.print(F("Preparing data for "));
                        byte buffer[128];
                        memcpy(buffer, CHIP_DATA_TEMPLATE, 128);
                        if (op == 0) buffer[8] = 100;
                        else buffer[8] = 0;
                        if (aDevices[i] == K_ADDRESS) {
                            Serial.print(F("k-black..."));
                            address = K_ADDRESS;
                            buffer[5] = 1;
                            buffer[15] = 50;
                            buffer[20] = 22;
                        } else if (aDevices[i] == C_ADDRESS) {
                            Serial.print(F("c-cyan..."));
                            address = C_ADDRESS;
                            buffer[5] = 2;
                            buffer[15] = 51;
                            buffer[20] = 23;
                        } else if (aDevices[i] == M_ADDRESS) {
                            Serial.print(F("m-magenta..."));
                            address = M_ADDRESS;
                            buffer[5] = 3;
                            buffer[15] = 52;
                            buffer[20] = 24;
                        } else if (aDevices[i] == Y_ADDRESS) {
                            Serial.print(F("y-yellow..."));
                            address = Y_ADDRESS;
                            buffer[5] = 4;
                            buffer[15] = 53;
                            buffer[20] = 25;
                        }
                        Serial.println(F("done"));
                        Serial.print(F("Writing..."));
                        for (byte i=0; i<128; i++) {
                            i2cWrite(address, i, buffer[i]);
                        }
                        Serial.println(F("done"));
                        Serial.print(F("Verifying..."));
                        byte buffer2[128];
                        for(byte i=0;i<128;i++) {
                            buffer2[i] = i2cRead(address, i);
                        }
                        if (memcmp(buffer, buffer2, 128) == 0) {
                            Serial.println(F("valid"));
                        } else {
                            Serial.println(F("invalid"));
                            Serial.println(F("Operation: aborted"));
                        }
                    }
                    Serial.println(F("Operation: completed"));
                }
            }
        } else return;
    }
}

void loop() {}

byte i2cRead(byte aAddress, byte bAddress) {    // device address, data address
    byte rData = 0;
    Wire.beginTransmission(aAddress);
    Wire.write(bAddress);
    Wire.endTransmission();

    Wire.requestFrom(aAddress, 1);
    while (Wire.available()){
        rData = Wire.read();
        return rData;
    }
}

void i2cWrite(char aAddress, byte bAddress, byte data) {    // device address, data address, data
    Wire.beginTransmission(aAddress);
    Wire.write(bAddress);
    Wire.write(data);
    Wire.endTransmission();
    delay(20);
}

bool headerCheck(byte aAddress) {
    if (i2cRead(aAddress, 0) == 168) return true;
    else return false;
}