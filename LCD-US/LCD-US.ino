/*
HC-SR04 ultrasonic distance sensor with Arduino and I2C LCD example code.
More info: https://www.makerguides.com/hc-sr04-arduino-tutorial/
*/

// Include the libraries:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Define Trig and Echo pin:
#define trigPin 2
#define echoPin 3

// Define SDA and SCL pin for LCD:
#define SDAPin A4 // Data pin
#define SCLPin A5 // Clock pin

// Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered):
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); //Change to (0x27,16,2) for 1602 LCD 

// Define variables:
long duration;
int distance;

void setup() {
  // Define inputs and outputs:
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin. This returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance:
  distance = duration*0.034/2;

  // Print the distance and temperature on the Serial Monitor:
  lcd.setCursor(0,0);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print(" " "\xDF" "C");
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  lcd.print(speedofsound);
  lcd.print(" m/s ");
  lcd.setCursor(0,2);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm  ");
  delay(100);
}
