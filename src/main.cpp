#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

HardwareSerial mySerial(2); // Use UART2
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting DFPlayer Test...");

  // RX=16, TX=17 on ESP32 (Connects to DFPlayer TX, RX)
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error: DFPlayer not responding. Check wiring & SD card.");
    while(true);
  }
  
  Serial.println("DFPlayer Initialized!");
  myDFPlayer.volume(10); // Keep low for your 0.5W speaker
  delay(500);
  
  Serial.println("Playing Track 1...");
  myDFPlayer.play(1);    // Plays 0001.mp3
}

void loop() {
  // Do nothing, just play once
}