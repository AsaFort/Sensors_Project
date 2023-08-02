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

#define filter_alpha 0.05

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);   //setting lcd screen dimendions
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } //stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));    //begin calibrating mpu6050
  lcd.setCursor(0,0);
  lcd.print("Do not move");
  lcd.setCursor(0,1);
  lcd.print("the MPU6050");
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero calibration
  Serial.println("Done!\n");
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Done!");
  delay(1000);
  lcd.clear();
}

void loop() {
  mpu.update();   //gather data from mpu6050

  int rawax = analogRead(xpin);   // ADXL335 data
  delay(1);
  int raway = analogRead(ypin);
  delay(1);
  int rawaz = analogRead(zpin);
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
  float yaw = mpu.getAngleZ();
  delay(1);
  float temp = (((mpu.getTemp())*(1.8))+32);    //mpu6050 temperature data converted to farenheit
  delay(1);

  float accAx = 0.0144*rawax-4.79;   //ADXL335 transfer function
  float accAy = 0.0152*raway-5.03;
  float accAz = 0.0150*rawaz-5.14;

  float filteredAcc = (filter_alpha * accAx) + ((1 - filter_alpha) * gyroAy);   //complementary filter function

  Serial.print("Ax: "); Serial.print(accAx);   //print ADXL335 data
  Serial.print("\t");
  Serial.print("Gyro Ay: "); Serial.print(gyroAy);    //print mpu6050 acceleration data
  Serial.print("\t");
  Serial.print("Filtered Acc: "); Serial.print(filteredAcc);    //print filtered acceleration data
  Serial.print("\t");
  Serial.print("Pitch: "); Serial.print(pitch);    //print mpu6050 gyro data
  Serial.print("\t");
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print("\t");
  Serial.print("Temp: "); Serial.print(temp);    //print mpu6050 temperature data
  Serial.print("\n");
  
  lcd.setCursor(0,0);   //sending data to lcd screen
  lcd.print("A:"); 
  lcd.setCursor(2,0);
  lcd.print(abs(filteredAcc));
  lcd.setCursor(10,0);  
  lcd.print("T:"); 
  lcd.setCursor(12,0);  
  lcd.print(temp); 
  lcd.setCursor(0,1);  
  lcd.print("P:"); 
  lcd.setCursor(2,1);  
  lcd.print(abs(pitch)); 
  lcd.setCursor(10,1);  
  lcd.print("R:"); 
  lcd.setCursor(12,1);  
  lcd.print(abs(roll)); 

  delay(500);   //sending data every half second, any quicker overloads the system
}