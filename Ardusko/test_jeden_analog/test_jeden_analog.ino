int knobValue = 0;
void setup() {
  Serial.begin(9600);

}

void loop() {
  knobValue = analogRead(0);
  Serial.print(knobValue);
  Serial.print(" ");
  delay(5);
}
