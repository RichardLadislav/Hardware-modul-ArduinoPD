// Arduino gyroskop a akcelerometr 2

// knihovny potřebné pro modul
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// nastavení adresy modulu
// 0x68 nebo 0x69, dle připojení AD0
// MPU6050 mpu(0x69)
MPU6050 mpu;
// číslo pinu s LED diodou pro notifikaci
#define LED_PIN 13 

// inicializace proměnných, do kterých se uloží data
bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

// inicializace proměnných pro výpočet
Quaternion q;           // [w, x, y, z] kvaternion
VectorFloat gravity;    // [x, y, z] vektor setrvačnosti
float rotace[3];        // rotace kolem os x,y,z

// Rutina přerušení
volatile bool mpuInterrupt = false;
void dmpINT() {
  mpuInterrupt = true;
}

void setup() {
  // nastavení LED jako výstupní
  pinMode(LED_PIN, OUTPUT);
  // nastavení I2C sběrnice
  Wire.begin();
  // komunikace přes sériovou linku rychlostí 115200 baud
  Serial.begin(115200);
  while (!Serial);
  // inicializace akcelerometru a gyroskopu
  //Serial.println(F("Inicializace I2C zarizeni.."));
  mpu.initialize();
  //Serial.println(F("Test pripojenych zarizeni.."));
  //Serial.println(mpu.testConnection() ? F("Modul pripojeni") : F("Pripojeni modulu selhalo"));
  // incializace DMP
  //Serial.println(F("Inicializace DMP..."));
  devStatus = mpu.dmpInitialize();
  // kontrola funkčnosti DMP
  if (devStatus == 0) {
      // zapnutí DMP
      //Serial.println(F("Povoleni DMP..."));
      mpu.setDMPEnabled(true);
      // nastavení pinu INT jako přerušovacího, interrupt 0 odpovídá pinu 2
      attachInterrupt(0, dmpINT, RISING);
      mpuIntStatus = mpu.getIntStatus();
      //Serial.println(F("DMP pripraveno, cekam na prvni preruseni.."));
      dmpReady = true;
      // načtení velikosti zpráv, které bude DMP posílat
      packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else {
      // V případě chyby:
      // 1 : selhání připojení k DMP
      // 2 : selhání při nastavení DMP
      //Serial.print(F("DMP inicializace selhala (kod "));
      //Serial.print(devStatus);
      //Serial.println(F(")"));
  }
  digitalWrite(LED_PIN, LOW);
}

void loop() {
// dokud nepošle DMP přerušení, můžeme provádět ostatní příkazy
// ve smyčce while níže
if (!dmpReady) return;
// tato podmínka čeká na příjem přerušení a můžeme v ní provádět
// ostatní operace
while (!mpuInterrupt && fifoCount < packetSize) {
    // místo pro ostatní operace
    // ..
}

// získání informace o statusu DSP
mpuInterrupt = false;
mpuIntStatus = mpu.getIntStatus();
// získání velikosti zásobníku dat
fifoCount = mpu.getFIFOCount();
// kontrola přetečení zásobníku dat
if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    //Serial.println(F("Preteceni zasobniku dat!"));
    // v případě přetečení zásobníku je nutné
    // častěji vyčítat data
}
else if (mpuIntStatus & 0x02) {
    // kontrola délky dat
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    // čtení paketu ze zásobníku
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    // při větším množství paketů snížíme počítadlo
    fifoCount -= packetSize;
    // přečtení dat z DSP a uložení do proměnných
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(rotace, &q, &gravity);

    // výpis informací o rotacích kolem jednotlivých os
    //Serial.print("Rotace \t X ");
    Serial.print(rotace[2] * 180/M_PI);
    Serial.print(" ");
    Serial.print(rotace[1] * 180/M_PI);
    Serial.print(" ");
    Serial.print(rotace[0] * 180/M_PI);
    Serial.println(" ");
}
}
