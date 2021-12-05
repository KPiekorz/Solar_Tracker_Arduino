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
                                 .sensor_orange = 0, .sensor_green = 0, .sensor_blue = 0, .sensor_white = 0,
                                 .button_pressed = false,
                                 .button_pressed_last = false,
                                 .azimuth_done = false,
                                 .azimuth_angle = SERVO_AZIMUTH_DEFAULT_ANGLE, .elevation_angle = SERVO_ELEVATION_DEFAULT_ANGLE,
                                 .stop_leds_state = LOW, .init_leds_state = LOW,
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
