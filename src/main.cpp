#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobotDFPlayerMini.h>
#include <OneButton.h>

// Pins
#define DFPLAYER_RX_PIN 16
#define DFPLAYER_TX_PIN 17

#define BTN_LEFT_PIN   32
#define BTN_MID_PIN    33
#define BTN_RIGHT_PIN  13

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Audio & Buttons
HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;

OneButton btnLeft(BTN_LEFT_PIN, true);
OneButton btnMid(BTN_MID_PIN, true);
OneButton btnRight(BTN_RIGHT_PIN, true);

// Playlist Data (0001=Donkey, 0002=Goose, 0003=Sheep, 0004=Dog)
const char* animals[] = {"Donkey", "Goose", "Sheep", "Dog"};
const int numAnimals = sizeof(animals) / sizeof(animals[0]);
int currentPos = 0;

// Player State
bool isPlaylistMode = true; // true = Playlist Mode, false = Volume Mode
bool isPlaying = false;
bool isPaused = false;
bool isMuted = false;
int volume = 12;            // Safe default for 0.5W speaker (0-30)
int savedVolume = 12;

void updateOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // 1. Top Header: Active Mode
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (isPlaylistMode) {
    display.print("MODE: [PLAYLIST]");
  } else {
    display.print("MODE: [VOLUME]");
  }
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // 2. Center: Animal Name
  display.setTextSize(2);
  display.setCursor(0, 18);
  display.println(animals[currentPos]);

  // 3. Status: Playing / Paused
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Status: ");
  display.println(isPlaying ? "Playing" : "Paused");

  // 4. Bottom: Volume Bar & Level
  display.setCursor(0, 53);
  display.print("Vol:");
  int barWidth = map(isMuted ? 0 : volume, 0, 30, 0, 70);
  display.drawRect(28, 53, 72, 8, SSD1306_WHITE);
  if (barWidth > 0) {
    display.fillRect(29, 54, barWidth, 6, SSD1306_WHITE);
  }
  display.setCursor(104, 53);
  if (isMuted) {
    display.print("MUTE");
  } else {
    display.print(volume);
  }

  display.display();
}

void playCurrentTrack() {
  myDFPlayer.play(currentPos + 1); // DFPlayer tracks are 1-indexed
  isPlaying = true;
  isPaused = false;
  updateOLED();
}

void onLeftClick() {
  if (isPlaylistMode) {
    currentPos = (currentPos + numAnimals - 1) % numAnimals;
    playCurrentTrack();
  } else {
    if (isMuted) {
      isMuted = false;
      volume = savedVolume;
    }
    volume = max(0, volume - 3);
    myDFPlayer.volume(volume);
    updateOLED();
  }
}

void onRightClick() {
  if (isPlaylistMode) {
    currentPos = (currentPos + 1) % numAnimals;
    playCurrentTrack();
  } else {
    if (isMuted) {
      isMuted = false;
      volume = savedVolume;
    }
    volume = min(30, volume + 3);
    myDFPlayer.volume(volume);
    updateOLED();
  }
}

void onMidClick() {
  if (isPlaylistMode) {
    if (isPlaying) {
      myDFPlayer.pause();
      isPlaying = false;
      isPaused = true;
    } else if (isPaused) {
      myDFPlayer.start(); // Resume
      isPlaying = true;
      isPaused = false;
    } else {
      playCurrentTrack();
    }
    updateOLED();
  } else {
    isMuted = !isMuted;
    if (isMuted) {
      savedVolume = volume;
      myDFPlayer.volume(0);
    } else {
      volume = savedVolume;
      myDFPlayer.volume(volume);
    }
    updateOLED();
  }
}

void onMidDoubleClick() {
  isPlaylistMode = !isPlaylistMode;
  updateOLED();
}

void setup() {
  Serial.begin(115200);

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error: OLED failed to initialize.");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 25);
  display.println("Initializing Audio...");
  display.display();

  // Init DFPlayer
  mySerial.begin(9600, SERIAL_8N1, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error: DFPlayer failed to initialize.");
    display.clearDisplay();
    display.setCursor(0, 25);
    display.println("DFPlayer Error!");
    display.display();
    while (true);
  }

  myDFPlayer.volume(volume);
  delay(200);

  // Attach Buttons
  btnLeft.attachClick(onLeftClick);
  btnRight.attachClick(onRightClick);
  btnMid.attachClick(onMidClick);
  btnMid.attachDoubleClick(onMidDoubleClick);

  Serial.println("Audio Player Ready!");
  updateOLED();
}

void loop() {
  btnLeft.tick();
  btnMid.tick();
  btnRight.tick();

  // Check DFPlayer serial events when track finishes
  if (myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    if (type == DFPlayerPlayFinished) {
      isPlaying = false;
      isPaused = false;
      updateOLED();
    }
  }
}