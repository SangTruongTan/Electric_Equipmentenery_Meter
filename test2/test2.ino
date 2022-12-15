#include "EmonLib.h"
#include <LiquidCrystal_I2C.h>
EnergyMonitor SCT013;
LiquidCrystal_I2C lcd(0x27,16,2);
int pinSCT = A0; //cảm biến kết nối với chân A0
int vol = 230;
int cong_suat;
int count;
double Irms;
String data;
float power ;
void setup()
{
SCT013.current(pinSCT, 60.606);
Serial.begin(115200);
}
void loop()
{
  unsigned long previousMillis = millis();
    while ((millis() - previousMillis) < 1000)
  {
    Irms += SCT013.calcIrms(1480) - 0.15;  // Calculate Irms only
    count++;
  }

 if ( Irms < 0) {
  Irms = 0 ;
 }
Irms = Irms / count ;
cong_suat = Irms * vol;
power=(power+cong_suat)/3600;


//Serial.println(Irms);
//Serial.println(cong_suat);
//send_data(Irms,cong_suat);
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Dong dien: ");
  lcd.print(Irms);
  lcd.setCursor(0,1);
  lcd.print("Cong suat: ");
  lcd.print(cong_suat);
delay(3000);
  
String data = "a" + String(Irms) + "b" + String(cong_suat) +"c" +String(power) +"d" ;
  Serial.print(data);
}


//void send_data(float Irms, float cong_suat){
 // String string_data = "";
  //string_data += String(Irms) + String(cong_suat);
 // Serial.println(string_data);
//}