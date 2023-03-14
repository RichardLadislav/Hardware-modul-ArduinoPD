// Tahový lineární potenciometr pro Arduino

#define pinOTA A0
#define pinOTB A1

void setup() {
  // komunikace po sériové lince rychlostí 9600 baud
  Serial.begin(9600);
}

void loop() {
  // načtení vstupních údajů z obou pinů
  int cteniOTA = analogRead(pinOTA);
  int cteniOTB = analogRead(pinOTB);
  // převedení průměru z obou měření ze vstupního rozsahu
  // na procentuální vyjádření
  int procenta = map((cteniOTA + cteniOTB) / 2, 0, 1023, 0, 100);
  // výpočet napájecího napětí na obou vstupních pinech
  float napetiOTA = cteniOTA * (5.0 / 1023.0);
  float napetiOTB = cteniOTB * (5.0 / 1023.0);
  // vytištění dostupných údajů po sériové lince
  Serial.print("Napeti na pinu OTA: ");
  Serial.print(napetiOTA);
  Serial.print(" V | Napeti na pinu OTB: ");
  Serial.print(napetiOTB);
  Serial.print(" V ");
  Serial.print(procenta);
  Serial.println("% ");
  // pauza před dalším měřením
  delay(500);
}
