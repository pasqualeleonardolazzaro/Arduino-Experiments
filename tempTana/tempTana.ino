#include <LiquidCrystal.h>
#include "pitch.h"
#define redLed_pin 6
#define tone_pin 5
#define analog_temperature A1

LiquidCrystal lcd(0, 1, 7, 8, 9, 10);
int row_temperature = 0;
int celsius = 0;
void setup() {
  pinMode(analog_temperature, INPUT);
  pinMode(tone_pin, OUTPUT);//buzzer
  Serial.begin(9600);
  lcd.begin(16, 2); 
  pinMode(redLed_pin, OUTPUT);
  digitalWrite(redLed_pin, LOW);
  int noteDuration        = 0;
  int pauseBetweenNotes   = 0;
  int sizeMelody          = sizeof(melodyJingleBells)/sizeof(int);

  for (int i = 0; i < sizeMelody; i++) {

    // Calcola la durata della nota in millisecondi.
    noteDuration = 1000 / timeJingleBells[i];

    tone(tone_pin, melodyJingleBells[i], noteDuration);

    // Per distingure le note viene settato una pausa tra una nota e l'altra
    // Questo intervallo è pari al 30% della durata.      
    pauseBetweenNotes = noteDuration * 1.30;
    
    delay(pauseBetweenNotes);
  }
}

// Heart
byte fullHeart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
void loop() {
  float meanTemperature = 0.0;
  for(int i = 0 ; i < 100 ; i++){
  row_temperature = analogRead(analog_temperature);
  celsius = ((row_temperature/1023.0*3.3*1000) -500 )/10 ;
  meanTemperature += celsius;
  }
  meanTemperature = meanTemperature/100.0;
  Serial.print(meanTemperature);
  Serial.println(" C.");
  //se la tempeatura è inferiore a 19 gradi accendi il led rosso
  if(meanTemperature < 19){
    digitalWrite(redLed_pin, HIGH);
  }
  else {
    digitalWrite(redLed_pin, LOW);
  }
// Costruisci la stringa da visualizzare sul display
  String displayString = "Temperatura: " + String(meanTemperature, 1) + (char)223 + "C";
  String welcomeString = "Benvenuti nella tana";

  //initialize heart character
  lcd.createChar(1, fullHeart);
  // Controlla se la lunghezza della stringa supera la larghezza del display
  if (displayString.length() > 16) {
    // Scorri la stringa sul display
    for (int i = 0; i < displayString.length(); ++i) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(welcomeString.substring(i, i + 16));
      if(welcomeString.substring(i, i + 16).length() < 16) {
      //initialize heart character
        lcd.createChar(1, fullHeart);
        lcd.setCursor(welcomeString.substring(i, i + 16).length(), 0);
        lcd.write(byte(1));
      }
      lcd.setCursor(0, 1);
      lcd.print(displayString.substring(i, i + 16));
      delay(700); // Puoi regolare la velocità dello scrolling modificando questo valore
    }
  } else {
    // La stringa è abbastanza corta da essere visualizzata interamente
    lcd.print(displayString);
  }
  delay(500);
}