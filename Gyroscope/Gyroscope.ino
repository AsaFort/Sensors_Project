#include <LiquidCrystal.h>
#include "Wire.h"
#include <MPU6050_light.h>

const int xpin = A0;    //analog pins for ADXL335
const int ypin = A1;
const int zpin = A2;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);   //setting lcd screen dimendions
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } //stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));    //begin calibrating mpu6050
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero calibration
  Serial.println("Done!\n");
}

void loop() {
  mpu.update();   //gather data from mpu6050

  int x = analogRead(xpin);   // ADXL335 data
  delay(1);
  int y = analogRead(ypin);
  delay(1);
  int z = analogRead(zpin);
  delay(1);

  float gyroAx = mpu.getAccX();   //mpu6050 acceleration/g-force data
  delay(1);
  float gyroAy = mpu.getAccY();
  delay(1);
  float gyroAz = mpu.getAccZ();
  delay(1);

  float pitch = mpu.getAngleX();    //mpu6050 gyro data
  delay(1);
  float roll = mpu.getAngleY();
  delay(1);
  float temp = (((mpu.getTemp())*(1.8))+32);    //mpu6050 temperature data converted to farenheit
  delay(1);

  float gx = 0.0149*x-4.91;   //ADXL335 transfer function
  float gy = 0.0154*y-5.1;
  float gz = 0.0152*z-5.17;

  Serial.print("Ax: "); Serial.print(gx);   //print ADXL335 data
  Serial.print("\t");
  //Serial.print("Ay: "); Serial.print(gy);
  //Serial.print("\t");
  //Serial.print("Az: "); Serial.print(gz);
  //Serial.print("\t");
  //Serial.print("Gyro Ax: "); Serial.print(gyroAx);    //print mpu6050 acceleration data
  //Serial.print("\t");
  Serial.print("Gyro Ay: "); Serial.print(gyroAy);
  Serial.print("\t");
  //Serial.print("Gyro Az: "); Serial.print(gyroAz);
  //Serial.print("\t");
  Serial.print("Pitch: "); Serial.print(pitch);    //print mpu6050 gyro data
  Serial.print("\t");
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print("\t");
  Serial.print("Temp: "); Serial.print(temp);    //print mpu6050 temperature data
  Serial.print("\n");

  lcd.setCursor(0,0);   //sending data to lcd screen
  lcd.print(abs(gx)); 
  lcd.setCursor(6, 0);
  lcd.print(abs(gyroAy));
  //lcd.setCursor(6,0);  
  //lcd.print(abs(gy)); 
  //lcd.setCursor(12,0);  
  //lcd.print(abs(gz)); 
  lcd.setCursor(0,1);  
  lcd.print(abs(pitch)); 
  lcd.setCursor(6,1);  
  lcd.print(abs(roll)); 
  lcd.setCursor(12,1);  
  lcd.print(abs(temp));

  delay(500);   //sending data every half second, any quicker overloads the system
}