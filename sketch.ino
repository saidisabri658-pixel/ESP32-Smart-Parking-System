#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define GREEN_LED 18
#define RED_LED 19

#define ENTRY_BUTTON 25
#define EXIT_BUTTON 26

const int MAX_PLACES = 4;
int occupiedPlaces = 0;

bool lastEntryState = HIGH;
bool lastExitState = HIGH;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void updateDisplay() {
  int freePlaces = MAX_PLACES - occupiedPlaces;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART PARKING");

  lcd.setCursor(0, 1);

  if (freePlaces == 0) {
    lcd.print("PARKING FULL!");
  } else {
    lcd.print("Free Spaces: ");
    lcd.print(freePlaces);
  }

  digitalWrite(GREEN_LED, freePlaces > 0 ? HIGH : LOW);
  digitalWrite(RED_LED, freePlaces == 0 ? HIGH : LOW);

  Serial.print("Available places: ");
  Serial.println(freePlaces);
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  pinMode(ENTRY_BUTTON, INPUT_PULLUP);
  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  Serial.println("Smart Parking System Started");

  updateDisplay();
}

void loop() {
  bool entryState = digitalRead(ENTRY_BUTTON);
  bool exitState = digitalRead(EXIT_BUTTON);

  if (lastEntryState == HIGH && entryState == LOW) {
    if (occupiedPlaces < MAX_PLACES) {
      occupiedPlaces++;
      Serial.println("Vehicle entered");
      updateDisplay();
    } else {
      Serial.println("Parking FULL!");
    }
  }

  if (lastExitState == HIGH && exitState == LOW) {
    if (occupiedPlaces > 0) {
      occupiedPlaces--;
      Serial.println("Vehicle exited");
      updateDisplay();
    }
  }

  lastEntryState = entryState;
  lastExitState = exitState;

  delay(100);
}
