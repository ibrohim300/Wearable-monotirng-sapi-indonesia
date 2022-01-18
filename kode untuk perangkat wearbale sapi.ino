/*
Weabale Sapi by Ibrohim
18 Desember 2021
*/


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

#include <DHT.h>
  


//Inisialisasi Variabel DHT dan LED dan MPU
#define DHTPIN 4
DHT dht(DHTPIN, DHT11);
Adafruit_MPU6050 mpu;
#define M0 5
#define M1 27

//inisialisai sensing variael
float  suhu, kelembaban ;
String node = "M01";
String gerakansblm;
String keadaan;
float thi;
String stres;
//Inisialisai Variabel Waktu kirim rutin
unsigned long startMillis; 
unsigned long currentMillis;
const unsigned long period = 10000;  
char data;

void setup(){
  pinMode(M0,OUTPUT);
  pinMode(M1,OUTPUT);
  //memuali serial monitor dan LoRa
  Serial.begin(9600);
  Serial2.begin(9600);
  //Memulai Input LED yang dipakai
  pinMode(25, OUTPUT);
  //memulai waktu kirim
  startMillis = millis(); 
  //memulai Sensor MPU Akselerometer
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  //Memulai akselero dan gyro
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(2000);
  //memuali DHT11
  dht.begin();
  
}

void loop(){
	//membuat mode LED off dan mode LoRa sleep
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);
  digitalWrite(25, LOW);
  //membaca waktu dan sensor MPU
  currentMillis = millis(); 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
/*Program sensing posisi akselero dan DHT11*/

if ((a.acceleration.x >= -4.31 && a.acceleration.x <= 8.12) && (a.acceleration.y >= 6.06 && a.acceleration.y <= 10.16) && (a.acceleration.z >= -3.57 && a.acceleration.z <= 2.96))
  {
//  Serial.println("berdiri");  
    keadaan = "berdiri";
    } 
else if((a.acceleration.x >= -1.25 && a.acceleration.x <= 1.33) && (a.acceleration.y >= -2.93 && a.acceleration.y <= 9.01) && (a.acceleration.z >= 2.86 && a.acceleration.z <= 9.75))
  {
    keadaan = "rebahan";
    //Serial.println("rebahan belakang"); 
    }
else if((a.acceleration.x >= -4.61 && a.acceleration.x <= 3.9) && (a.acceleration.y >= -3.4 && a.acceleration.y <= 8.14) && (a.acceleration.z >= -11.35 && a.acceleration.z <= -6.25))
  {
    keadaan = "rebahan";
    //Serial.println("rebahan depan"); 
    }

//else if((a.acceleration.x >= -4.72 && a.acceleration.x <= 3.78) && (a.acceleration.y >= -1.91 && a.acceleration.y <= 8.23) && (a.acceleration.z >= -11.88 && a.acceleration.z <= -5.12))
//  {
//    keadaan = "rebahan";
//  //  Serial.println("rebahan");  
//    }
else
  {
    
    //digitalWrite(25, LOW);
    }


  while (gerakansblm != keadaan && keadaan != "lain") {
      bacanilai();
      lorakirim();
      gerakansblm = keadaan;
      Serial.println("data telah mdikirim");
    }



//Menghasilkan nilai random	
//	for (int i = 0; i < 10; i++) {
//    atitude_acak = random(1, 5);
//    udara = random(50, 100);
//    int randomsuhu = random(90, 140);
//    suhu = ceilf((randomsuhu / 3.33) * 100) / 100;
//    int randomkelembaban = random(200, 300);
//    kelembaban = ceilf((randomkelembaban / 3.33) * 100) / 100;
//    int aX = random(1, 10);
//    int aY = random(1, 10);
//    int aZ = random(1, 10);
  if (currentMillis - startMillis >= period) 
  {
    startMillis = currentMillis;  
      bacanilai();
      lorakirim();
  }

    Serial.println(keadaan);
    delay(100);
  
}

void lorakirim() {  //membuat format json
//menyalakan LED saat dikirim dan membangunkan LoRa dari mode sleep
digitalWrite(25, HIGH);
digitalWrite(M0, LOW);
digitalWrite(M1, LOW);
  delay(1000);
DynamicJsonDocument doc(1000);
doc["node"] = node;
doc["header"] = "MKR2021";
doc["suhu"] = suhu;
doc["kelembaban"] = kelembaban;
doc["atitude"] = keadaan;
doc["stress"] = stres;
doc["Tail"] = "#";
  String data = "";
  serializeJson(doc, data);
  serializeJson(doc, Serial);
  serializeJson(doc, Serial2);
  Serial.println();

}

void bacanilai() {
    //menampung data variabel DHT11

  float h;                  // Variabel lokal kelembaban
  float t;                  //Variabel Lokal Kelembaban

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    h = 0;
    t = 0;
  }
  h = dht.readHumidity();
  t = dht.readTemperature();
  suhu = t;
  kelembaban = h;
  thi = ((1.8*suhu*+32)-((0.55-(0.0055*h*0.01))*(1.8*t-26.8)));		
  if(thi>=70 and thi <=79){
	  stres="kecil";  `
  }
  else if(thi>=80 and thi <=89){
	  stres="sedang";  
  }
  if(thi>=90 and thi <=98){
	  stres="berat";  
  }
  if(thi>=99){
	  stres="mati";  
  }
//  prosesco2();
 // udara = averageValue.average();
}
