#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NEXT_PIN 5
#define PREV_PIN 4

bool nextPressed = true;
bool prevPressed = true;

int currentIndex = 0;

String reminders[] = {
  "your as5s is grass", 
  "tiger eat dragon     i eat you", 
  "jonathan michael, you are a menace", 
  "i hope u die <3", 
  "with love, CL"
};
int totalReminders = sizeof(reminders) / sizeof(reminders[0]);

void setup() {
  Serial.begin(9600);

  pinMode(NEXT_PIN, INPUT_PULLUP);
  pinMode(PREV_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display failed to load"));
    for (;;); // go into permanent loop so we don't break stuff 
  }

  display.clearDisplay();
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(reminders[currentIndex]);
  display.display();
}

void loop() {
  if (clicked(&nextPressed, NEXT_PIN)) {
    currentIndex = (currentIndex + 1) % totalReminders;
    updateDisplay();
  }
  if (clicked(&prevPressed, PREV_PIN)) {
    currentIndex = (currentIndex - 1 + totalReminders) % totalReminders;
    updateDisplay();
  }
}

bool clicked(bool *isPressed, int pin) {
  if (digitalRead(pin) == HIGH && !(*isPressed)) {
    *isPressed = true;
    return true;
  } else if (digitalRead(pin) == LOW) {
    *isPressed = false;
  }

  return false;
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(reminders[currentIndex]);
  display.display();
}
