/*
  BasicReadings.ino

  This example demonstrates how to read parameters at once.

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
    Serial.println("Soil Sensor Example");

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

        // Option 1: Read all variables at once (more efficient)
        if (soilSensor.readAllVariables())
        {
            printAllReadings();
        }
        else
        {
            Serial.println("Failed to read sensor data");
        }

        // Option 2: Read individual variables as needed
        // Uncomment to use instead of readAllVariables()
        /*
        if (soilSensor.readTemperature()) {
          Serial.print("Temperature: ");
          Serial.print(soilSensor.getTemperature());
          Serial.println(" °C");
        }

        if (soilSensor.readHumidity()) {
          Serial.print("Humidity: ");
          Serial.print(soilSensor.getHumidity());
          Serial.println(" %");
        }
        */
    }
}

// Function to print all sensor readings
void printAllReadings()
{
    Serial.println("------- Soil Sensor Readings -------");

    Serial.print("Temperature: ");
    Serial.print(soilSensor.getTemperature());
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(soilSensor.getHumidity());
    Serial.println(" %");

    Serial.print("Electrical Conductivity: ");
    Serial.print(soilSensor.getEC());
    Serial.println(" µS/cm");

    Serial.print("pH: ");
    Serial.println(soilSensor.getPH());

    Serial.print("Nitrogen: ");
    Serial.print(soilSensor.getNitrogen());
    Serial.println(" mg/kg");

    Serial.print("Phosphorus: ");
    Serial.print(soilSensor.getPhosphorus());
    Serial.println(" mg/kg");

    Serial.print("Potassium: ");
    Serial.print(soilSensor.getPotassium());
    Serial.println(" mg/kg");

    Serial.println("------------------------------------");
}