# 🎮 ESP32 BLE Gamepad Collection

A collection of ready-to-flash Arduino sketches for building your own **Bluetooth Low Energy (BLE) gamepad** using an ESP32. Each sketch is a standalone configuration — pick the one that matches your hardware build.

This repository contains two sub-projects:

| Sub-project | Description |
|---|---|
| [gamepad without vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor) | Gamepad layouts only — buttons, joystick, analog triggers |
| [gamepad with vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor) | All gamepad layouts + rumble motor driver circuits and code |

---

## 📦 Sketches

### [gamepad without vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor)

| Folder | Buttons | Axes | Best For |
|---|---|---|---|
| `gamepad_4btn_1joystick` | 4 (A/B/X/Y) | X, Y | Mini arcade stick, simple RC |
| `gamepad_8btn_joystick` | 8 + joystick click | X, Y | Full gamepad clone |
| `gamepad_8btn_only` | 8 (no stick) | None | Media remote, quiz buzzer |
| `gamepad_triggers_joystick` | 4 face + 2 analog triggers | X, Y, Z(LT), RZ(RT) | Racing / FPS controller |

### [gamepad with vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor)

All four gamepad layouts above, plus rumble support using one of four motor driver options:

| Driver | Channels | Supply | GPIO per motor | Best For |
|---|---|---|---|---|
| Vibration Module (S8050) | 1 per module | 3.3V or 5V | 1 | Simplest build |
| TB6612FNG | 2 | 5V motor + 3.3V logic | 3 + STBY | Full control + brake |
| MX1508 | 2 | 3.3–5V | 1 (IN2 → GND) | Cheapest option |
| DRV8833 | 2 | 3.3V only | 2 + nFAULT | Best for 3.3V-only builds |

---

## 🔧 Requirements

### Hardware

**All builds:**
- ESP32 development board (any variant: DevKit v1, WROOM, WROVER, etc.)
- Momentary push buttons
- Analog joystick module(s) (KY-023 or similar)
- Potentiometers or hall sensors for analog triggers (optional)
- Breadboard + jumper wires

**Additional for gamepad with vibration motor:**
- 2× coin vibration motors (3V phone type)
- Motor driver — choose one:
  - Vibration Module (S8050 NPN, black PCB) — simplest
  - TB6612FNG (Toshiba dual H-bridge, red PCB)
  - MX1508 (Mixic dual H-bridge, red PCB) — cheapest
  - DRV8833 (Texas Instruments, blue/black PCB) — best for 3.3V

