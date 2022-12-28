/*
 * Calculates and displays the amount of water left in a water tank given its dimensions and mounted on the top of it.
 * Date: 15/01/2017 ----- updated to english at 28/12/2022
 * Author: Gabriel Noya
*/

#include <LiquidCrystal.h> // Include the LCD screen library

// --------- Declaring tank's dimension variables
float perimeter = 315.3; // Perimeter of the tank (cm)
float height = 129.0; // Height of the tank (cm)
float borderWidth = 0.4; // Width of the tank's walls (cm)

float internalRadius; // Internal radius of the tank (cm)

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Initialize the LCD library with the numbers of the interface pins

void setup() {
  Serial.begin(9600); // Serial com to debug via Serial port

  lcd.begin(16, 2);
  pinMode(3, OUTPUT); // Trigger pin
  pinMode(4, INPUT); // Echo pin

  lcd.setCursor(0, 0); // Start at the top left of the creen
  lcd.print("Volume: ");
  
  // --------- Calculate the external and internal radius of the tank
  float externalRadius = perimeter / (2 * PI);
  internalRadius = externalRadius - borderWidth;
}

void loop() {
  float waterDistance = getSensorDistance(); // Get the distance from top of tank to the water

  float waterLevel = height - waterDistance; // Calculate the height of the water
  int waterVolume = calculateWaterVolume(waterLevel, internalRadius); // Calculate the water volume

  displayWaterVolume(waterVolume); // Display to LCD screen
  delay(1000); // Delay so we do this once per second

  eraseDisplayRow(); // Erase LCD row
}

/*
  Returns the value of the distance sensor
*/
float getDistanceSensorValue() {
  digitalWrite(3, LOW); // Turn off trigger signal
  delayMicroseconds(2);

  digitalWrite(3, HIGH); // Turn oon trigger signal
  delayMicroseconds(10);

  digitalWrite(3, LOW); // Turn off trigger signal

  float sensorValue = pulseIn(4, HIGH); // Read the width of the pulse

  return sensorValue;
}

/*
  Returns the distance from an object to the distance sensor
*/
float getSensorDistance() {
  float sensorValue = getDistanceSensorValue();
  float waterDistance = sensorValue / 29 / 2; // Sensor convertion from value to cm

  return waterDistance;
}

/*
  Returns the volume of the water given its height and tank's radius
*/
int calculateWaterVolume(float waterLevel, float radius) {
  return PI * radius * radius * waterLevel / 1000; // Volume formula (liters);
}

/*
  Displays the water volume on the LCD screen
*/
void displayWaterVolume(int waterVolume) {
  Serial.println(waterVolume); // Debugging via Serial
  lcd.setCursor(0, 1); // Set LCD cursor to beginning of second row

  // --------- Show something in the LCD depending on volume

  // --------- Error case
  if (waterVolume < 0) {
    lcd.print("Error");
    return;
  }
  
  // --------- Too much water and might be flooding
  if (waterVolume > 940) {
    lcd.print("Too much water");
    return;
  }

  // --------- Running out of water
  if (waterVolume < 10) {
    lcd.print("Low water volume");
    return;
  }
  
  // --------- Okay level
  lcd.print(waterVolume); 
  lcd.setCursor(10, 1);
  lcd.print("liters");

  return;
}

/*
  Erases the second row of the LCD display
*/
void eraseDisplayRow() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
}