/*
 * NOTES:
 * Leds for signalling working state of system.
 * Button for start and stop system.
 */

#include "common.h"
#include "state_machine.h"
#include "read_photoresistors.h"
#include "control_servomotors.h"

solar_tracker_t solar_tracker = {.state = INIT_STATE,
                                 .photosensors = {0, 0, 0, 0},
                                 .button_pressed = false,
                                 .button_pressed_last = false,
                                 .azimuth_done = false,
                                 .stop_leds_state = LOW, .init_leds_state = LOW,
                                 .azimuth_servo_state = SERVO_AZIMUTH_STOP, .elevation_servo_state = SERVO_ELEVATION_STOP
                                };

#define GetSolarTracker() (&solar_tracker)

void setup() {
  /* init serial com port */
  Serial.begin(SERIAL_SPEED);
  /* init button */
  pinMode(BUTTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTTON_PIN), button_interrupt, FALLING);	// Configure interrupt
  /* init photoresistors analog */
  pinMode(SENSOR_ORANGE_PIN, INPUT);
  pinMode(SENSOR_GREEN_PIN, INPUT);
  pinMode(SENSOR_ORANGE_PIN, INPUT);
  pinMode(SENSOR_WHITE_PIN, INPUT);
  /* init pwm for servos */
  pinMode(SERVO_AZIMUTH_PIN, OUTPUT);
  pinMode(SERVO_ELEVATION_PIN, OUTPUT);
  /* init leds for signaling state */
  pinMode(LED_AZIMUTH_PIN, OUTPUT);
  pinMode(LED_ELEVATION_PIN, OUTPUT);
}

void loop() {
  read_photoresistors(GetSolarTracker());
  state_machine_evolution(GetSolarTracker());
  control_servomotors(GetSolarTracker());
  delay(STEP_MS);
}

/* interrupt */

void button_interrupt(void) {
  LOG_DEBUG("Button pressed.");
  GetSolarTracker()->button_pressed = true;
}
