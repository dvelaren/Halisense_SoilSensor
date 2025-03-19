/*
  ReadingsSerial.ino

  This example demonstrates how to read individual soil parameters
  one at a time or reading all parameters at once with Serial commands.

  Commands:
  T: Reads temperature
  H: Reads humidity
  p: Reads pH
  e: Reads electrical conductivity
  N: Reads nitrogen
  P: Reads phosphorus
  K: Reads potassium
  A: Reads all parameters


  Created by David Velásquez, March 2025
  Released under MIT License
*/

#include "SoilSensor.h"
// Define custom pins for serial communication
#define RX_PIN 16 // Custom RX pin
#define TX_PIN 17 // Custom TX pin

// Choose which serial port to use (Serial1 or Serial2)
SoilSensor soilSensor(Serial2, RX_PIN, TX_PIN);

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

void readSerial()
{
    if (Serial.available() > 0)
    {
        char comm = Serial.read();
        switch (comm)
        {
        case 'T':
            Serial.println("Reading Temperature...");
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
            break;
        case 'H':
            Serial.println("Reading Humidity...");
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
            break;
        case 'p':
            Serial.println("Reading pH...");
            if (soilSensor.readPH())
            {
                Serial.print("pH: ");
                Serial.println(soilSensor.getPH());
            }
            else
            {
                Serial.println("Failed to read pH");
            }
            break;
        case 'e':
            Serial.println("Reading Electrical Conductivity...");
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
            break;
        case 'N':
            Serial.println("Reading Nitrogen...");
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
            break;
        case 'P':
            Serial.println("Reading Phosphorus...");
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
            break;
        case 'K':
            Serial.println("Reading Potassium...");
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
            break;
        case 'A':
            Serial.println("Reading T, H, pH, EC, N, P, K...");
            if (soilSensor.readAllVariables())
            {
                printAllReadings();
            }
            else
            {
                Serial.println("Failed to read sensor data");
            }
            break;
        }
    }
}

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
    readSerial();
}