/*
  Création d'une sonde de température et humidité connectée (IoT)
  ESP8266 + DHT22 + LED + MQTT + Home-Assistant
  Projet RPinfo: https://rpinfo.ch/ - Mai 2021
  Article du projet : https://rpinfo.ch/blog-post/controle-chauffage-electrique-et-mesure-de-consommation-part-3

  Project d'origine:
  Projets DIY (http://www.projetsdiy.fr) - Mai 2016
  Article des projet : http://www.projetsdiy.fr/esp8266-dht22-mqtt-projet-objet-connecte/
                       https://projetsdiy.fr/esp8266-activer-mode-deep-sleep-wakeup-pir/?amp=1
  Licence : MIT
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"          // Librairie des capteurs DHT
#include <Ticker.h>

Ticker ticker;

#define wifi_ssid "SSID"
#define wifi_password "MOT-DE-PASSE"

#define mqtt_server "ADRESSE-IP-MOSQUITTO"
#define mqtt_user "guest"  //s'il a été configuré sur Mosquitto
#define mqtt_password "guest" //idem

#define temperature_topic "sensor/temperature"  //Topic température
#define humidity_topic "sensor/humidity"        //Topic humidité


bool debug = false;  //Affiche sur la console si True

#define DHTPIN D4    // Pin sur lequel est branché le DHT

// Dé-commentez la ligne qui correspond à votre capteur 
//#define DHTTYPE DHT11       // DHT 11 
#define DHTTYPE DHT22         // DHT 22  (AM2302)

//#define durationSleep  600 // 10 minutes
#define durationSleep  10 // secondes
#define NB_TRYWIFI     10 // nbr d'essai connexion WiFi, number of try to connect WiFi


//Création des objets
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);


void tick() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //Lecture de l'humidité ambiante
  float h = dht.readHumidity();
  // Lecture de la température en Celcius
  float t = dht.readTemperature();

  client.publish(temperature_topic, String(t).c_str(), true);   //Publie la température sur le topic temperature_topic
  client.publish(humidity_topic, String(h).c_str(), true);      //Et l'humidité
}


void setup() {
  Serial.begin(115200); //Facultatif pour le debug
  dht.begin();

  setup_wifi();
  ticker.attach(0.5, tick);

  ESP.deepSleep(durationSleep * 1000000);

}

//Connexion au réseau WiFi
void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connexion a ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  int _try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");
    delay(500);
    _try++;
    if (_try >= NB_TRYWIFI) {
      Serial.println("Impossible to connect WiFi network, go to deep sleep");
      ESP.deepSleep(durationSleep * 1000000);
    }
  }

  Serial.println("");
  Serial.println("Connexion WiFi etablie ");
  Serial.print("=> Addresse IP : ");
  Serial.print(WiFi.localIP());
}

//Reconnexion
void reconnect() {
  //Boucle jusqu'à obtenur une reconnexion
  while (!client.connected()) {
    Serial.print("Connexion au serveur MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, erreur : ");
      Serial.print(client.state());
      Serial.println(" On attend 5 secondes avant de recommencer");
      delay(5000);
    }
  }
}

void loop() {

}