### Software
- [Arduino IDE](https://www.arduino.cc/en/software) **2.x** recommended
- **ESP32 Board Package** by Espressif (add via Board Manager)
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- **BleGamepad** library by lemmingDev  
  → Install via **Sketch → Include Library → Manage Libraries** → search `BleGamepad`

---

## 🚀 Quick Start

1. Install all dependencies above
2. Choose your sub-project:
   - **No rumble** → open the `.ino` file from [gamepad without vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor)
   - **With rumble** → open the `.ino` file from [gamepad with vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor)
3. Check the **wiring table** at the top of the file and connect your components
4. Select board: `Tools → Board → ESP32 Arduino → ESP32 Dev Module`
5. Select the correct COM port
6. Click **Upload**
7. On your phone/PC, pair with the device name shown in the sketch (e.g. `ESP32 Gamepad 4B+J`)

---

## 📌 Wiring Summary

### Common to all sketches
| Signal | GPIO | Notes |
|---|---|---|
| All buttons | Various | Wired to GND; internal pull-up enabled |
| JOY_X | 34 | ADC1 channel, input-only pin |
| JOY_Y | 35 | ADC1 channel, input-only pin |

> ⚠️ **GPIO 34, 35, 36, 39** are input-only on ESP32 — do not use them for output.

### Trigger sketch (analog mode)
| Signal | GPIO | Notes |
|---|---|---|
| LT (Left Trigger) | 32 | 10kΩ potentiometer or hall sensor |
| RT (Right Trigger) | 33 | 10kΩ potentiometer or hall sensor |

Set `#define DIGITAL_TRIGGERS true` in the trigger sketch to use simple push buttons instead.

---

## ⚡ Vibration Motor Wiring Summary

> Applies to [gamepad with vibration motor](https://github.com/Rajgauravyadav1/-ESP32-BLE-Gamepad-Collection/tree/main/gamepad%20without%20vibration%20motor) only.

### Vibration Module (S8050)
| Signal | GPIO | Notes |
|---|---|---|
| Motor 1 IN | 25 | PWM via `ledcWrite` |
| Motor 2 IN | 26 | PWM via `ledcWrite` |
| VCC | 3.3V or 5V | module power |

### TB6612FNG
| Signal | GPIO | Notes |
|---|---|---|
| PWMA (M1 speed) | 25 | LEDC PWM |
| AIN1 / AIN2 | 26 / 27 | M1 direction |
| PWMB (M2 speed) | 32 | LEDC PWM |
| BIN1 / BIN2 | 33 / 14 | M2 direction |
| STBY | 3.3V | must be HIGH to enable |
| VM | 5V | motor power rail |

### MX1508
| Signal | GPIO | Notes |
|---|---|---|
| IN1-A (M1 PWM) | 25 | LEDC PWM |
| IN2-A | GND | fixed LOW = forward |
| IN1-B (M2 PWM) | 26 | LEDC PWM |
| IN2-B | GND | fixed LOW = forward |

### DRV8833
| Signal | GPIO | Notes |
|---|---|---|
| AIN1 (M1 PWM) | 25 | LEDC PWM |
| AIN2 (M1 dir) | 26 | set LOW in code |
| BIN1 (M2 PWM) | 27 | LEDC PWM |
| BIN2 (M2 dir) | 14 | set LOW in code |
| nSLEEP | 3.3V | must be HIGH to enable |
| nFAULT | 35 | LOW = fault (optional) |
| VM + VCC | 3.3V | no 5V rail needed |

---

## 🎯 BLE Axis Mapping

| Axis | BLE Name | Typical Use |
|---|---|---|
| X | X-Axis | Joystick horizontal |
| Y | Y-Axis | Joystick vertical |
| Z | Z-Axis | Left Trigger (LT) |
| RZ | RZ-Axis | Right Trigger (RT) |
| RX | RX-Axis | Right stick horizontal |
| RY | RY-Axis | Right stick vertical |

All axes range from **-32767 to +32767** (or 0 to 32767 for triggers).

---

## 🛠️ Customisation Tips

- **Change device name**: Edit the first argument in `BleGamepad bleGamepad("Name Here", ...)` 
- **Adjust deadzone**: Change `JOY_DEADZONE` (default 200 out of 4095 ADC range)
- **Add D-Pad / HAT switch**: Use `bleGamepad.setHat1(HAT_UP)` etc.
- **Add second joystick**: Enable `RX` and `RY` axes and read a second joystick module
- **Battery level**: Pass actual battery % to `BleGamepad("Name", "Manufacturer", batteryLevel)`

---

## 🐛 Troubleshooting

| Problem | Fix |
|---|---|
| Device doesn't appear in BLE scan | Check ESP32 is powered; serial monitor should print "BLE advertising started" |
| Buttons not registering | Verify `INPUT_PULLUP` and button wired to GND |
| Joystick always drifting | Increase `JOY_DEADZONE` or recalibrate `JOY_CENTER` |
| Compilation error on BleGamepad | Ensure correct ESP32 board package version (≥ 2.0.0) |
| Disconnects frequently | Keep the BLE gamepad device closer; reduce `delay()` if >20ms |
| Motor not spinning | Duty too low — minimum is ~80/255 for most drivers |
| TB6612FNG no output | STBY pin not HIGH — wire to 3.3V |
| nFAULT triggered on DRV8833 | Motor stall or short circuit — check wiring and reduce duty |
| MX1508 gets hot | Running at too high a duty or continuous stall — add cooling or reduce load |

---

## 🔋 Battery Powering — Safety Warning

> **⚠️ Read this before connecting any lithium battery to your ESP32.**

### The common mistake

Many builders connect an 18650 lithium-ion cell directly to the ESP32's `VIN` and `GND` pins, then assume the battery charges safely when USB is plugged in. This setup is **not safe** and is **not recommended**.

It may appear to work — the ESP32 powers on, the battery voltage may even rise slightly — but the board is **not a lithium battery charger**. It provides no charging control whatsoever.

---

### What lithium-ion cells actually require

18650 cells must be charged using a dedicated **CC/CV** (Constant Current / Constant Voltage) profile:

| Requirement | Why it matters |
|---|---|
| Constant Current (CC) phase | Controls inrush during low state-of-charge |
| Constant Voltage (CV) phase | Prevents overcharge above 4.2V |
| Accurate 4.2V cutoff | Even 4.3V causes accelerated degradation |
| Overcharge protection | Thermal runaway risk above safe voltage |
| Overdischarge protection | Cell damage and capacity loss below ~2.5V |
| Short-circuit protection | Uncontrolled discharge can cause fire |

Standard ESP32 dev boards include **none** of these.

---

### Risks of doing it wrong

Connecting a battery like this:

```
18650 cell ──► VIN / GND ──► ESP32
                    ▲
              USB also here
```

Can result in:

- **Overcharging** — no 4.2V cutoff means the cell exceeds safe voltage
- **Cell swelling** — lithium cells puff up when abused; a swollen cell is a fire risk
- **Thermal runaway** — in the worst case, an overcharged 18650 can vent, catch fire, or explode
- **Undervoltage damage** — no cutoff on discharge kills the cell permanently
- **ESP32 damage** — a failing cell can spike voltage and destroy the board

---

### Safe alternatives

#### Option 1 — Dedicated charging module (recommended for DIY)

Use a **TP4056** (or similar) module between the battery and ESP32:

```
USB ──► TP4056 charger module ──► 18650 cell ──► boost converter (if needed) ──► ESP32 VIN
```

The TP4056 provides CC/CV charging, 4.2V cutoff, and overcharge protection for under ₹20 / $0.25.

Pair it with a separate **DW01A** or **HY2112** protection board for overdischarge and short-circuit protection if your cell does not already have a built-in PCB.

#### Option 2 — Use a purpose-built ESP32 battery board

Several ESP32 boards include integrated LiPo charging out of the box:

| Board | Charger IC | Notes |
|---|---|---|
| TTGO T7 / T-Energy | TP4054 | 18650 holder onboard |
| Adafruit HUZZAH32 | MCP73831 | JST connector for LiPo |
| TTGO T-OI | AXP192 | Ultra-compact, 16340 cell |
| LilyGO T-Internet-POE | TP5000 | Also supports 26650 |

#### Option 3 — USB power bank

For a gamepad that only needs to run (not charge while operating), a USB power bank with a `5V` output is the simplest and safest option — no battery management needed on your side.

---

### Quick decision guide

```
Want to run ESP32 on battery?
│
├─ Short sessions / prototyping  →  USB power bank
│
├─ Permanent portable build
│   ├─ Have LiPo / 18650         →  TP4056 + protection board + ESP32
│   └─ Want all-in-one           →  TTGO T7, Adafruit HUZZAH32, etc.
│
└─ Never connect bare 18650 directly to a plain ESP32 dev board
```

---

## 📄 License

MIT License — free to use, modify, and distribute.

---
