# ESP8266 + DHT22 + MQTT + Home-Assistant
Adaptation du projet http://www.projetsdiy.fr/esp8266-dht22-mqtt-projet-objet-connecte : 
- Supression de la partie LED et l'installation de Home-Assistant et Mosquitto
- Modification de la liste du matériel
- Ajout du deep-sleep mode pour une optimisation de la configuration
- Revue du code selon mon besoin

Vous pouvez retrouver l'intégralité de ce projet sur mon blog: 
https://rpinfo.ch/blog-post/controle-chauffage-electrique-et-mesure-de-consommation


## Matériel nécessaire
Pour réaliser ce projet vous aurez besoin des éléments suivants :
- Un module WiFi ESP8266 de préférence basé sur un ESP-12. J'utilise une D1 mini
- Une sonde de température DHT11 ou DHT22
- Un câble USB (pour programmer et alimenter l'ESP)

## Logiciels nécessaires
- Le Broker MQTT Mosquitto
- Un serveur Home-Assistant

Voir https://rpinfo.ch/blog-post/controle-chauffage-electrique-et-mesure-de-consommation-part-1 pour l'installation sur un raspberry pi


## Branchement
<img align="center" src="https://github.com/romain-pfund/esp8266-dht22-mqtt-home-assistant/blob/master/d1_mini_DHT22_bb.jpg" style="max-width:100%;">

## Code
Téléchargey et ouvrez le fichier DTH22.ino avec l'IDE Arduino
Si vous découvrez les modules ESP8266, lisez cet article qui explique comment les programmer avec l'IDE Arduino http://www.projetsdiy.fr/esp-01-esp8266-flasher-firmware-origine/

## Intégration MQTT + Home-Assistant
Connectez-bous sur votre server Home-Assistant et allez dans le répertoire de d'installation
dans notre cas : TBD

```sudo nano configuration.yaml```

Ajoutez une section mqtt
```
mqtt:
  broker: localhost          
  port: 1883                 #par défaut
  client_id: home-assistant-1
  keepalive: 60
  username: USERNAME  #optionnel
  password: PASSWORD   #optionnel
  protocol: 3.1              #par défaut
``` 

et maintenant deux sensors (température et humidité)
```
sensor:
  platform: mqtt
  state_topic: "sensor/temperature"
  name: "Température"
  qos: 0
  unit_of_measurement: "°C"
  #value_template: '{{ payload }}'

sensor 2:
  platform: mqtt
  state_topic: "sensor/humidity"
  name: "Humidité"
  qos: 0
  unit_of_measurement: "°C"
  #value_template: '{{ payload }}'
  ```
 

Rechargez la page d'Home-Assistant. Vous avez maintenant deux indicateurs (température et humidité)

<img align="center" src="https://github.com/romain-pfund/esp8266-dht22-mqtt-home-assistant/blob/master/mqtt+home-assistant.png" style="max-width:100%;">


