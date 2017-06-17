#include <Wire.h>
#include <BMP280_I2C.h>

BMP280_I2C dps = BMP280_I2C();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  Wire.begin();
  delay(1000);
  if (!dps.init()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t Pressure = dps.getPressure();
  int32_t Temperature = dps.temperature;
  Serial.print("Pressure(mmHg):");
  Serial.println(Pressure/(256 * 133.32));   //Выводим давление в мм.рт ст
  Serial.print("Temp:");
  Serial.println(Temperature * 0.01);   // Температуру в градусах Цельсия
  Serial.println("--------------");
  delay(5000);
}