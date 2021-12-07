#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"

#define STEP_MS             (250)
#define SERIAL_SPEED        (9600)
#define PHOTOSENSOR_ERROR   (30)

#define LOG_ALWAYS(s)       Serial.print("[ALWAYS]: " + String(s) + "\n\r");
#define LOG_ERROR(s)        Serial.print("[ERROR]: " + String(s) + "\n\r");

#define DEBUG

#ifdef DEBUG
    #define LOG_DEBUG(s)    Serial.print("[DEBUG]: " + String(s) + "\n\r");
#else
    #define LOG_DEBUG(s)
#endif

#define UNUSED(x) (void)(x)

const int
/* photo resistor analog pins */
SENSOR_ORANGE_PIN = A5, SENSOR_GREEN_PIN  = A4, SENSOR_BLUE_PIN   = A3, SENSOR_WHITE_PIN  = A2,
/* servos pwm pins */
SERVO_AZIMUTH_PIN = 10, SERVO_ELEVATION_PIN = 11,
/* leds pins */
LED_ELEVATION_PIN = 12, LED_AZIMUTH_PIN = 13,
/* button pin */
BUTTTON_PIN = 2,
/*default servos angle in degree */
DEFAULT_SERVOS_ANGLE = 90,
/* servo angle interval (can be negative to move to different direction) */
SERVO_ANGLE_INTERVAL = 1,
/* servo max angle */
SERVO_MAX_ANGLE = 180,
/* servo min angle */
SERVO_MIN_ANGLE = 0;

typedef enum {
  SERVO_AZIMUTH_STOP,
  SERVO_AZIMUTH_MOVE_RIGHT,
  SERVO_AZIMUTH_MOVE_LEFT,
} servo_azimuth_state_t;

typedef enum {
  SERVO_ELEVATION_STOP,
  SERVO_ELEVATION_MOVE_UP,
  SERVO_ELEVATION_MOVE_DOWN,
} servo_elevation_state_t;

typedef enum {
  PHOTOSENSOR_GREEN = 0,
  PHOTOSENSOR_ORANGE = 1,
  PHOTOSENSOR_WHITE = 2,
  PHOTOSENSOR_BLUE = 3,

  PHOTOSENSOR_LAST
} photosensor_t;

typedef enum {
  INIT_STATE = 0,
  ELEVATION_CONTROL_STATE = 1,
  AZIMUTH_CONTROL_STATE = 2,
  STOP_STATE = 3,

  LAST_STATE
} solar_tracker_state_t;

typedef struct {
  /* solar tracker state */
  solar_tracker_state_t state;
  /* photoresistors values values */
  int photosensors[PHOTOSENSOR_LAST];
  /* start/stop button state */
  bool button_pressed, button_pressed_last;
  /* stop and init leds last state */
  int stop_leds_state, init_leds_state;
  /* azimuth servo state */
  servo_azimuth_state_t azimuth_servo_state;
  /* elevation servo state */
  servo_elevation_state_t elevation_servo_state;
} solar_tracker_t;

#endif /* COMMON_H */