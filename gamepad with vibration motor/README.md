# 🎮 [gamepad with vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor)

> Interactive HTML reference with wiring diagrams, pin tables, and ready-to-flash Arduino code for every gamepad layout and vibration motor driver.

Open `esp32_gamepad_all_circuits.html` in any browser — no internet required, no dependencies, fully self-contained.

---

## 🚀 How to Open the HTML File

No installation needed. Just follow the steps for your operating system.

### Windows

1. Download or clone this repository
2. Open the folder in File Explorer
3. Double-click `esp32_gamepad_all_circuits.html`
4. It opens automatically in your default browser (Chrome, Edge, Firefox)

> If nothing opens, right-click the file → **Open with** → choose your browser

### macOS

1. Download or clone this repository
2. Open Finder and navigate to the folder
3. Double-click `esp32_gamepad_all_circuits.html`
4. It opens in Safari, Chrome, or Firefox

> If it asks which app to use, select your browser from the list

### Linux

1. Download or clone this repository
2. Navigate to the folder in your file manager
3. Double-click `esp32_gamepad_all_circuits.html`

Or open from terminal:
```bash
xdg-open esp32_gamepad_all_circuits.html   # Ubuntu / Debian
firefox esp32_gamepad_all_circuits.html    # if using Firefox directly
google-chrome esp32_gamepad_all_circuits.html  # if using Chrome directly
```

### Mobile (Android / iOS)

1. Copy the file to your phone (USB, Google Drive, AirDrop, etc.)
2. Open your **Files** app and locate the file
3. Tap it — it will open in your mobile browser

> On Android, use **Chrome** or **Firefox**. On iOS, use **Safari**.

### From GitHub (no download needed)

If the file is hosted on GitHub Pages, just open the link in your browser directly — no downloading required.

To enable GitHub Pages for your own fork:
1. Go to your repo → **Settings** → **Pages**
2. Set source to `main` branch, `/ (root)` folder
3. Click **Save** — GitHub gives you a public URL in seconds

---

## 📑 Contents

The file has **7 tabs**, split into two sections:

### 🕹️ Gamepad layouts (tabs 1–3)

| Tab | Buttons | Axes | Trigger | Best for |
|---|---|---|---|---|
| **4 Btn + Joystick** | 4 (A/B/X/Y) | X, Y | — | Mini arcade stick, simple RC |
| **8 Btn + Joystick** | 8 + joy click | X, Y | — | Full gamepad clone |
| **8 Btn + Joystick + Triggers** | 8 | X, Y, Z (LT), RZ (RT) | ✅ analog switch | Racing / FPS controller |

### ⚙️ Vibration motor drivers (tabs 4–7)

| Tab | Driver IC | Channels | Supply | GPIO per motor |
|---|---|---|---|---|
| **Vibration Module** | S8050 NPN | 1 per module | 3.3V or 5V | 1 |
| **TB6612FNG** | Toshiba H-bridge | 2 | 5V motor + 3.3V logic | 3 + STBY |
| **MX1508** | Mixic H-bridge | 2 | 3.3–5V | 1 (IN2 → GND) |
| **DRV8833** | TI H-bridge | 2 | 3.3V only | 2 + nFAULT |

---

## 🔌 GPIO Quick Reference

### Gamepad — 4 Buttons + Joystick

```
GPIO 12  →  BTN A        (INPUT_PULLUP → GND)
GPIO 13  →  BTN B        (INPUT_PULLUP → GND)
GPIO 14  →  BTN X        (INPUT_PULLUP → GND)
GPIO 27  →  BTN Y        (INPUT_PULLUP → GND)
GPIO 34  →  KY-023 VRX   (analog, input-only)
GPIO 35  →  KY-023 VRY   (analog, input-only)
3.3V     →  KY-023 VCC
GND      →  all GND pins
```

### Gamepad — 8 Buttons + Joystick

```
GPIO 12  →  BTN A        (INPUT_PULLUP → GND)
GPIO 13  →  BTN B        (INPUT_PULLUP → GND)
GPIO 14  →  BTN X        (INPUT_PULLUP → GND)
GPIO 27  →  BTN Y        (INPUT_PULLUP → GND)
GPIO 26  →  BTN LB       (INPUT_PULLUP → GND)
GPIO 25  →  BTN RB       (INPUT_PULLUP → GND)
GPIO 33  →  START        (INPUT_PULLUP → GND)
GPIO 32  →  SELECT       (INPUT_PULLUP → GND)
GPIO 15  →  Joy SW click (INPUT_PULLUP → GND)
GPIO 34  →  KY-023 VRX   (analog, input-only)
GPIO 35  →  KY-023 VRY   (analog, input-only)
3.3V     →  KY-023 VCC
GND      →  all GND pins
```

### Gamepad — 8 Buttons + Joystick + Trigger Switches

```
GPIO 12/13/14/27  →  A / B / X / Y    (INPUT_PULLUP → GND)
GPIO 26 / 25      →  LB / RB          (INPUT_PULLUP → GND)
GPIO 33 / 32      →  START / SELECT   (INPUT_PULLUP → GND)
GPIO 36           →  LT trigger       (analog, input-only → Z axis)
GPIO 39           →  RT trigger       (analog, input-only → RZ axis)
GPIO 34           →  KY-023 VRX       (analog, input-only)
GPIO 35           →  KY-023 VRY       (analog, input-only)
3.3V              →  KY-023 VCC
GND               →  all GND pins
```

