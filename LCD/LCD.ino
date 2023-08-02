#include <LiquidCrystal.h>

const int xpin = A0;
const int ypin = A1;
const int zpin = A2;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop() {
  int x = analogRead(xpin);
  delay(1);
  int y = analogRead(ypin);
  delay(1);
  int z = analogRead(zpin);
  delay(1);

  float gx = 0.0149*x-4.91;
  float gy = 0.0154*y-5.1;
  float gz = 0.0152*z-5.17;

  Serial.print(gx);
  Serial.print("\t");
  Serial.print(gy);
  Serial.print("\t");
  Serial.print(gz);
  Serial.print("\n");

  lcd.setCursor(0,0);  
  lcd.print(abs(gx)); 
  lcd.setCursor(6,0);  
  lcd.print(abs(gy)); 
  lcd.setCursor(12,0);  
  lcd.print(abs(gz)); 

  delay(250);
}