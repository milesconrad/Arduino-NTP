#define BUFFER_SIZE 25

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

char buffer[BUFFER_SIZE];
int byte_read;
int index = 0;

// initialize UART for ESP-01S
SoftwareSerial ESPSerial(0, 1);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

bool process() {
  if (ESPSerial.available() > 0) {
    byte_read = ESPSerial.read();
    buffer[index] = (char)byte_read;
    if (buffer[index] == ';') buffer[index] = ':';
    index++;

    return (byte_read == '\n' || index >= BUFFER_SIZE);
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  ESPSerial.begin(9600);
  while (!ESPSerial);

  while (String(buffer).indexOf("WiFi connected") == -1) {
    while (!process());
  }

  // Clear "WiFi connected" from the buffer
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }

  lcd.begin(16, 2);
  lcd.print("The time is: ");
}

void loop() {
  if (process()) {
    // Remove newline character from LCD output
    buffer[index - 1] = 0;

    lcd.setCursor(0, 1);
    lcd.print(buffer);

    index = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      buffer[i] = 0;
    }
  }
}