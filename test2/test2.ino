#include "EmonLib.h"
#include <LiquidCrystal_I2C.h>

//Function prototype
int billing_calculate(float Energy);

EnergyMonitor SCT013;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int pinSCT = A0;  //cảm biến kết nối với chân A0
int vol = 230;
int cong_suat;
int count;
double Irms;
String data;
float power;
int Billing;

void setup() {
  SCT013.current(pinSCT, 60.606);
  Serial.begin(115200);
}

void loop() {
  unsigned long previousMillis = millis();
  while ((millis() - previousMillis) < 1000) {
    Irms += SCT013.calcIrms(1480) - 0.15;  // Calculate Irms only
    count++;
  }
  if (Irms < 0) {
    Irms = 0;
  }
  Irms = Irms / count;
  count = 0;
  cong_suat = Irms * vol;
  power = (power + cong_suat) / 3600;
  //Calculating billing
  Billing = billing_calculate(power);
  String dataSent = String(Irms) + "," + String(cong_suat) + "," + String(power) + "," + String(Billing);
  //Sent data to Esp
  Serial.println(dataSent);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Dong dien: ");
  lcd.print(Irms);
  lcd.setCursor(0, 1);
  lcd.print("Cong suat: ");
  lcd.print(cong_suat);
  delay(3000);

}


int billing_calculate(float Energy) {
    int BillingFee = 0;
    const static int numberOfStages = 6;
    const static int stage[] = {0, 50, 100, 200, 300, 400};
    const static int bill[] = {1678, 1734, 2014, 2536, 2834, 2927};
    if (Energy <= 0) return 0;
    for(int i = 1; i <= numberOfStages; i++) {
        if(i != numberOfStages) {
            if(Energy <= stage[i]) {
                BillingFee += (Energy - stage[i - 1])*bill[i - 1];
                break;
            } else {
                BillingFee += (stage[i] - stage[i-1])*bill[i - 1];
            }   
        } else {
            BillingFee += (Energy - stage[i - 1])*bill[i - 1];
        }
    }
    return BillingFee;
}