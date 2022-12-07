#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define flamme  A0 
int fire;
char data;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT); //LED VERTE
  pinMode(8, OUTPUT); //LED ROUGE
  pinMode(flamme, INPUT);
  
   lcd.init(); // initialisation de l'afficheur
   lcd.backlight();
 // Envoi du message
 lcd.setCursor(0, 0);
 lcd.print("Fire NOT detected");
}

void loop() {

data=Serial.read();


 
  if(data=='5'){
lcd.clear();
lcd.backlight();
 // Envoi du message
 lcd.setCursor(0, 0);
 lcd.print("Fire ended");
    
 }
  fire = analogRead(flamme);


  if ( fire <= 200)
  {
    Serial.print("1");
    digitalWrite(8, LOW); //  ROUGE eteint
    digitalWrite(4,HIGH); 
    delay(3000);
    lcd.clear();
       lcd.backlight();
 // Envoi du message
 lcd.setCursor(0, 0);
 lcd.print("Fire detected");
  }

  else {
    Serial.print("2");
    digitalWrite(4, LOW); 
    digitalWrite(8, HIGH);// VERTE ALLUMEE
    delay(3000);
    
  }
}
