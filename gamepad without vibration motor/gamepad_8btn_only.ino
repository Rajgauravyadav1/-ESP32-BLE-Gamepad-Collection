/*
 * ESP32 BLE Gamepad - 8 Buttons Only
 * ====================================
 * Layout:
 *   - 8 Digital Buttons (A, B, X, Y, LB, RB, START, SELECT)
 *   - No analog axes
 *
 * Perfect for: simple TV remotes, media controllers, turn-based games
 *
 * Wiring:
 *   BTN_A      → GPIO 12
 *   BTN_B      → GPIO 13
 *   BTN_X      → GPIO 14
 *   BTN_Y      → GPIO 27
 *   BTN_LB     → GPIO 26
 *   BTN_RB     → GPIO 25
 *   BTN_START  → GPIO 33
 *   BTN_SELECT → GPIO 32
 *
 *   All buttons wired: PIN → Button → GND (uses internal pull-up)
 *
 * Library: BleGamepad by lemmingDev
 */

#include <BleGamepad.h>

// ── Pin Definitions ──────────────────────────────────────────
#define BTN_A_PIN      12
#define BTN_B_PIN      13
#define BTN_X_PIN      14
#define BTN_Y_PIN      27
#define BTN_LB_PIN     26
#define BTN_RB_PIN     25
#define BTN_START_PIN  33
#define BTN_SEL_PIN    32

// ── Button Labels (for Serial debug) ─────────────────────────
const char* BTN_NAMES[] = { "A", "B", "X", "Y", "LB", "RB", "START", "SELECT" };
const int BTN_COUNT = 8;
const int btnPins[BTN_COUNT] = {
  BTN_A_PIN, BTN_B_PIN, BTN_X_PIN, BTN_Y_PIN,
  BTN_LB_PIN, BTN_RB_PIN, BTN_START_PIN, BTN_SEL_PIN
};

bool prevStates[BTN_COUNT] = {false};

// ── BLE Gamepad ───────────────────────────────────────────────
BleGamepad bleGamepad("ESP32 Gamepad 8B", "ESP32-DIY", 100);
BleGamepadConfiguration config;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 BLE Gamepad — 8 Buttons Only");

  for (int i = 0; i < BTN_COUNT; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }

  config.setAutoReport(false);
  config.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  config.setButtonCount(BTN_COUNT);

  // Disable all axes to keep packet small
  config.setIncludeXAxis(false);
  config.setIncludeYAxis(false);
  config.setIncludeZAxis(false);
  config.setIncludeRzAxis(false);
  config.setIncludeRxAxis(false);
  config.setIncludeRyAxis(false);
  config.setIncludeSlider1(false);
  config.setIncludeSlider2(false);

  bleGamepad.begin(&config);
  Serial.println("BLE advertising started.");
}

void loop() {
  if (!bleGamepad.isConnected()) {
    delay(100);
    return;
  }

  bool changed = false;

  for (int i = 0; i < BTN_COUNT; i++) {
    bool cur = !digitalRead(btnPins[i]);
    if (cur != prevStates[i]) {
      if (cur) {
        bleGamepad.press(i + 1);
        Serial.printf("Button %s PRESSED\n", BTN_NAMES[i]);
      } else {
        bleGamepad.release(i + 1);
        Serial.printf("Button %s RELEASED\n", BTN_NAMES[i]);
      }
      prevStates[i] = cur;
      changed = true;
    }
  }

  if (changed) {
    bleGamepad.sendReport();
  }

  delay(10);
}
