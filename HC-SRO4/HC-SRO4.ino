/* HC-SR04 ultrasonic distance sensor with NewPing library example code. More info: www.makerguides.com */

// Include the library:
#include <NewPing.h>

// Define pins and max distance:
#define trigPin  2 //create a Parallax Ping to use only one I/O ping
#define echoPin  2 //use one digital pin for both
#define MAX_DISTANCE 350 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


NewPing sonar(trigPin, echoPin, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
float duration, distance;

void setup() {
  Serial.begin(9600); // Open serial monitor at 9600 baud to see ping results.
}

void loop() {
  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  int iterations = 5; //take median distance reading for higher accuracy
  duration = sonar.ping_median(iterations);
  distance = (duration / 2) * 0.0343;

  Serial.print("Distance = ");
  Serial.print(distance); // Distance will be 0 when out of set max range.
  Serial.println(" cm");
}
