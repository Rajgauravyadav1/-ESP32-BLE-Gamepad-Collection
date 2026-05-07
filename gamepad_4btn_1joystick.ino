/*
 * ESP32 BLE Gamepad - 4 Buttons + 1 Joystick
 * ============================================
 * Layout:
 *   - 4 Digital Buttons (BTN_A, BTN_B, BTN_X, BTN_Y)
 *   - 1 Analog Joystick (X-axis, Y-axis)
 *
 * Wiring:
 *   BTN_A  → GPIO 12
 *   BTN_B  → GPIO 13
 *   BTN_X  → GPIO 14
 *   BTN_Y  → GPIO 27
 *   JOY_X  → GPIO 34 (ADC)
 *   JOY_Y  → GPIO 35 (ADC)
 *
 * Library: BleGamepad by lemmingDev
 * Install via Arduino Library Manager
 */

#include <BleGamepad.h>

// ── Pin Definitions ──────────────────────────────────────────
#define BTN_A_PIN   12
#define BTN_B_PIN   13
#define BTN_X_PIN   14
#define BTN_Y_PIN   27
#define JOY_X_PIN   34
#define JOY_Y_PIN   35

// ── Joystick Calibration ─────────────────────────────────────
#define JOY_DEADZONE   200     // Raw ADC deadzone around center (0–4095)
#define JOY_CENTER     2048    // Expected ADC center value
#define JOY_MIN        0
#define JOY_MAX        4095
#define BLE_AXIS_MIN  -32767
#define BLE_AXIS_MAX   32767

// ── BLE Gamepad Instance ──────────────────────────────────────
BleGamepad bleGamepad("ESP32 Gamepad 4B+J", "ESP32-DIY", 100);

BleGamepadConfiguration config;

// ── Previous Button States (for edge detection) ───────────────
bool prevA = false, prevB = false, prevX = false, prevY = false;

// ── Helpers ───────────────────────────────────────────────────
int32_t mapJoystick(int32_t raw) {
  int32_t centered = raw - JOY_CENTER;
  if (abs(centered) < JOY_DEADZONE) return 0;
  // Map to BLE axis range
  if (centered > 0)
    return map(centered, JOY_DEADZONE, JOY_CENTER, 0, BLE_AXIS_MAX);
  else
    return map(centered, -JOY_CENTER, -JOY_DEADZONE, BLE_AXIS_MIN, 0);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 BLE Gamepad — 4 Buttons + 1 Joystick");

  // Button pins with internal pull-up (active LOW)
  pinMode(BTN_A_PIN, INPUT_PULLUP);
  pinMode(BTN_B_PIN, INPUT_PULLUP);
  pinMode(BTN_X_PIN, INPUT_PULLUP);
  pinMode(BTN_Y_PIN, INPUT_PULLUP);

  // Configure BLE Gamepad
  config.setAutoReport(false);       // Manual report for efficiency
  config.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  config.setButtonCount(4);
  config.setIncludeXAxis(true);
  config.setIncludeYAxis(true);
  config.setAxesMin(BLE_AXIS_MIN);
  config.setAxesMax(BLE_AXIS_MAX);

  bleGamepad.begin(&config);
  Serial.println("BLE advertising started. Connect your device.");
}

void loop() {
  if (!bleGamepad.isConnected()) {
    delay(100);
    return;
  }

  // ── Read Buttons (active LOW) ──
  bool curA = !digitalRead(BTN_A_PIN);
  bool curB = !digitalRead(BTN_B_PIN);
  bool curX = !digitalRead(BTN_X_PIN);
  bool curY = !digitalRead(BTN_Y_PIN);

  if (curA != prevA) { curA ? bleGamepad.press(BUTTON_1) : bleGamepad.release(BUTTON_1); prevA = curA; }
  if (curB != prevB) { curB ? bleGamepad.press(BUTTON_2) : bleGamepad.release(BUTTON_2); prevB = curB; }
  if (curX != prevX) { curX ? bleGamepad.press(BUTTON_3) : bleGamepad.release(BUTTON_3); prevX = curX; }
  if (curY != prevY) { curY ? bleGamepad.press(BUTTON_4) : bleGamepad.release(BUTTON_4); prevY = curY; }

  // ── Read Joystick ──
  int rawX = analogRead(JOY_X_PIN);
  int rawY = analogRead(JOY_Y_PIN);
  int16_t axisX = (int16_t)mapJoystick(rawX);
  int16_t axisY = (int16_t)mapJoystick(rawY);

  bleGamepad.setX(axisX);
  bleGamepad.setY(axisY);

  bleGamepad.sendReport();

  delay(10);  // ~100 Hz report rate
}
