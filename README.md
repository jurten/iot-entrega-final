# iot-entrega-final

Instalación:
1. Ardunio 1.8.19
2. Mosquitto https://mosquitto.org/download/
    a. `sudo apt install mosquitto-clients`


Para levantar mosquitto
- `mosquitto_sub -t "name" -p 1884`
- `mosquitto_pub -t "name" -p 1884 -m "mensaje"`