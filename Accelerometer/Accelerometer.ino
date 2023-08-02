
const int xpin = A0;
const int ypin = A1;
const int zpin = A2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int x = analogRead(xpin);
  delay(1);
  int y = analogRead(ypin);
  delay(1);
  int z = analogRead(zpin);
  delay(1);

  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.print("\n");

  delay(100);
}
