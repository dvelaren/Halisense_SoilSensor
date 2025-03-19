/*
  CustomPins.ino

  This example demonstrates how to use the SoilSensor library with custom
  RX and TX pins on ESP32 boards. This allows for more flexible hardware
  design and pin allocation.

  Created by David Velásquez, March 2025
  Released under MIT License
*/

#include "SoilSensor.h"

// Define custom pins for serial communication
#define RX_PIN 16 // Custom RX pin
#define TX_PIN 17 // Custom TX pin

// Create a sensor instance with custom pins
SoilSensor soilSensor(Serial2, RX_PIN, TX_PIN);

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
    Serial.println("Soil Sensor Example with Custom Pins");

    // Initialize the soil sensor with baud rate
    if (soilSensor.begin(4800))
    {
        Serial.println("Soil sensor initialized successfully");
    }
    else
    {
        Serial.println("Failed to initialize soil sensor");
    }

    Serial.println("Using custom pins for serial communication:");
    Serial.print("RX Pin: ");
    Serial.println(RX_PIN);
    Serial.print("TX Pin: ");
    Serial.println(TX_PIN);
}

void loop()
{
    unsigned long currentTime = millis();

    // Read sensor data at regular intervals
    if (currentTime - lastReadTime >= readInterval)
    {
        lastReadTime = currentTime;

        // Read all variables at once
        if (soilSensor.readAllVariables())
        {
            printAllReadings();
        }
        else
        {
            Serial.println("Failed to read sensor data");
        }
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