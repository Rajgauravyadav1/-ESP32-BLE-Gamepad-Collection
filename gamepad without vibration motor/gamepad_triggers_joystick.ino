/*
 * ESP32 BLE Gamepad - 2 Analog Triggers + 1 Joystick
 * =====================================================
 * Layout:
 *   - 2 Analog Trigger buttons (LT, RT) → mapped to BLE Z / RZ axes
 *   - 1 Analog Joystick (X, Y axes)
 *   - 4 Face Buttons (A, B, X, Y) — bonus digital buttons
 *
 * Why analog triggers?
 *   Triggers are read via ADC so they report pressure 0–100% (like Xbox/PS triggers).
 *   If you use simple push buttons, wire them with a voltage divider or use
 *   the digital fallback mode defined below.
 *
 * Wiring:
 *   LT (Analog)  → GPIO 32 (ADC)     [or use a potentiometer / hall sensor]
 *   RT (Analog)  → GPIO 33 (ADC)
 *   JOY_X        → GPIO 34 (ADC)
 *   JOY_Y        → GPIO 35 (ADC)
 *   BTN_A        → GPIO 12
 *   BTN_B        → GPIO 13
 *   BTN_X        → GPIO 14
 *   BTN_Y        → GPIO 27
 *
 * ── Digital Trigger Mode ─────────────────────────────────────
 * Set DIGITAL_TRIGGERS true to use simple push-buttons for LT/RT
 * instead of analog potentiometers. They will report 0 or MAX.
 *
 * Library: BleGamepad by lemmingDev
 */

#include <BleGamepad.h>

// ── Mode Toggle ───────────────────────────────────────────────
#define DIGITAL_TRIGGERS  false   // true = digital buttons, false = analog ADC

// ── Pin Definitions ──────────────────────────────────────────
#define LT_PIN         32   // Left Trigger  (ADC or digital)
#define RT_PIN         33   // Right Trigger (ADC or digital)
#define JOY_X_PIN      34
#define JOY_Y_PIN      35
#define BTN_A_PIN      12
#define BTN_B_PIN      13
#define BTN_X_PIN      14
#define BTN_Y_PIN      27

// ── Calibration ───────────────────────────────────────────────
#define JOY_DEADZONE    200
#define JOY_CENTER      2048
#define BLE_AXIS_MIN   -32767
#define BLE_AXIS_MAX    32767

// Trigger: ADC 0–4095 mapped to BLE 0–32767
#define TRIGGER_ADC_MIN   0
#define TRIGGER_ADC_MAX   4095
#define TRIGGER_BLE_MIN   0
#define TRIGGER_BLE_MAX   32767

// ── BLE Gamepad ───────────────────────────────────────────────
BleGamepad bleGamepad("ESP32 Gamepad TRG+J", "ESP32-DIY", 100);
BleGamepadConfiguration config;

// ── Button State Tracking ─────────────────────────────────────
const int FACE_COUNT = 4;
const int facePins[FACE_COUNT] = { BTN_A_PIN, BTN_B_PIN, BTN_X_PIN, BTN_Y_PIN };
const char* faceNames[FACE_COUNT] = { "A", "B", "X", "Y" };
bool prevFace[FACE_COUNT] = {false};
bool prevLT = false, prevRT = false;

// ── Helpers ───────────────────────────────────────────────────
int32_t mapJoystick(int32_t raw) {
  int32_t c = raw - JOY_CENTER;
  if (abs(c) < JOY_DEADZONE) return 0;
  return (c > 0)
    ? map(c, JOY_DEADZONE, JOY_CENTER, 0, BLE_AXIS_MAX)
    : map(c, -JOY_CENTER, -JOY_DEADZONE, BLE_AXIS_MIN, 0);
}

int32_t mapTrigger(int32_t raw) {
  // Clamp and map ADC reading to BLE trigger range
  raw = constrain(raw, TRIGGER_ADC_MIN, TRIGGER_ADC_MAX);
  return map(raw, TRIGGER_ADC_MIN, TRIGGER_ADC_MAX, TRIGGER_BLE_MIN, TRIGGER_BLE_MAX);
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 BLE Gamepad — 2 Triggers + 1 Joystick");

  // Face buttons
  for (int i = 0; i < FACE_COUNT; i++) pinMode(facePins[i], INPUT_PULLUP);

  // Trigger pins
#if DIGITAL_TRIGGERS
  pinMode(LT_PIN, INPUT_PULLUP);
  pinMode(RT_PIN, INPUT_PULLUP);
  Serial.println("Trigger mode: DIGITAL");
#else
  // ADC pins don't need pinMode
  Serial.println("Trigger mode: ANALOG");
#endif

  // BLE config
  config.setAutoReport(false);
  config.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  config.setButtonCount(FACE_COUNT);

  // Joystick axes
  config.setIncludeXAxis(true);
  config.setIncludeYAxis(true);
  config.setAxesMin(BLE_AXIS_MIN);
  config.setAxesMax(BLE_AXIS_MAX);

  // Trigger axes (Z = LT, RZ = RT, matching Xbox/generic mapping)
  config.setIncludeZAxis(true);
  config.setIncludeRzAxis(true);

  bleGamepad.begin(&config);
  Serial.println("BLE advertising started.");
}

void loop() {
  if (!bleGamepad.isConnected()) {
    delay(100);
    return;
  }

  // ── Face Buttons ──
  for (int i = 0; i < FACE_COUNT; i++) {
    bool cur = !digitalRead(facePins[i]);
    if (cur != prevFace[i]) {
      cur ? bleGamepad.press(i + 1) : bleGamepad.release(i + 1);
      if (cur) Serial.printf("Button %s pressed\n", faceNames[i]);
      prevFace[i] = cur;
    }
  }

  // ── Triggers ──
#if DIGITAL_TRIGGERS
  // Digital: 0 or full
  int16_t ltVal = !digitalRead(LT_PIN) ? TRIGGER_BLE_MAX : 0;
  int16_t rtVal = !digitalRead(RT_PIN) ? TRIGGER_BLE_MAX : 0;
#else
  // Analog: smooth pressure
  int16_t ltVal = (int16_t)mapTrigger(analogRead(LT_PIN));
  int16_t rtVal = (int16_t)mapTrigger(analogRead(RT_PIN));
#endif

  bleGamepad.setZ(ltVal);    // Left Trigger  → Z axis
  bleGamepad.setRZ(rtVal);   // Right Trigger → RZ axis

  // ── Joystick ──
  bleGamepad.setX((int16_t)mapJoystick(analogRead(JOY_X_PIN)));
  bleGamepad.setY((int16_t)mapJoystick(analogRead(JOY_Y_PIN)));

  bleGamepad.sendReport();
  delay(10);
}
