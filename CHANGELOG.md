# Changelog

All notable changes to the LazyDragon IFX9201SG motor driver are documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

### Added
- `code/ifx9201sg_motor_driver.ino`: self-contained PWM/DIR driver for the
  breakout (`motorBegin`, `motorForward`, `motorReverse`, `motorDrive`,
  `motorCoast`, `motorBrake`) plus a ready-to-flash demo.

## [1.0.0] - 2025-09-23

### Added
- Hardware: IFX9201SG 6 A H-bridge motor driver breakout (PWM/DIR control) with
  `VCC`/`GND`/`OUT_A`/`OUT_B` pads and a 4-pin logic header.
- Documentation: `README.md`, `MANUAL.md`, `DEBUG.md`.
- Licensing: hardware under CC BY-NC-SA 4.0 (`LICENSE`), firmware under
  PolyForm Noncommercial 1.0.0 (`LICENSE-CODE`).
- `COMMERCIAL-LICENSE.md` for paid commercial/royalty licensing.
- README license badges, documentation index, and no-warranty notice.
- Repository structure aligned to the LazyDragon standard (`assets/`,
  `datasheets/`, `fabrication/`, `code/`; docs at root).
