// connect to wifi
#include <ESP8266WiFi.h>
// control sensor jarak
#include <NewPing.h>
// connect to http
#include <ESP8266HTTPClient.h>
// connect to https
#include <WiFiClient.h>

#define TRIGGER_PIN D0    // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN D1       // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define pinCH_1 D1        //Pin untuk Relay
int state;

// NewPing untuk setup pin dan maximum jarak.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "robotic";
char pass[] = "12345678";

// Menghubungkan ke telegram
String tokenBot = "6101043877:AAEc2IxLgHBq7mCv9serPFU0gGPppTXeOSU";
String serverName = "https://api.telegram.org/bot" + tokenBot + "/sendMessage";

void setup() {
  // Debug console
  Serial.begin(9600);

  Serial.print("Connecting To WIFI\n");
  // inisiasi connect wifi
  WiFi.begin(ssid, pass);

  // looping untuk memeriksa koneksi wifi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("*");
  }

  //Set Relay sebagai Output
  pinMode(pinCH_1, OUTPUT);  
  
}

void loop() {
  // Your Domain name with URL path or IP address with path
  String status = "";

  //Melakukan Pembacaan Jarak dalam bentuk cm
  int jarak = sonar.ping_cm();  
  //Kondisi untuk menyalakan relay berdasarkan nilai dari pembacaan sensor dan kontrol
  if (jarak >= 10 && jarak <=400) {
    digitalWrite(pinCH_1, LOW);
    Serial.print("Pompa Nyala\n");
    status = "Pompa%20Nyala%20";
   
  } else if (jarak < 10) {
    digitalWrite(pinCH_1, HIGH);
    Serial.print("Pompa Mati\n");
    status = "Pompa%20Mati%20";
  }
  String log = "jarak : " + String(jarak) + "\n";
  Serial.println(log);
  WiFiClientSecure client;
  HTTPClient https;

  // kalau sertifikasi https gk masuk tetap akan diterima
  client.setInsecure();

  Serial.print("Kirim Request Ke Telegram\n");
  https.begin(client, serverName + "?chat_id=1615663166&text=Status%20:%20" + status + "%20||%20Jarak%20:%20" + String(jarak));     
  https.GET();
  delay(1000);
}
