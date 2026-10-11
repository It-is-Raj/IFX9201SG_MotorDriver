# Manual — LazyDragon IFX9201SG Motor Driver

How the IFX9201SG breakout drives a DC motor, what each pin does, and how to
integrate it into a control loop. For the hardware overview see
[`README.md`](README.md); for troubleshooting see [`DEBUG.md`](DEBUG.md).

> **Status:** the driver ships as `code/ifx9201sg_motor_driver.ino` — a small,
> modular PWM/DIR library plus a ready-to-flash demo. The API below matches it.

---

## Overview

The IFX9201SG is a general-purpose 6 A H-bridge. Four integrated n-channel
MOSFETs, their gate drive, and a charge pump switch a DC motor between forward,
reverse, and freewheeling states. Direction comes from `DIR`; speed comes from
the duty cycle on `PWM`. The breakout wires the IC into a usable module: a
motor-supply input, a motor output pair, a 4-wire logic header, and the bulk
capacitance needed to survive motor switching.

---

## How it works

1. `VS` (routed to `VCC`) powers the bridge and is the motor supply.
2. `DIR` selects the current direction through the motor.
3. `PWM` chops the high-side switch to set the average motor voltage.
4. With `PWM` low, the bridge freewheels through the appropriate high-side
   switch, recirculating inductive motor current.

The bridge is controlled purely by `PWM` and `DIR` on this breakout. The SPI
diagnosis interface (`SO`, `SI`, `SCK`, `CSN`) is not exposed; on the board
`SI`, `CSN`, `SCK`, and `DIS` are tied to `GND` and `SO` is left open, so the
device runs in `PWM`/`DIR` mode.

Built-in behaviour worth knowing:

- **Current limitation** — above ~8 A the bridge enters chopper current
  limitation, switching the low-side device off to let the current decay.
- **Protection** — short-circuit shutdown, overtemperature shutdown, and `VS`
  undervoltage lockout, each with latch behaviour.
- **Sleep mode** — pulling `VSO` low puts the device into a low-current sleep
  state (the board ties `VSO` to `+5 V`, so sleep is only available if you
  modify the board).

---

## Configuration

Hardware parameters (fixed by the board):

| Parameter | Value | Meaning |
|-----------|-------|---------|
| `VS` / `VCC` | ≤ 36 V | Motor supply input. |
| `VSO` | +5 V (tied) | Logic/SO-buffer supply. |
| `PWM` | ≤ 20 kHz | Speed input frequency limit. |
| `DIR` | 0 / 1 | Direction select. |
| Continuous current | 6 A | Per-channel rating. |
| Bulk capacitance | 220 µF | Onboard `C2`. |

Firmware configuration (top of `code/ifx9201sg_motor_driver.ino`):

| Constant | Default | Meaning |
|----------|---------|---------|
| `PIN_DIR` | `4` | H1 `DIR` pin (any digital pin). |
| `PIN_PWM` | `5` | H1 `PWM` pin (must be PWM-capable). |
| `PWM_MAX` | `255` | `analogWrite` range (8-bit AVR). |

---

## Function / API reference

Self-contained functions (no external libraries). Call `motorBegin()` once in
`setup()`, then call the rest from anywhere in your sketch.

### `motorBegin()`
Configures `PIN_DIR`/`PIN_PWM` as outputs and starts in freewheel.

### `motorForward(uint8_t speed)`
Drives `DIR` high and applies `speed` (0..255) to `PWM`.

### `motorReverse(uint8_t speed)`
Drives `DIR` low and applies `speed` (0..255) to `PWM`.

### `motorDrive(int speed)`
Signed convenience call: `+255` = full forward, `0` = coast, `-255` = full
reverse. Values are clamped to `±PWM_MAX`.

### `motorCoast()`
Freewheel stop — sets `PWM` to 0 and lets the motor spin down.

### `motorBrake()`
Stops the motor. The IFX9201SG in PWM/DIR mode has **no active short-brake**, so
this is a freewheel stop (same as `motorCoast()`); it is kept for a familiar API.

---

## Output / data format

This is an **actuator** board: it has no data output and emits no telemetry.
Control is one-directional (`PWM` + `DIR` in; motor current out). The bundled
firmware drives the motor only and exposes no telemetry.

---

## Integration guide

- **Keep PWM ≤ 20 kHz.** Higher frequencies increase switching losses; lower
  frequencies can be audible.
- **Never exceed `VS` = 36 V**, and keep the supply within the board's input
  capacitor rating.
- **Mind the thermal path.** The IFX9201SG relies on the exposed pad; continuous
  high current needs a heatsink or airflow. Junction temperature range is
  -40..125 °C.
- **Motor current transients.** The `220 µF` bulk capacitor absorbs inductive
  transients; add supply-side decoupling if the motor leads are long.
- **Freewheeling, not hard braking.** With `PWM` low the bridge freewheels; do
  not expect dynamic braking from the stock board without firmware support.
- **Do not drive `PWM` before power is stable.** Apply `VS` first, then begin
  switching.

---

## Examples

The bundled `code/ifx9201sg_motor_driver.ino` is a complete, flashable demo. To
reuse the driver, copy the **CONFIG** + **DRIVER FUNCTIONS** sections into your
sketch and call `motorBegin()` in `setup()`:

```cpp
void setup() {
  motorBegin();
}

void loop() {
  motorForward(180);   // forward at ~70% duty
  delay(1000);
  motorDrive(-150);    // signed call: reverse
  delay(1000);
  motorBrake();        // stop (freewheel)
  delay(1000);
}
```
