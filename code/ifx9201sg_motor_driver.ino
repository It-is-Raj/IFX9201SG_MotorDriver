/*
  LazyDragon IFX9201SG motor driver breakout
  Simple, modular PWM + DIR driver for an Arduino Nano (or any Arduino).

  The IFX9201SG is a 6 A H-bridge. On this breakout it is wired for PWM/DIR
  control and the DIS enable is tied to GND, so the bridge is always enabled:
    DIR : direction      (HIGH = forward, LOW = reverse)
    PWM : speed / duty   (analogWrite 0..255); PWM = 0 lets the motor freewheel

  IFX9201SG output truth table (PWM/DIR mode):
    DIR = 1, PWM > 0  ->  forward
    DIR = 0, PWM > 0  ->  reverse
    PWM = 0           ->  freewheel / coast
  There is no active short-brake in this mode, so brake() stops by freewheeling.

  --------------------------------------------------------------------------
  HOW TO REUSE
    Copy the CONFIG and DRIVER FUNCTIONS below into your own sketch, call
    motorBegin() in setup(), then use the motor*() calls in your loop.
    Delete the EXAMPLE block at the bottom.
  --------------------------------------------------------------------------
*/

// ------------------------------- CONFIG ------------------------------------
#define PIN_DIR   4        // H1 "DIR"  -> any digital pin
#define PIN_PWM   5        // H1 "PWM"  -> any PWM-capable pin (3,5,6,9,10,11)
#define PWM_MAX   255      // analogWrite range on AVR (8-bit)

// --------------------------- DRIVER FUNCTIONS ------------------------------

// Freewheel / coast: stop driving and let the motor spin down (PWM = 0).
void motorCoast() {
  analogWrite(PIN_PWM, 0);
}

// Stop the motor. The IFX9201SG in PWM/DIR mode has no active short-brake, so
// this behaves like motorCoast() (freewheel). Kept for a familiar API.
void motorBrake() {
  motorCoast();
}

// Spin forward at the given duty (0..255).
void motorForward(uint8_t speed) {
  digitalWrite(PIN_DIR, HIGH);
  analogWrite(PIN_PWM, speed);
}

// Spin reverse at the given duty (0..255).
void motorReverse(uint8_t speed) {
  digitalWrite(PIN_DIR, LOW);
  analogWrite(PIN_PWM, speed);
}

// Signed drive, like a typical motor library: +255 = full forward, 0 = coast,
// -255 = full reverse.
void motorDrive(int speed) {
  if (speed >  PWM_MAX) speed =  PWM_MAX;
  if (speed < -PWM_MAX) speed = -PWM_MAX;

  if (speed > 0)      motorForward((uint8_t) speed);
  else if (speed < 0) motorReverse((uint8_t)(-speed));
  else                motorCoast();
}

// Configure the driver pins. Call once from setup().
void motorBegin() {
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_PWM, OUTPUT);
  motorCoast();
}

// ------------------------------- EXAMPLE -----------------------------------
// Ready-to-flash demo. Remove this block to paste the driver into your code.

void setup() {
  motorBegin();
}

void loop() {
  motorForward(180);  delay(1500);   // forward
  motorCoast();       delay(800);    // let it roll
  motorReverse(180);  delay(1500);   // reverse
  motorBrake();       delay(800);    // stop (freewheel)
  motorDrive(0);      delay(800);    // signed API, 0 = stop
}
