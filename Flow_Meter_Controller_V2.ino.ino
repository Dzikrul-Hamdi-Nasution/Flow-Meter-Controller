#include <Wire.h> // Library komunikasi I2C 
#include <LiquidCrystal_I2C.h> // Library modul I2C LCD
LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include <EEPROM.h>
int arus, memori, kondisi_memori, kondisi;
int sensorValue = 0;
const int analogInPin = A0;
int fx, rumus, hasil_baca_1, hasil_baca, kalibrasi_ulang, kalibrasi_ulang_2;
int output_arus;

void setup()
{
  Serial.begin(9600);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode (7, INPUT_PULLUP);
  pinMode (6, INPUT_PULLUP);
  pinMode (5, INPUT_PULLUP);
  pinMode (4, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  lcd.begin(16, 2);         // Initiate the LCD module
  lcd.backlight();
  lcd.setCursor(0, 0);
  // lcd.print("Flow Meter");
}
void loop()
{

  kondisi_memori = EEPROM.read(1);
  kondisi = kondisi_memori;
  sensorValue = analogRead(analogInPin);
  for (int x = 0; x < 20; x++) {
    sensorValue = analogRead(analogInPin);
    hasil_baca = hasil_baca + sensorValue;
    delay(50);
    if (digitalRead(7) == LOW) {
      // turn LED on:
      lcd.setCursor(0, 1);
      lcd.print("SP : ");
      memori = EEPROM.read(0);
      arus = memori;
      arus++;
      EEPROM.write(0, arus);
      lcd.print(arus);
    }

    if (digitalRead(6) == LOW) {
      // turn LED on:
      lcd.setCursor(0, 1);
      lcd.print("SP : ");
      memori = EEPROM.read(0);
      arus = memori;
      arus--;
      EEPROM.write(0, arus);
      lcd.print(arus);
      if (arus < 100) {
        lcd.setCursor(7, 1);
        lcd.print(" ");

      }
    }

    if (digitalRead(5) == LOW) {
      // turn LED on:
      if (digitalRead(4) == LOW) {
        // turn LED on:
        kondisi++;
        delay(300);
      }
      if (kondisi > 1) {
        kondisi = 0;
      }
      if (kondisi == 0) {
        lcd.setCursor(14, 1);
        lcd.print("A");
      }
      if (kondisi == 1) {
        lcd.setCursor(14, 1);
        lcd.print("M");
      }
      EEPROM.write(1, kondisi);
    }





    if (digitalRead(4) == LOW) {
      // turn LED on:
    }


  }

  memori = EEPROM.read(0);
  lcd.setCursor(0, 1);
  lcd.print("SP : ");
  lcd.print(memori);


  hasil_baca_1 = hasil_baca / 20;
  hasil_baca = 0;

  kalibrasi_ulang = map(hasil_baca_1, 26, 988, 0, 378);
  lcd.setCursor(0, 0);
  lcd.print("Flow : ");
  lcd.print(kalibrasi_ulang);
  if (kalibrasi_ulang < 100) {
    lcd.setCursor(9, 0);
    lcd.print(" ");
  }
  Serial.println(kalibrasi_ulang);
  output_arus = map(kalibrasi_ulang, 0, 354, 0, 255);
  
  if (kondisi == 1) {
    lcd.setCursor(14, 1);
    lcd.print("M");
  }
  if (kondisi == 0) {
    lcd.setCursor(14, 1);
    lcd.print("A");
    if (kalibrasi_ulang > arus) {
      analogWrite(3, output_arus);
    }
    else {
      analogWrite(3, 0);
    }
  }








  delay(80);

}
