# Debug — LazyDragon IFX9201SG Motor Driver

What to expect from the IFX9201SG breakout and how to isolate common motor-drive
problems. For the hardware overview see [`README.md`](README.md); for the
control details see [`MANUAL.md`](MANUAL.md).

> **Status:** the driver ships as `code/ifx9201sg_motor_driver.ino`. Symptoms
> below assume that sketch; firmware-specific notes are called out where
> relevant.

---

## Expected behavior

**Boot / startup:** with `VS` applied, the board is idle and the motor is still
until `PWM` is driven. The logic side needs `+5 V` on `H1`.

**Normal operation:** `DIR` selects forward/reverse; increasing `PWM` duty
increases motor speed; `PWM` low lets the motor coast (freewheel).

**Edge cases:**
- **Duty 0:** motor coasts, no drive.
- **Overcurrent / overtemperature:** the bridge latches off; it stays off until
  power is cycled.
- **`VS` too low:** undervoltage lockout disables the bridge.

---

## Quick checks

- Confirm **`VS`** on `VCC` and **`+5 V`** on `H1` pin 4, with a common ground.
- Confirm `DIR` and `PWM` actually toggle between 0 V and their logic high.
- Confirm the motor is wired across `OUT_A` / `OUT_B` (not to ground).
- Confirm `PWM` frequency is ≤ 20 kHz.

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---------|--------------|-----|
| Motor never moves | `PWM` duty 0, `DIR` floating, or `VS` missing | Drive `PWM` > 0; check `DIR` and `VCC` |
| Motor spins only one way | `DIR` not toggling | Verify `DIR` at `H1` pin 3 |
| Motor runs then stops until power cycle | Overcurrent/overtemperature latch tripped | Reduce load/current; add cooling; power-cycle |
| Driver gets very hot | High continuous current, no heatsinking | Add heatsink/airflow; lower duty or current |
| MCU resets when motor starts | Supply sag from motor inrush | Strengthen supply; check the 220 µF bulk cap |
| Weak/erratic speed | `VS` too low or PWM frequency too high | Raise `VS`; keep PWM ≤ 20 kHz |
| Nothing at all, no heat | `DIS` not held low or `VS` below UVLO | `DIS` is tied to `GND` on-board; verify `VS` ≥ ~4.2 V |

---

## Known limitations / errata

- **No SPI diagnostics exposed.** The board ties `SI`, `CSN`, `SCK`, `DIS` to
  `GND` and leaves `SO` open, so fault detail is only observable through
  behaviour, not read out.
- **No dynamic braking out of the box.** `PWM` low freewheels; braking requires
  firmware support or a board change.
- **Sleep mode not accessible.** `VSO` is tied to `+5 V`, so the low-current
  sleep state cannot be entered without modifying the board.
- <!-- TODO: add any further errata discovered during bring-up. -->

---

## Support

Open an issue in the project repository, or contact
**contact@lazydragon.club**.
