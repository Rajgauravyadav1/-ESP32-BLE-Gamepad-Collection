/*
 * ESP32 BLE Gamepad - 8 Buttons + 1 Joystick
 * ============================================
 * Layout:
 *   - 8 Digital Buttons (A, B, X, Y, LB, RB, START, SELECT)
 *   - 1 Analog Joystick (X-axis, Y-axis) with click button
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
 *   JOY_X      → GPIO 34 (ADC)
 *   JOY_Y      → GPIO 35 (ADC)
 *   JOY_SW     → GPIO 15 (Joystick click, optional)
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
#define JOY_X_PIN      34
#define JOY_Y_PIN      35
#define JOY_SW_PIN     15   // Joystick click (set to -1 to disable)

// ── Joystick Calibration ─────────────────────────────────────
#define JOY_DEADZONE   200
#define JOY_CENTER     2048
#define BLE_AXIS_MIN  -32767
#define BLE_AXIS_MAX   32767

// ── BLE Gamepad Instance ──────────────────────────────────────
BleGamepad bleGamepad("ESP32 Gamepad 8B+J", "ESP32-DIY", 100);
BleGamepadConfiguration config;

// ── Button State Tracking ─────────────────────────────────────
const int BTN_COUNT = 9;  // 8 face/shoulder + 1 joystick click
const int btnPins[BTN_COUNT] = {
  BTN_A_PIN, BTN_B_PIN, BTN_X_PIN, BTN_Y_PIN,
  BTN_LB_PIN, BTN_RB_PIN, BTN_START_PIN, BTN_SEL_PIN, JOY_SW_PIN
};
bool prevStates[BTN_COUNT] = {false};

int32_t mapJoystick(int32_t raw) {
  int32_t centered = raw - JOY_CENTER;
  if (abs(centered) < JOY_DEADZONE) return 0;
  if (centered > 0)
    return map(centered, JOY_DEADZONE, JOY_CENTER, 0, BLE_AXIS_MAX);
  else
    return map(centered, -JOY_CENTER, -JOY_DEADZONE, BLE_AXIS_MIN, 0);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 BLE Gamepad — 8 Buttons + 1 Joystick");

  for (int i = 0; i < BTN_COUNT; i++) {
    if (btnPins[i] >= 0) pinMode(btnPins[i], INPUT_PULLUP);
  }

  config.setAutoReport(false);
  config.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  config.setButtonCount(BTN_COUNT);
  config.setIncludeXAxis(true);
  config.setIncludeYAxis(true);
  config.setAxesMin(BLE_AXIS_MIN);
  config.setAxesMax(BLE_AXIS_MAX);

  bleGamepad.begin(&config);
  Serial.println("BLE advertising started.");
}

void loop() {
  if (!bleGamepad.isConnected()) {
    delay(100);
    return;
  }

  // ── Read & Report Buttons ──
  for (int i = 0; i < BTN_COUNT; i++) {
    if (btnPins[i] < 0) continue;
    bool cur = !digitalRead(btnPins[i]);
    if (cur != prevStates[i]) {
      cur ? bleGamepad.press(i + 1) : bleGamepad.release(i + 1);
      prevStates[i] = cur;
    }
  }

  // ── Read & Report Joystick ──
  int16_t axisX = (int16_t)mapJoystick(analogRead(JOY_X_PIN));
  int16_t axisY = (int16_t)mapJoystick(analogRead(JOY_Y_PIN));
  bleGamepad.setX(axisX);
  bleGamepad.setY(axisY);

  bleGamepad.sendReport();
  delay(10);
}
