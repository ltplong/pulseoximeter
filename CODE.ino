
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
float a,b;
int i =10;
float c= 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte smile[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
byte mod[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};
byte sad[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B01110,
  B10001,
  B00000,
  B00000
};

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{

  Serial.println("Beat!!!");

}

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.createChar(1 , smile);
  lcd.createChar(2 , mod);
  lcd.createChar(3 , sad);
  lcd.setCursor(0, 0);
  lcd.print("     Pluse");
  lcd.setCursor(0, 1);
  lcd.print("   Oximeter");
  delay(2000);

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  pox.update();

 if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
      a = pox.getHeartRate();
      b = pox.getSpO2();
          if( a == 0 || b == 0)
            {
              return;
            }

              if (i>0)
              {
                c = ((c+a)/2);
                i = i-1;
                Serial.println(i);
              }
             if (i==0)
              {
                i=10;
                Serial.print("Heart rate: ");
                Serial.print(c);
                Serial.print(" bpm & SpO2: ");
                Serial.print(b);
                Serial.println("%");
                
               lcd.setCursor(10,12); 
                lcd.print("Pulse Oximeter");     
                lcd.setCursor(0,35); 
                lcd.print("HeartR:");
                lcd.setCursor(62,35);
                lcd.print(a,0); 
                lcd.println(" bpm");
                lcd.setCursor(0,59);
                lcd.print("SpO2  : ");
                lcd.setCursor(62,59);
                lcd.print(b);
                lcd.println(" %");
               
                c=0;             
              } 
        tsLastReport = millis();
    }
    
}