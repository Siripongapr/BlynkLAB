/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#include <Wire.h>
//set blynk token
#define BLYNK_TEMPLATE_ID "xxxx"
#define BLYNK_TEMPLATE_NAME "xxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxx"
#define SHT31_ADDRESS 0x44 // ที่อยู่ของ SHT31
// #define relay 2
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "xxxxxxx";
char pass[] = "11111111";
uint8_t buffer[6];

float temperature;
float humidityPercent;
BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // conditon if value == 1 for turn on relay
  if (value) {
    digitalWrite(14, 0);
    Blynk.virtualWrite(V1,"ON");
  }
  else{
    digitalWrite(14, 1);
    Blynk.virtualWrite(V1,"OFF");
  }
}




void setup()
{
  // Debug console
  Serial.begin(115200);
  // set relay pin
  pinMode(14, OUTPUT);
  Wire.begin();
  //blynk begin send token , id , pass
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop()
{
  //blynk run
  Blynk.run();
  // get temperature & humidty value
  Wire.beginTransmission(SHT31_ADDRESS); // เริ่มการสื่อสารกับเซ็นเซอร์ SHT31 ที่ SHT31_ADDRESS
  Wire.write(0x24);                      // ส่งคำสั่งในการอ่านข้อมูลอุณหภูมิและความชื้น
  Wire.write(0x16);                      // ส่งคำสั่งในการเริ่มการวัด
  Wire.endTransmission();  
   int rv = Wire.requestFrom(SHT31_ADDRESS, 6);    // ขอข้อมูลจากเซ็นเซอร์ SHT31 ขนาด 6 ไบต์
  if (rv == 6)                                    // ตรวจสอบว่ามีข้อมูลทั้ง 6 ไบต์พร้อมใช้งานหรือไม่
  {
    for (uint8_t i = 0; i < 6; i++)
    {
      buffer[i] = Wire.read();                    // เก็บข้อมูลลง buffer
    }
  }
  uint16_t rawTemperature = (buffer[0] << 8) + buffer[1];   // อ่านข้อมูลอุณหภูมิ
  uint16_t rawHumidity    = (buffer[3] << 8) + buffer[4];   // อ่านข้อมูลความชื้น
  temperature = rawTemperature * (175.0 / 65535) - 45;     // คำนวณและแปลงข้อมูลอุณหภูมิ
  humidityPercent = rawHumidity * (100.0 / 65535);   

  // virtual wirte v2 , v3 virtualPin
  Blynk.virtualWrite(V2,temperature);
  Blynk.virtualWrite(V3,humidityPercent);
  //print temperature , humidity value
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidityPercent);
  Serial.println("%");
  // delay
  delay(2000);
}


