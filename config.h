// Dados MQTT
#define   IO_USERNAME   "YOUR_USERNAME"
#define   IO_KEY        "YOUR_KEY"

// Dados Wi-Fi
#define   WIFI_SSID   "YOUR_WIFI"
#define   WIFI_PASS   "YOUR_WIFI_PASSWORD"

// Dados para comunicacao
#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