> **Note:** GPIO 34, 35, 36, 39 are input-only pins on ESP32 — no internal pull-up, do not drive them as output.

---

## ⚡ Vibration Motor GPIO Reference

### Vibration Module (S8050)

```
GPIO 25  →  IN (Motor 1)   PWM via ledcWrite
GPIO 26  →  IN (Motor 2)   PWM via ledcWrite
3.3V     →  VCC (both modules)
GND      →  GND (both modules)
```

### TB6612FNG

```
GPIO 25  →  PWMA    (Motor 1 speed — LEDC)
GPIO 26  →  AIN1    (Motor 1 direction)
GPIO 27  →  AIN2    (Motor 1 direction)
GPIO 32  →  PWMB    (Motor 2 speed — LEDC)
GPIO 33  →  BIN1    (Motor 2 direction)
GPIO 14  →  BIN2    (Motor 2 direction)
3.3V     →  STBY + VCC
5V       →  VM      (motor power)
GND      →  GND
```

### MX1508

```
GPIO 25  →  IN1-A   (Motor 1 PWM)
GND      →  IN2-A   (fixed LOW = forward direction)
GPIO 26  →  IN1-B   (Motor 2 PWM)
GND      →  IN2-B   (fixed LOW = forward direction)
3.3–5V   →  VCC
GND      →  GND
```

### DRV8833

```
GPIO 25  →  AIN1    (Motor 1 PWM — LEDC)
GPIO 26  →  AIN2    (Motor 1 dir — set LOW)
GPIO 27  →  BIN1    (Motor 2 PWM — LEDC)
GPIO 14  →  BIN2    (Motor 2 dir — set LOW)
3.3V     →  nSLEEP  (must be HIGH to enable)
GPIO 35  ←  nFAULT  (LOW = overcurrent / thermal fault)
3.3V     →  VM + VCC (both on 3.3V — no 5V needed)
GND      →  GND
```

---

## 📦 Required Libraries

Install both via **Sketch → Include Library → Manage Libraries** in Arduino IDE.

| Library | Author | Install name |
|---|---|---|
| BleGamepad | lemmingDev | `BleGamepad` |
| ESP32 board package | Espressif | add board manager URL below |

**Board Manager URL:**
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

**Arduino IDE settings:**
- Board: `ESP32 Dev Module`
- Upload speed: `921600`
- Partition scheme: `Default 4MB`

---

## ⚙️ PWM Settings Reference

All motor driver code uses `ledcSetup()` / `ledcWrite()` (ESP32 Arduino core ≤ 2.x).

> For ESP32 Arduino core **3.x**, replace `ledcSetup` + `ledcAttachPin` with `ledcAttachChannel`.

| Driver | Recommended frequency | Resolution | Min duty to spin |
|---|---|---|---|
| Vibration Module | 5 kHz | 8-bit | ~80 / 255 (31%) |
| TB6612FNG | 10 kHz | 8-bit | ~80 / 255 (31%) |
| MX1508 | 5 kHz | 8-bit | ~100 / 255 (39%) |
| DRV8833 | 20 kHz (inaudible) | 8-bit | ~70 / 255 (27%) |

---

## 🕹️ BLE Axis Mapping

| Physical input | BLE axis | Range |
|---|---|---|
| Joystick X | X-Axis | −32767 → +32767 |
| Joystick Y | Y-Axis | −32767 → +32767 |
| Left Trigger (LT) | Z-Axis | 0 → +32767 |
| Right Trigger (RT) | RZ-Axis | 0 → +32767 |

Joystick deadzone is set to ±200 ADC counts (out of 0–4095). Adjust `#define DEADZONE` in code to suit your module.

---

## 🔋 Battery Safety

> ⚠️ **Never connect an 18650 cell directly to ESP32 VIN and assume it will charge safely over USB.**

Standard ESP32 dev boards have no charging circuitry. Use one of:

- **TP4056 module** — dedicated CC/CV charger, ~₹20, wires between USB and battery
- **Purpose-built board** — TTGO T7, Adafruit HUZZAH32, or similar with onboard LiPo charging
- **USB power bank** — simplest option for a gamepad that only needs to run

See `README.md` in the main repository for the full battery safety section.

---

## 🐛 Common Issues

| Problem | Likely cause | Fix |
|---|---|---|
| BLE device not visible | Sketch not running | Check Serial Monitor for "BLE advertising started" |
| Buttons not registering | Wrong pin or wiring | Confirm button → GPIO on one side, GND on other |
| Joystick always drifting | Deadzone too small | Increase `DEADZONE` constant |
| Motor not spinning | Duty too low | Increase minimum duty; DRV8833 needs ≥ 70/255 |
| nFAULT triggered | Motor stall or short | Check motor wiring; reduce duty cycle |
| TB6612FNG no output | STBY not HIGH | Wire STBY to 3.3V or set GPIO HIGH in code |
| Upload fails | Wrong COM port or driver | Install CH340/CP2102 driver for your board |

---

## 📄 License

MIT — free to use, modify, and distribute for personal and commercial projects.
