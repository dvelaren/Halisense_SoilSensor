#include "SoilSensor.h"

// Define the static command bytes
const byte SoilSensor::READ_ALL_REG[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08};
const byte SoilSensor::READ_TEMP_REG[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x01, 0xD5, 0xCA};
const byte SoilSensor::READ_HUM_REG[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
const byte SoilSensor::READ_EC_REG[8] = {0x01, 0x03, 0x00, 0x02, 0x00, 0x01, 0x25, 0xCA};
const byte SoilSensor::READ_PH_REG[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A};
const byte SoilSensor::READ_N_REG[8] = {0x01, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC5, 0xCB};
const byte SoilSensor::READ_P_REG[8] = {0x01, 0x03, 0x00, 0x05, 0x00, 0x01, 0x94, 0x0B};
const byte SoilSensor::READ_K_REG[8] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B}; // Fixed K register (was duplicate of P)

SoilSensor::SoilSensor(HardwareSerial &serial) : sensorSerial(serial),
                                                 _customPins(false),
                                                 temperature(0.0),
                                                 humidity(0.0),
                                                 ec(0),
                                                 ph(0.0),
                                                 nitrogen(0),
                                                 phosphorus(0),
                                                 potassium(0),
                                                 tempFresh(false),
                                                 humFresh(false),
                                                 ecFresh(false),
                                                 phFresh(false),
                                                 nFresh(false),
                                                 pFresh(false),
                                                 kFresh(false)
{
}

// Constructor for hardware serial with custom pins (for ESP32)
SoilSensor::SoilSensor(HardwareSerial &serial, int rxPin, int txPin) : sensorSerial(serial),
                                                                       _rxPin(rxPin),
                                                                       _txPin(txPin),
                                                                       _customPins(true),
                                                                       temperature(0.0),
                                                                       humidity(0.0),
                                                                       ec(0),
                                                                       ph(0.0),
                                                                       nitrogen(0),
                                                                       phosphorus(0),
                                                                       potassium(0),
                                                                       tempFresh(false),
                                                                       humFresh(false),
                                                                       ecFresh(false),
                                                                       phFresh(false),
                                                                       nFresh(false),
                                                                       pFresh(false),
                                                                       kFresh(false)
{
}

bool SoilSensor::begin(uint32_t baudRate)
{
    if (_customPins)
    {
// For ESP32, we can set custom pins
#if defined(ESP32)
        sensorSerial.begin(baudRate, SERIAL_8N1, _rxPin, _txPin);
#else
        // For other boards that don't support pin reassignment
        sensorSerial.begin(baudRate);
#endif
    }
    else
    {
        // Use default pins
        sensorSerial.begin(baudRate);
    }
    // sensorSerial.setTimeout(3000);
    // Clear sensor
    sensorSerial.write(READ_ALL_REG, 8);
    sensorSerial.flush();
    sensorSerial.write(READ_ALL_REG, 8);
    sensorSerial.flush();
    sensorSerial.write(READ_ALL_REG, 8);
    sensorSerial.flush();
    while (sensorSerial.available() > 0)
        char tempByte = sensorSerial.read();
    delay(100); // Give the serial port time to initialize
    return true;
}

