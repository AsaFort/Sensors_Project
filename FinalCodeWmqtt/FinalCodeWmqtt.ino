#include <LiquidCrystal.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

const int xpin = A0;    //analog pins for ADXL335
const int ypin = A1;
const int zpin = A2;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

MPU6050 mpu(Wire);
unsigned long timer = 0;

#define filter_alpha 0.05

const char *ssid = "RedBlack";
const char *password = "DAwg$4697pitch";

const char broker[] = "test.mosquitto.org";
int port = 1883;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char endpoint[] = "afort/endpoint1";
char buffer[100];

void setup() {
  Serial.begin(9600);
  Wire.begin();

  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.println("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.print("WiFi connected");
  
  mqttClient.connect(broker,port);
  mqttClient.subscribe(endpoint);

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } //stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));    //begin calibrating mpu6050
  mpu.calcOffsets(); // gyro and accelero calibration
  Serial.println("Done!\n");
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

  mqttClient.beginMessage(endpoint);
  mqttClient.print(filteredAcc);
  mqttClient.print(" ");
  mqttClient.print(accAx);
  mqttClient.print(" ");
  mqttClient.print(gyroAy);
  mqttClient.endMessage();

  delay(1000);   //sending data every half second, any quicker overloads the system
}