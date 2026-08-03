#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SSID "The Compound"
#define WIFI_PWD "ConradHou$e!"
#define NTP_SRV "us.pool.ntp.org"
#define REFRESH_INTERVAL_MS 600000
#define BUFFER_SIZE 20

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
char buffer[BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, WIFI_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  timeClient.begin();
  timeClient.setTimeOffset(-14400);
}

void loop() {
  timeClient.update();

  int hours = timeClient.getHours();
  if (hours > 12) hours -= 12;
  if (hours == 0) hours = 12;
  sprintf(buffer, "%02i:%02i:%02i\n", hours, timeClient.getMinutes(), timeClient.getSeconds());

  Serial.print(buffer);

  delay(100);
}
