# Halisense SoilSensor Library for Arduino

![alt text](https://github.com/dvelaren/Halisense_SoilSensor/blob/main/resources/architecture.png?raw=true)

An Arduino library for interfacing with specialized soil sensors over serial communication. This library allows reading soil temperature, humidity, electrical conductivity, pH, nitrogen, phosphorus, and potassium levels from [Halisense 7 in 1 Sensor](https://www.jxct-iot.com/product/showproduct.php?id=197).

## Features

- Read 7 soil parameters: temperature, humidity, electrical conductivity, pH, nitrogen, phosphorus, and potassium
- Support for both default and custom serial pins (especially useful for ESP32)
- Individual or batch reading of all parameters
- Comprehensive error handling
- Easy to use API

## Hardware Compatibility

This library is designed to work with:

- Arduino boards with multiple hardware serial ports
- ESP32 boards with configurable serial pins
- Any board with a compatible soil sensor using the specified serial protocol

## Installation

### Using the Arduino Library Manager

1. Open the Arduino IDE
2. Go to Sketch > Include Library > Manage Libraries...
3. Search for "SoilSensor"
4. Click "Install"

### Manual Installation

1. Download the latest release from the GitHub repository
2. Extract the ZIP file
3. Move the extracted folder to your Arduino libraries directory
   - Windows: `Documents\Arduino\libraries\`
   - macOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
4. Restart the Arduino IDE

## Usage

### Basic Usage

```cpp
#include <SoilSensor.h>

// Create a sensor instance using default pins
SoilSensor soilSensor(Serial2);

void setup() {
  Serial.begin(115200);

  // Initialize the sensor
  if (soilSensor.begin(4800)) {
    Serial.println("Sensor initialized successfully");
  } else {
    Serial.println("Failed to initialize sensor");
  }
}

void loop() {
  // Read all parameters
  if (soilSensor.readAllVariables()) {
    // Display the readings
    Serial.print("Temperature: ");
    Serial.print(soilSensor.getTemperature());
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(soilSensor.getHumidity());
    Serial.println(" %");

    // ... and so on for other parameters
  }

  delay(5000); // Wait 5 seconds before next reading
}
```

### Using Custom Pins (ESP32)

```cpp
#include <SoilSensor.h>

// Define custom pins
#define RX_PIN 16
#define TX_PIN 17

// Create a sensor instance with custom pins
SoilSensor soilSensor(Serial2, RX_PIN, TX_PIN);

void setup() {
  Serial.begin(115200);

  // Initialize the sensor
  soilSensor.begin(9600);
}

// ... rest of the code
```

## Available Methods

### Initialization

- `SoilSensor(HardwareSerial& serial)`: Constructor using default pins
- `SoilSensor(HardwareSerial& serial, int rxPin, int txPin)`: Constructor using custom pins
- `bool begin(uint32_t baudRate = 9600)`: Initialize the sensor with the specified baud rate

### Reading Methods

- `bool readAllVariables()`: Read all soil parameters at once
- `bool readTemperature()`: Read only temperature
- `bool readHumidity()`: Read only humidity
- `bool readEC()`: Read only electrical conductivity
- `bool readPH()`: Read only pH
- `bool readNitrogen()`: Read only nitrogen
- `bool readPhosphorus()`: Read only phosphorus
- `bool readPotassium()`: Read only potassium

### Getter Methods

- `float getTemperature()`: Get the last temperature reading (°C)
- `float getHumidity()`: Get the last humidity reading (%)
- `float getEC()`: Get the last electrical conductivity reading (µS/cm)
- `float getPH()`: Get the last pH reading
- `float getNitrogen()`: Get the last nitrogen reading (mg/kg)
- `float getPhosphorus()`: Get the last phosphorus reading (mg/kg)
- `float getPotassium()`: Get the last potassium reading (mg/kg)

## Examples

The library comes with several examples demonstrating different features:

1. `BasicReading`: Simple example showing how to read all parameters
2. `CustomPins`: Shows how to use custom RX and TX pins on ESP32
3. `IndividualReadings`: Demonstrates reading parameters individually
4. `ReadingsSerial`: Reads parameters on demand by sending serial commands

## Folder Structure

```
SoilSensor/
├── examples/
│   ├── BasicReading/
│   │   └── BasicReading.ino
│   ├── CustomPins/
│   │   └── CustomPins.ino
│   ├── IndividualReadings/
│   │   └── IndividualReadings.ino
│   └── ReadingsSerial/
│       └── ReadingsSerial.ino
├── src/
│   ├── SoilSensor.cpp
│   └── SoilSensor.h
├── keywords.txt
├── library.properties
├── LICENSE
├── README.md
├── SoilSensor.cpp
└── SoilSensor.h
```

## License

This library is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

If you encounter any issues or have questions, please open an issue.
