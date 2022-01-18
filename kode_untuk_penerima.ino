#include <ArduinoJson.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <WiFi.h>
WiFiClient client;
#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// kode ke firebase
#define API_KEY "AIzaSyDdgGGK5kXMEW3lLfOy_DsG1zeRO8uuXVU"

/* 3. Define the user Email and password that already registerd or added in your project */
#define USER_EMAIL "ibrohim@gmail.com"
#define USER_PASSWORD "1234567"

/* 4. If work with RTDB, define the RTDB URL */
#define DATABASE_URL "http://esp32-autentfikasi-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/** 5. Define the database secret (optional)
 * 
 * This database secret needed only for this example to modify the database rules
 * 
 * If you edit the database rules yourself, this is not required.
*/
#define DATABASE_SECRET "DATABASE_SECRET"

/* 6. Define the Firebase Data object */
FirebaseData fbdo;

/* 7. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 8. Define the FirebaseConfig data for config data */
FirebaseConfig config;
StaticJsonDocument<10000> doc;

String nod, suh,kel,sikap, stress;

void setup() {
//  pinMode(D3, OUTPUT);
//  pinMode(D4, OUTPUT);
  Serial.begin(9600);
 WiFi.mode(WIFI_STA);
 WiFiManager wm; 
 bool res;
// wm.resetSettings();
     res = wm.autoConnect("AutoConnectAP","password"); // password protected ap


    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yey terkoneksi");
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

/* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL */
    config.database_url = DATABASE_URL;

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);

    String base_path = "/UsersData/";

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    /** Assign the maximum retry of token generation */
    config.max_token_generation_retry = 5;

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    /** Now modify the database rules (if not yet modified)
     *
     * The user, <user uid> in this case will be granted to read and write
     * at the certain location i.e. "/UsersData/<user uid>".
     * 
     * If you database rules has been modified, please comment this code out.
     * 
     * The character $ is to make a wildcard variable (can be any name) represents any node key 
     * which located at some level in the rule structure and use as reference variable
     * in .read, .write and .validate rules
     * 
     * For this case $userId represents any <user uid> node that places under UsersData node i.e.
     * /UsersData/<user uid> which <user uid> is user UID.
     * 
     * Please check your the database rules to see the changes after run the below code.
    */
    String var = "$userId";
    String val = "($userId === auth.uid && auth.token.premium_account === true && auth.token.admin === true)";
    Firebase.RTDB.setReadWriteRules(&fbdo, base_path.c_str(), var.c_str(), val.c_str(), val.c_str(), DATABASE_SECRET);

    /** path for user data is now "/UsersData/<user uid>"
     * The user UID can be taken from auth.token.uid
    */


  Serial.println();
  Serial.println("connect");
  Serial2.begin(9600);
  delay(100);
  Serial.println();
}

void loop() {
//  digitalWrite(D3, LOW);
//  digitalWrite(D4, LOW);
  if (Serial2.available()) {
    String terima = Serial2.readString();
    if (terima != "") {
      DeserializationError error = deserializeJson(doc, terima);
      if (error) {
        Serial.println("ERROR JSON");
      } else {
		Serial.print("Data Json Diterima: "); Serial.print(terima); Serial.println();
        String ID = doc["node"];
        nod = ID;
        Serial.println("Node Alat: " + ID);
        String FV = doc["header"];
        Serial.println("Header: " + FV);
        String DT = doc["suhu"];
        suh = DT;
        Serial.println("Suhu: " + DT);
        String MS = doc["kelembaban"];
        kel = MS;
        Serial.println("Kelembaban: " + MS);
        String RS = doc["atitude"];
        sikap = RS;
        Serial.println("Keadaan: " + RS);
        String tail = doc["Tail"];
        //Serial.println("Tail: " + tail);
        String stres = doc["stress"];
        stress = stres;
                Serial.println("Stress: " + stres);
        Serial.println();
        kirim();
      }
    } else {
      Serial.println("DATA JSON KOSONG");
    }
  }
}


void kirim() {
    if (Firebase.ready())
    {
    String path = "/HealthCow/";
    path += auth.token.uid.c_str(); 
    String lokasi = path + "/node";
    Firebase.RTDB.setString(&fbdo, lokasi.c_str(), nod);
     lokasi = path + "/suhu";
    Firebase.RTDB.setString(&fbdo, lokasi.c_str(), suh);
    lokasi = path + "/kelembaban";
    Firebase.RTDB.setString(&fbdo, lokasi.c_str(), kel);
    lokasi = path + "/keadaan";
    Firebase.RTDB.setString(&fbdo, lokasi.c_str(), sikap);
    lokasi = path + "/stres";
    Firebase.RTDB.setString(&fbdo, lokasi.c_str(), stress);
        //Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, path.c_str(), udara) ? "ok" : fbdo.errorReason().c_str());
    //menyalakan LED saat dikirim
//    digitalWrite(ledkirim, HIGH);
    }
}
