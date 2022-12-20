#include "EmonLib.h"
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define _SS_MAX_RX_BUFF 512 // RX buffer size


//Function prototype
int32_t billing_calculate(double Energy);

//Software Serial
const byte rxPin = 2;
const byte txPin = 3;
// Set up a new SoftwareSerial object
SoftwareSerial mySerial (rxPin, txPin);

EnergyMonitor SCT013;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int pinSCT = A0;  //cảm biến kết nối với chân A0
double vol = 230;
double cong_suat;
int count;
double Irms;
String data;
double power;
int32_t Billing;
int times = 0;
int monthly = 0;

void setup() {
  SCT013.current(pinSCT, 60.606);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
}

void loop() {
  unsigned long previousMillis = millis();
  while ((millis() - previousMillis) < 915) {
    Irms += SCT013.calcIrms(1480) - 0.15;  // Calculate Irms only
    count++;
  }
  if (Irms < 0) {
    Irms = 0;
  }
  Irms = Irms / count;
  count = 0;
  cong_suat = Irms * vol;
  power += cong_suat/360.0; // 1Kwh = 3600000Ws (J)
  //Monthly reset
  monthly++;
  if(monthly == 30*24*3600) {
    power = 0;
  }
  //Calculating billing
  times++;
  if(times == 20) {
    times = 0;
    Billing = billing_calculate(power);
    String dataSent = String(Irms) + "," + String(cong_suat) + "," + String(power) + "," + String(Billing) + "\n";
    //Sent data to Esp
    // Serial.println(dataSent);
    mySerial.print(dataSent);
  }
  while(mySerial.available()) {
    Serial.write(mySerial.read());
  }
  //LCD display
  lcd.setCursor(0, 0);
  lcd.print("Dong dien: ");
  lcd.print(Irms);
  lcd.setCursor(0, 1);
  lcd.print("Cong suat: ");
  lcd.print(cong_suat);
}


int32_t billing_calculate(double Energy) {
    int32_t BillingFee = 0;
    const static int numberOfStages = 6;
    const static int32_t stage[] = {0, 50, 100, 200, 300, 400};
    const static int32_t bill[] = {1678, 1734, 2014, 2536, 2834, 2927};
    if (Energy <= 0) return 0;
    for(int i = 1; i <= numberOfStages; i++) {
        if(i != numberOfStages) {
            if(Energy <= stage[i]) {
                BillingFee += (Energy - stage[i - 1])*bill[i - 1];
                break;
            } else {
                BillingFee += (stage[i] - stage[i - 1])*bill[i - 1];
            }   
        } else {
            BillingFee += (Energy - stage[i - 1])*bill[i - 1];
        }
    }
    return BillingFee;
}