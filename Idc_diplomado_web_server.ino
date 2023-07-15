#include <ESP8266WiFi.h>
const char ssid[]    = "Cisco_Taller";      // SSID
const char password[] = "incorrecta22";      // Password
const char host[] = "192.168.1.197";  // IP serveur - Server IP
const int   port = 80;            // Port serveur - Server Port
const int   watchdog = 5000;        // Fréquence du watchdog - Watchdog frequency
unsigned long previousMillis = millis(); 
const int relayPin = 12;  //D6
const int relay2Pin = 14; //D5

#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT22   // DHT 22

#define dht_dpin 15

DHT dht(dht_dpin, DHTTYPE); 

void setup(void)
{ 
  dht.begin();
  Serial.begin(9600);
  pinMode(relayPin,OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  Serial.println("Humedad y Temperatura\n\n");
  delay(700);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Humedad = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("Temperatura = ");
    Serial.print(t); 
    Serial.println("°C  ");
    //Control del relay 
    if (t >= 30 ){
    digitalWrite(relayPin, LOW);
    Serial.println("Ventilador Encendido!");
    digitalWrite(relay2Pin, HIGH);
    Serial.println("Bombillo Apagado!");
    delay(1500);
    }
    //Control del relay2
    else if (t < 28 and t > 25 ){
    digitalWrite(relay2Pin, LOW);
    Serial.println("Bombillo Encendido!");
    digitalWrite(relayPin, HIGH);
    Serial.println("Ventilador Apagado");
    delay(1500);
    }
     
    unsigned long currentMillis = millis();
  
  if ( currentMillis - previousMillis > watchdog ) {
    previousMillis = currentMillis;
    WiFiClient client;
  
    if (!client.connect(host, port)) {
      Serial.println("Fallo al conectar");
      return;
    }

    String url = "/Idc_diplomado/index.php?temp=";
    url += t;
    url += "&hum=";
    url += h;
    
    // Enviamos petición al servidor
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
  
    // Leemos la respuesta del servidor
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  delay(800);
}
