/*
 * Este programa calcula la cantidad de litros que posee un tanque de agua de dimensiones especÃ­ficas.
 * Fecha: 15/01/2017
 * Autor: Gabriel Noya 13-10982
*/
// include the library code:
#include <LiquidCrystal.h>

//Declaracion de variables
float perimetro= 315.3; // Perimetro del tanque (cm).
int y=129; // Distancia entre el fondo del tanque y el sensor (cm).
float deltaradio= 0.4; // Grosor de las paredes del tanque (cm).
float radioint; // Radio interno del tanque.
float radioext; // Radio externo del tanque.
float AguaSensor; // Distancia sensor-agua;
float h; // Distancia fondo del tanque - superficie del agua.
int Volumen; // Volumen disponible de agua.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(3, OUTPUT); //Trigger
  pinMode(4, INPUT); //Echo
  lcd.setCursor(0, 0);
  lcd.print("Volumen: ");
  
  //Calculos de medidas del tanque:
  radioext= perimetro / (2*PI);
  radioint= radioext - deltaradio;
}

void loop() {
  digitalWrite(3,LOW); // Apagamos la seÃ±al de trigger.
  delayMicroseconds(2);
  digitalWrite(3,HIGH); // Encendemos la seÃ±al de trigger.
  delayMicroseconds(10);
  digitalWrite(3,LOW); // Apagamos la seÃ±al de trigger.
  float sensorValue = pulseIn(4,HIGH); // Leemos el ancho del pulso del echo.
  AguaSensor = sensorValue / 29 / 2;
  h = y - AguaSensor; // Calculo del nivel del agua
  Volumen= PI*radioint*radioint*h/1000; // Formula de volumen en litros.
  Serial.println(Volumen);
  // Condiciones para la pantalla:
  
  if (Volumen < 0){
    lcd.setCursor(0, 1);
    lcd.print("Error");
  }
  else if (Volumen > 940){
    lcd.setCursor(0, 1);
    lcd.print("Demasiada agua");
  }
  else if (Volumen < 10){
    lcd.setCursor(0, 1);
    lcd.print("< 10");
    lcd.setCursor(10, 1);
    lcd.print("litros");
  }
  else{
  lcd.setCursor(0, 1);
  lcd.print(Volumen); 
  lcd.setCursor(10, 1);
  lcd.print("litros");
  }
  delay(500);
  // Borramos la segunda fila de la pantalla:
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

