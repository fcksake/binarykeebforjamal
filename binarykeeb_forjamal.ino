#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keyboard.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ZERO_PIN 5
#define ONE_PIN 4

// I wanted to use interrupts but I couldn't get it working
// I'll research it more later
bool zeroPressed = true;
bool onePressed = true;

unsigned int currentByte = 0;
unsigned int count = 0;

String debugData = "";

String reminders[] = {"your ass is grass", "tiger eat dragon     i eat you", "jonathan michael, you are a menace", "i hope u die <3", "your ass is grass",  "with love, CL"};
/*
String binaryInputString = ""; // To store binary inputs
String outputString = "";      // To store the resulting characters
*/

/**
 * Welcome to hacky hack ville, population: this code
 * Warning, may(does) contain, bad practices because it's 3AM and I'm tired
 */
void setup() {
  Serial.begin(9600);

  pinMode(ZERO_PIN, INPUT_PULLUP);
  pinMode(ONE_PIN, INPUT_PULLUP);


  // Generate display voltage from 3.3 internally - from SSD1306 example
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display failed to load"));
    for (;;); // go into permanenet loop so we don't break stuff 
  }
/*
  // Assuming 'bit' is the variable that holds the current binary input (0 or 1)
    binaryInputString += (currentByte == 0 ? "0" : "1");

    // After forming a complete byte and converting it to a character
    // Let's say 'currentChar' is the variable that holds the resulting character
    outputString += currentChar;

    // Call a function to update the display
    updateDisplay();
*/

// Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Check your display's address
  display.clearDisplay();

  // Array of custom text options
  String customTextOptions[] = {"tiger eat dragon     i eat you", "jonathan michael, you are a menace", "i hope u die <3", "your ass is grass",  "with love, CL"};
  int numberOfOptions = sizeof(customTextOptions) / sizeof(customTextOptions[0]);

  // Initialize random seed
  randomSeed(analogRead(0));

  // Select and display random text
  String selectedText = customTextOptions[random(numberOfOptions)];
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(selectedText);
  display.display();

  // Delay for 2.5 seconds
  delay(2500);

  Keyboard.begin();
  drawBits();
}



void loop() {
//  delay(10);
  
  if (clicked(&zeroPressed, ZERO_PIN)) {
    addBit(0);
  }
  if (clicked(&onePressed, ONE_PIN)) {
    addBit(1);
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

void addBit(int b) {
  // Shift bits left
  currentByte <<= 1;
  // Modify least significant bit
  currentByte |= b;
  
  count++;

  if (count == 8) {
    Keyboard.write((int)currentByte);
    currentByte = 0;
    count = 0;
  }
  
  drawBits();

}

/*
void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Display binary input string
    display.setCursor(0, 0);
    display.println(binaryInputString);

    // Display the output string below the binary string
    display.setCursor(0, 15); // Adjust Y-coordinate as needed
    display.println(outputString);

    display.display();
}
*/
void drawBits() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,8);
  String output = "";

  for (int i = count; i > 0; i--) {
    output += ((currentByte >> i-1) & 1) == 0 ? "0" : "1";
  }

  for (int i = count; i <= 7; i++) {
    output += "_";
  }

  output += "";
  display.println(output);

  
/* display.setTextSize(1);
 String withLove = " plug me into le computer :)";
 display.println(withLove);
 */

  
  display.display();
}
