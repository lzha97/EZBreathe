// Include libraries:
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins and max distance for Ultra Sound(HC_US):
#define trigPin  2 //create a Parallax Ping to use only one I/O ping
#define echoPin  2 //use one digital pin for both
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// pins for Ambubag FSR:
#define fsrpinA A1
#define ledpin 3

// pins for Body FSR:
#define fsrpinB A0
#define ledpin 2

//// LCD pins:
#define SDAPin A4 // Data pin
#define SCLPin A5 // Clock pin

// Sensor variables:
int fsrreading; // The current reading from the FSR
int state = HIGH; // The current state of the output pin
int previous = 0; // The previous reading from the FSR
long time = 0;  // The last time the output pin was toggled
long debounce = 40; // The debounce time, increase if the output flickers
float duration, distance; //Sonar variables

NewPing sonar(trigPin, echoPin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); // Connect to LCD via I2C:

// Breathing intervals
int URI = 30; // breathes per minute
int LRI = 10; //bpm
int CRI; // current rate interval bpm


void setup() {
  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  // Initiate the LCD:
  lcd.init();
  lcd.backlight();
  
  // Set ledpin as output response for now (can add speaker later):
  pinMode(ledpin, OUTPUT);
}

void loop() {
  
  // Monitor for patient paced inhilation
  fsrreading = analogRead(fsrpinB);
  Serial.println(fsrreading); // Print for programmer checks

  // If the input just went from below 500 to above 500 and we've waited long enough to ignore any noise on the circuit, toggle the output pin and remember the time:
  if (fsrreading > 500 && previous < 500 && millis() - time > debounce) {
    CRI = 60/(millis() - time);

    if (CRI < URI){
      state = HIGH;// Trig motor for inhalation
      //pump and reset timer
      time = millis();}
    else if (CRI > LRI){
      state = HIGH;// Trig motor for inhalation
      time = millis();}
    else
      state = LOW;
  }
  digitalWrite(ledpin, state); // to be replaced by motor code
  previous = fsrreading;


  // Ambubag pressure check
  delay(50); // (20 pings/sec)

  // Distance frequency array
  int iterations = 5; //take median distance reading for higher accuracy
  duration = sonar.ping_median(iterations);
  distance = (duration / 2) * 0.0343;

  Serial.print("Distance = ");
  Serial.print(distance); // Distance will be 0 when out of set max range.
  Serial.println(" cm");

  // Pressure frequency wave on Afsr
  // Monitor for ambubag force-generated pressure
  fsrreading = analogRead(fsrpinA);
  Serial.println(fsrreading); // Print for programmer checks
  if (fsrreading > 500 && previous < 500 && millis() - time > debounce) {
    state = HIGH;}
  
  if (distance > 12 && state){
    digitalWrite(ledpin, HIGH); // Warning for pressure drop
    lcd.setCursor(0,0);
    lcd.print(" " "\xDF" "C");
    lcd.setCursor(0,1);
    lcd.print("WARNING: Ambubag Pressure Drop!");
  }

 // Print stuff on LCD Serial Monitor:
  lcd.setCursor(0,0);
  lcd.print(" " "\xDF" "C");
  lcd.setCursor(0,1);
  lcd.print("Speed: ");
  lcd.print(duration);
  lcd.print(" m/s ");
  lcd.setCursor(0,2);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm  ");
  delay(100);

}
