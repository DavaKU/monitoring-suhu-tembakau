#include "DHT.h"
#define DHTPIN D2     
#define DHTTYPE DHT11   // DHT 11
#include <ESP8266WiFi.h>

DHT dht(DHTPIN, DHTTYPE);
//Konfigurasi WiFi
const char *ssid = "ZTE_2.4G_HKdG6U";
const char *password = "87654321";
 
//IP Address Server yang terpasang XAMPP
const char *host = "192.168.1.25";
 
void setup() {
  Serial.begin(9600);

  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //Jika koneksi berhasil, maka akan muncul address di serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
int value = 0;
 
void loop() {
  // Proses Pengiriman -----------------------------------------------------------
  delay(60000);
  ++value;
 
  // Membaca Sensor Analog -------------------------------------------------------
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.println(t) ;
  Serial.print("connecting to ");
  Serial.println(host);
 
// Mengirimkan ke alamat host dengan port 80 -----------------------------------
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
 
// Isi Konten yang dikirim adalah alamat ip si esp -----------------------------
  String url = "/web-warriornux/write-data.php?data=";
  url += t;

 
  Serial.print("Requesting URL: ");
  Serial.println(url);
 
// Mengirimkan Request ke Server -----------------------------------------------
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 1000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
 
// Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
 
  Serial.println();
  Serial.println("closing connection");
}
