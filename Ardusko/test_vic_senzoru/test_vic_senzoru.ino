#include <NewPing.h>
#define pinTrigger    3
#define pinEcho       4
#define maxVzdalenost 450

NewPing sonar(pinTrigger, pinEcho, maxVzdalenost);

int val[6];
int vzdalenost;
void setup() {
  Serial.begin(9600);// put your setup code here, to run once:

}

void loop() {
  for(int i = 0; i<3; i++){
    val[i]=
    analogRead(i);
    Serial.print(val[i]);
    Serial.print(" ");
  }
    int vzdalenost = sonar.ping_cm();
  // pauza před dalším měřením
  delay(50);
  // pokud byla detekována vzdálenost větší než 0,
  // provedeme další měření
  if (vzdalenost > 0) {
    vzdalenost = 0;
    // pro získání stabilnějších výsledků provedeme 5 měření
    // a výsledky budeme přičítat do proměnné vzdalenost
    for (int i = 0; i < 5; i++) {
      vzdalenost += sonar.ping_cm();
      delay(50);
    }
    // v proměnné vzdálenost máme součet posledních 5 měření
    // a musíme tedy provést dělení 5 pro získání průměru
    vzdalenost = vzdalenost / 5;
    // vytištění informací po sériové lince
    
    Serial.print(vzdalenost);
    
  }
  Serial.println();
  delay(10);

}
