// nRF24L01 vysílač

// připojení knihoven
#include <SPI.h>
#include "RF24.h"
// nastavení propojovacích pinů
#define CE 7
#define CS 8
// inicializace nRF s piny CE a CS
RF24 nRF(CE, CS);
// nastavení adres pro přijímač a vysílač,
// musí být nastaveny stejně v obou programech!
byte adresaPrijimac[]= "prijimac00";
byte adresaVysilac[]= "vysilac00";

void setup() {
  // komunikace přes sériovou linku rychlostí 9600 baud
  Serial.begin(9600);
  // zapnutí komunikace nRF modulu
  nRF.begin();
  // nastavení výkonu nRF modulu,
  // možnosti jsou RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX,
  // pro HIGH a MAX je nutný externí 3,3V zdroj
  nRF.setPALevel(RF24_PA_LOW);
  // nastavení zapisovacího a čtecího kanálu
  nRF.openWritingPipe(adresaVysilac);
  nRF.openReadingPipe(1,adresaPrijimac);
  // začátek příjmu dat
  nRF.startListening();
}

void loop() {
  // for smyčka pro postupné odeslání
  // hodnot 0 až 3 pro načtení všech dat
  // z přijímače
  for (int i=0; i < 4; i++ ) {
    // ukončení příjmu dat
    nRF.stopListening();
    // vytisknutí aktuální volby po sériové lince
    Serial.print("Posilam volbu ");
    Serial.println(i);
    // uložení startovního času komunikace
    unsigned long casZacatek = micros();
    // odeslání aktuální volby, v případě selhání
    // vytištění chybové hlášky na sériovou linku
    if (!nRF.write( &i, sizeof(i) )){
       Serial.println("Chyba při odeslání!");
    }
    // přepnutí do příjmu dat pro další komunikaci
    nRF.startListening();
    // uložení času začátku čekání
    unsigned long casCekaniOdezvy = micros();
    // proměnná s uložením stavu čekání na odezvu
    // od přijímače - "timeout"
    boolean timeout = false;
    // čekací while smyčka na odezvu od přijímače
    while ( ! nRF.available() ){
      // pokud do 200 000 mikrosekund nepřijde odezva,
      // ukonči čekání a nastav timeout
      if (micros() - casCekaniOdezvy > 200000 ){
          timeout = true;
          break;
      }      
    }
    // kontrola stavu timeoutu
    if ( timeout ){
      // v případě vypršení čekací smyčky,
      // vytiskni informaci o chybě spojení
      Serial.println("Chyba při prijmu, vyprseni casu na odezvu!");
    }
    // v opačném případě ulož přijatou zprávu a vypiš ji po sériové lince
    else{
        // proměnná pro uložení přijatých dat
        unsigned long prijataData;
        // příjem dat se zápisem do proměnné prijataData
        nRF.read( &prijataData, sizeof(prijataData) );
        // uložení času konce komunikace
        unsigned long casKonec = micros();
        // výpis dat z komunikace po sériové lince
        // včetně délky trvání spojení
        Serial.print("Odeslana volba: ");
        Serial.print(i);
        Serial.print(", prijata data: ");
        Serial.println(prijataData);
        Serial.print("Delka spojeni: ");
        Serial.print(casKonec - casZacatek);
        Serial.println(" mikrosekund.");
    }
    // pauza 1 sekunda
    delay(1000);
  }
}
