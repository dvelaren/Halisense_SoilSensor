/*
  IndividualReadings.ino

  This example demonstrates how to read individual soil parameters
  one at a time instead of reading all parameters at once.

  Created by David Velásquez, March 2025
  Released under MIT License
*/

#include "SoilSensor.h"

// Choose which serial port to use (Serial1 or Serial2)
SoilSensor soilSensor(Serial2);

// Variables to store the last reading time
unsigned long lastReadTime = 0;
const unsigned long readInterval = 5000; // Read every 5 seconds

void setup()
{
    // Initialize the Serial Monitor
    Serial.begin(115200);
    while (!Serial)
    {
        ; // Wait for Serial to connect (needed for native USB port only)
    }
    Serial.println("Soil Sensor Individual Readings Example");

    // Initialize the soil sensor with baud rate
    if (soilSensor.begin(4800))
    {
        Serial.println("Soil sensor initialized successfully");
    }
    else
    {
        Serial.println("Failed to initialize soil sensor");
    }
}

void loop()
{
    unsigned long currentTime = millis();

    // Read sensor data at regular intervals
    if (currentTime - lastReadTime >= readInterval)
    {
        lastReadTime = currentTime;

        Serial.println("------- Individual Soil Sensor Readings -------");

        // Read temperature
        if (soilSensor.readTemperature())
        {
            Serial.print("Temperature: ");
            Serial.print(soilSensor.getTemperature());
            Serial.println(" °C");
        }
        else
        {
            Serial.println("Failed to read temperature");
        }

        // Read humidity
        if (soilSensor.readHumidity())
        {
            Serial.print("Humidity: ");
            Serial.print(soilSensor.getHumidity());
            Serial.println(" %");
        }
        else
        {
            Serial.println("Failed to read humidity");
        }

        // Read electrical conductivity
        if (soilSensor.readEC())
        {
            Serial.print("Electrical Conductivity: ");
            Serial.print(soilSensor.getEC());
            Serial.println(" µS/cm");
        }
        else
        {
            Serial.println("Failed to read electrical conductivity");
        }

        // Read pH
        if (soilSensor.readPH())
        {
            Serial.print("pH: ");
            Serial.println(soilSensor.getPH());
        }
        else
        {
            Serial.println("Failed to read pH");
        }

        // Read nitrogen
        if (soilSensor.readNitrogen())
        {
            Serial.print("Nitrogen: ");
            Serial.print(soilSensor.getNitrogen());
            Serial.println(" mg/kg");
        }
        else
        {
            Serial.println("Failed to read nitrogen");
        }

        // Read phosphorus
        if (soilSensor.readPhosphorus())
        {
            Serial.print("Phosphorus: ");
            Serial.print(soilSensor.getPhosphorus());
            Serial.println(" mg/kg");
        }
        else
        {
            Serial.println("Failed to read phosphorus");
        }

        // Read potassium
        if (soilSensor.readPotassium())
        {
            Serial.print("Potassium: ");
            Serial.print(soilSensor.getPotassium());
            Serial.println(" mg/kg");
        }
        else
        {
            Serial.println("Failed to read potassium");
        }

        Serial.println("---------------------------------------------");
    }
}