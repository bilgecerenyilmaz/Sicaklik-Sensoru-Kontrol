

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //I2C Adresi 0x27,16 sütun x2 satır
#define BUZZER_PIN 13 //Buzzer pini
#define POT_PIN 34 //Potansiyometre pini 
#define DHT_PIN 4  //DHT22 Sensör pini
#define DHT_TYPE DHT22  //sensör tipi

DHT dht(DHT_PIN, DHT_TYPE);  //DHT nesnesi

int alarmTemp = 0;  //alarm sıcaklık değeri

void lcd_write(int row,int col,String text){ //lcd için write fonksiyonu
  lcd.setCursor(col,row);
  lcd.print(text);
}
void setup() {
  Serial.begin(115200);//seri iletişim başlatma
  lcd.init(); 
  lcd.backlight();
  
  pinMode(BUZZER_PIN, OUTPUT); //buzzer pini çıkış olarak ayarlandı
  digitalWrite(BUZZER_PIN, LOW); //buzzer başlangıçta kapalı
  
  dht.begin();  

  float checkTemp = dht.readTemperature(); 
  if (isnan(checkTemp)) { //Sıcaklık okuması başarısızsa
    lcd_write(1,0,"Sensor Hatasi");
    Serial.println("DHT sensör hatası! Lütfen bağlantıları kontrol edin.");
  }
}

void loop() {
  int potValue = analogRead(POT_PIN); //potansiyometre analog değeri okunur
  alarmTemp = map(potValue, 0, 4095, 0, 80); //0-4095 arası değer 0-80 dereceye dönüştürüldü.
  
  lcd_write(0,0,"Alarm Sicakligi: ");
  lcd_write(1,0, String(alarmTemp)+" C  ");
  float sensorTemp = dht.readTemperature();  //sensörden sıcaklık değeri alındı

  if (isnan(sensorTemp)) {
    lcd_write(0,1,"Sensor Hatasi");
    Serial.println("DHT sensör hatası! Veriyi alamadık.");
  } else {
    Serial.print("Sensör Sıcaklık: ");
    Serial.println(sensorTemp);
    Serial.print("Alarm Sıcaklık: ");
    Serial.println(alarmTemp);
    if (alarmTemp < sensorTemp) { //Sensör sıcaklık alarmı aşılırsa
      tone(BUZZER_PIN, 1000); 
      Serial.println("Alarm! Potansiyometre değeri aşıldı");
    } else {
      noTone(BUZZER_PIN);      
      Serial.println("Normal");
    }
  }
  delay(1000); 
}