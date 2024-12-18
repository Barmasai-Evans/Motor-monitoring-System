#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Definitions
const int currentPin = A0;
const int voltagePin = A1;
const int tempPin = A2;
const int irSensorPin = 2; // IR sensor connected to digital pin 2

// RPM Calculation Variables
volatile unsigned int pulseCount = 0; // Number of pulses detected
unsigned long lastUpdateTime = 0; // Last time RPM was calculated
float rpm = 0.0; // RPM value

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Motor Params:");
  delay(2000);

  // Initialize IR sensor pin
  pinMode(irSensorPin, INPUT);
  
  // Attach interrupt to IR sensor pin
  attachInterrupt(digitalPinToInterrupt(irSensorPin), countPulse, RISING);
}

void loop() {
  // Read current
  float current = analogRead(currentPin) * (5.0 / 1023.0);

  // Read voltage
  float voltage = analogRead(voltagePin) * (5.0 / 1023.0);

  // Read temperature
  float temp = analogRead(tempPin) * (5.0 / 1023.0) * 100;

  // Calculate RPM every second
  if (millis() - lastUpdateTime >= 1000) {
    detachInterrupt(digitalPinToInterrupt(irSensorPin)); // Disable interrupt temporarily
    rpm = (pulseCount * 60.0); // Convert pulses per second to RPM
    pulseCount = 0; // Reset pulse count
    lastUpdateTime = millis();
    attachInterrupt(digitalPinToInterrupt(irSensorPin), countPulse, RISING); // Re-enable interrupt
  }

  // Display data on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage, 2);
  lcd.print(" I:");
  lcd.print(current, 2);
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temp, 1);
  lcd.print("C RPM:");
  lcd.print(rpm, 0);

  delay(500); // Short delay for LCD refresh
}

// Interrupt Service Routine for counting pulses
void countPulse() {
  pulseCount++;
}
