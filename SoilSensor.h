#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include <Arduino.h>

class SoilSensor
{
public:
    // Constructor allows selecting Serial1 or Serial2
    SoilSensor(HardwareSerial &serial = Serial1);

    // Constructor for hardware serial with custom pins (for ESP32)
    SoilSensor(HardwareSerial &serial, int rxPin, int txPin);

    // Initialize the sensor with specified baud rate
    bool begin(uint32_t baudRate = 4800);

    // Read all variables at once (most efficient)
    bool readAllVariables();

    // Individual reading functions
    bool readTemperature();
    bool readHumidity();
    bool readEC();
    bool readPH();
    bool readNitrogen();
    bool readPhosphorus();
    bool readPotassium();

    // Get the latest read values
    float getTemperature() const { return temperature; }
    float getHumidity() const { return humidity; }
    float getEC() const { return ec; }
    float getPH() const { return ph; }
    float getNitrogen() const { return nitrogen; }
    float getPhosphorus() const { return phosphorus; }
    float getPotassium() const { return potassium; }

    // Check if readings are fresh (updated since last check)
    bool isTemperatureFresh()
    {
        bool temp = tempFresh;
        tempFresh = false;
        return temp;
    }
    bool isHumidityFresh()
    {
        bool temp = humFresh;
        humFresh = false;
        return temp;
    }
    bool isECFresh()
    {
        bool temp = ecFresh;
        ecFresh = false;
        return temp;
    }
    bool isPHFresh()
    {
        bool temp = phFresh;
        phFresh = false;
        return temp;
    }
    bool isNitrogenFresh()
    {
        bool temp = nFresh;
        nFresh = false;
        return temp;
    }
    bool isPhosphorusFresh()
    {
        bool temp = pFresh;
        pFresh = false;
        return temp;
    }
    bool isPotassiumFresh()
    {
        bool temp = kFresh;
        kFresh = false;
        return temp;
    }

private:
    HardwareSerial &sensorSerial;
    int _rxPin = -1;
    int _txPin = -1;
    bool _customPins = false;

    // Command bytes for each reading
    static const byte READ_ALL_REG[8];
    static const byte READ_TEMP_REG[8];
    static const byte READ_HUM_REG[8];
    static const byte READ_EC_REG[8];
    static const byte READ_PH_REG[8];
    static const byte READ_N_REG[8];
    static const byte READ_P_REG[8];
    static const byte READ_K_REG[8];

    // Buffer for receiving data
    byte readBuffer[20] = {0x00};

    // Sensor readings
    float temperature;
    float humidity;
    float ec;
    float ph;
    float nitrogen;
    float phosphorus;
    float potassium;

    // Flags for fresh data
    bool tempFresh;
    bool humFresh;
    bool ecFresh;
    bool phFresh;
    bool nFresh;
    bool pFresh;
    bool kFresh;

    // Helper function to send commands and read responses
    bool sendCommand(const byte *command, size_t length);

    // Process individual reading from buffer
    bool processReadingAll();
    bool processReadingTemperature();
    bool processReadingHumidity();
    bool processReadingEC();
    bool processReadingPH();
    bool processReadingNitrogen();
    bool processReadingPhosphorus();
    bool processReadingPotassium();
};

#endif // SOIL_SENSOR_H