bool SoilSensor::sendCommand(const byte *command, size_t length)
{
    // Clear the buffer
    while (sensorSerial.available() > 0)
        char tempByte = sensorSerial.read();

    // Send the command
    // Command sent...
    Serial.print("Command: ");
    for (int i = 0; i < length; i++)
    {
        if (command[i] < 0x10)
        {
            Serial.print("0");
        }
        Serial.print(command[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // Send the command
    sensorSerial.write(command, length);
    sensorSerial.flush();

    int readExpectedLength = command[5] == 0x07 ? 19 : 7;

    Serial.println("Expected length: " + String(readExpectedLength));

    int bytesRead = sensorSerial.readBytes(readBuffer, readExpectedLength);

    Serial.print("RAW Data: ");
    for (int i = 0; i < bytesRead; i++)
    {
        if (readBuffer[i] < 0x10)
            Serial.print("0");
        Serial.print(readBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    if (bytesRead != readExpectedLength)
    {
        Serial.println("Error reading bytes");
        return false;
    }

    // Verify device address and function code
    if (readBuffer[0] != 0x01 || readBuffer[1] != 0x03)
    {
        Serial.println("Device address error...");
        Serial.println("readBuffer[0]: " + String(readBuffer[0]) + " readBuffer[1]: " + String(readBuffer[1]));
        return false;
    }

    // Get expected data length and wait for all data
    int dataLength = readBuffer[2];
    int expectedBytes = dataLength + 5; // 1(addr) + 1(func) + 1(len) + dataLength + 2(CRC)
    Serial.println("Bytes read: " + String(bytesRead));
    Serial.println("Expected bytes: " + String(expectedBytes));
    // Check if we got all the expected data
    return (bytesRead == expectedBytes);
}

bool SoilSensor::readAllVariables()
{
    if (!sendCommand(READ_ALL_REG, 8))
    {
        return false;
    }

    return processReadingAll();
}

bool SoilSensor::processReadingAll()
{
    // The response format should be:
    // byte 0: sensor address (0x01)
    // byte 1: function code (0x03)
    // byte 2: byte count (0x0E for 7 variables, each with 2 bytes)
    // bytes 3-4: humidity value
    // bytes 5-6: temperature value
    // bytes 7-8: EC value
    // bytes 9-10: pH value
    // bytes 11-12: nitrogen value
    // bytes 13-14: phosphorus value
    // bytes 15-16: potassium value
    // bytes 17-18: CRC

    // Process the readings using the formula provided
    humidity = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF) / 10.0;
    temperature = (((readBuffer[5] << 8) | readBuffer[6]) & 0xFFF) / 10.0;
    ec = (((readBuffer[7] << 8) | readBuffer[8]) & 0xFFF);
    ph = (((readBuffer[9] << 8) | readBuffer[10]) & 0xFFF) / 10.0;
    nitrogen = (((readBuffer[11] << 8) | readBuffer[12]) & 0xFFF);
    phosphorus = (((readBuffer[13] << 8) | readBuffer[14]) & 0xFFF);
    potassium = (((readBuffer[15] << 8) | readBuffer[16]) & 0xFFF);

    // Mark all readings as fresh
    tempFresh = true;
    humFresh = true;
    ecFresh = true;
    phFresh = true;
    nFresh = true;
    pFresh = true;
    kFresh = true;

    memset(readBuffer, 0x00, 20);

    return true;
}

bool SoilSensor::readTemperature()
{
    if (!sendCommand(READ_TEMP_REG, 8))
    {
        Serial.println("Error sending temperature command");
        return false;
    }

    return processReadingTemperature();
}

bool SoilSensor::processReadingTemperature()
{
    // Extract temperature from the response
    temperature = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF) / 10.0;
    tempFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readHumidity()
{
    if (!sendCommand(READ_HUM_REG, 8))
    {
        return false;
    }

    return processReadingHumidity();
}

bool SoilSensor::processReadingHumidity()
{
    // Extract humidity from the response
    humidity = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF) / 10.0;
    humFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readEC()
{
    if (!sendCommand(READ_EC_REG, 8))
    {
        return false;
    }

    return processReadingEC();
}

bool SoilSensor::processReadingEC()
{
    // Extract EC from the response
    ec = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF);
    ecFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readPH()
{
    if (!sendCommand(READ_PH_REG, 8))
    {
        return false;
    }

    return processReadingPH();
}

bool SoilSensor::processReadingPH()
{
    // Extract pH from the response
    ph = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF) / 10.0;
    phFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readNitrogen()
{
    if (!sendCommand(READ_N_REG, 8))
    {
        return false;
    }

    return processReadingNitrogen();
}

bool SoilSensor::processReadingNitrogen()
{
    // Extract nitrogen from the response
    nitrogen = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF);
    nFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readPhosphorus()
{
    if (!sendCommand(READ_P_REG, 8))
    {
        return false;
    }

    return processReadingPhosphorus();
}

bool SoilSensor::processReadingPhosphorus()
{
    // Extract phosphorus from the response
    phosphorus = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF);
    pFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}

bool SoilSensor::readPotassium()
{
    if (!sendCommand(READ_K_REG, 8))
    {
        return false;
    }

    return processReadingPotassium();
}

bool SoilSensor::processReadingPotassium()
{
    // Extract potassium from the response
    potassium = (((readBuffer[3] << 8) | readBuffer[4]) & 0xFFF);
    kFresh = true;
    memset(readBuffer, 0x00, 20);
    return true;
}