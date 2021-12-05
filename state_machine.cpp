#include "state_machine.h"
#include <math.h>

/* static helper function */

static photosensor_t get_more_illuminated_sensor(solar_tracker_t * solar_tracker, photosensor_t photsesor_a, photosensor_t photsesor_b) {

  if (solar_tracker->photosensors[photsesor_a] < solar_tracker->photosensors[photsesor_b]) {
    return photsesor_a;
  }

  return photsesor_b;
}

static bool is_equal_illuminated_sensors(solar_tracker_t * solar_tracker, photosensor_t photsesor_a, photosensor_t photsesor_b) {

  if (abs(solar_tracker->photosensors[photsesor_a] - solar_tracker->photosensors[photsesor_b]) < PHOTOSENSOR_ERROR)
  {
    return true;
  }

  return false;
}

static void set_state(solar_tracker_t * solar_tracker, solar_tracker_state_t state) {
  if (state < LAST_STATE) {
    LOG_DEBUG("New state: " + String(state, DEC));
    solar_tracker->state = state;
  } else {
    LOG_ERROR("Set wrong state. Got to stop state");
    solar_tracker->state = STOP_STATE;
  }
}

/**
 * @brief Set the azimuth object
 * Only base on read from azimuth photoresistors set moving direction of azimuth servomotor
 * @param solar_tracker solar tracker object
 */
static void set_azimuth(solar_tracker_t * solar_tracker) {
    UNUSED(solar_tracker);
    // sensorValue = analogRead(A0);
    // outputValue = map(sensorValue, 0, 1023, 0, 255);

    LOG_DEBUG("More iluminated: " + String(get_more_illuminated_sensor(solar_tracker, PHOTOSENSOR_WHITE, PHOTOSENSOR_GREEN), DEC));
    LOG_DEBUG("Is equal: " + String(is_equal_illuminated_sensors(solar_tracker, PHOTOSENSOR_WHITE, PHOTOSENSOR_GREEN), DEC));
}

/**
 * @brief Set the elevation object
 * Only base on read from elevation photoresistors set moving direction of elevation servomotor
 * @param solar_tracker solar tracker object
 */
static void set_elevation(solar_tracker_t * solar_tracker) {
    UNUSED(solar_tracker);
}

static void stop_motors(solar_tracker_t * solar_tracker) {
    solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_STOP;
    solar_tracker->elevation_servo_state = SERVO_ELEVATION_STOP;
}

inline bool is_button_pressed(solar_tracker_t * solar_tracker) {
  if (solar_tracker->button_pressed_last == false && solar_tracker->button_pressed == true) {
    return true;
  }

  return false;
}

/* states implementation */

void init_state(solar_tracker_t * solar_tracker) {

  stop_motors(solar_tracker);

  solar_tracker->init_leds_state = !solar_tracker->init_leds_state;
  digitalWrite(LED_AZIMUTH_PIN, LOW);
  digitalWrite(LED_ELEVATION_PIN, solar_tracker->init_leds_state);


  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, AZIMUTH_CONTROL_STATE);
  }
}

void azimuth_control_state(solar_tracker_t * solar_tracker) {
  digitalWrite(LED_AZIMUTH_PIN, HIGH);
  digitalWrite(LED_ELEVATION_PIN, LOW);

  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, STOP_STATE);
  } else if (solar_tracker->azimuth_done) {
    set_state(solar_tracker, ELEVATION_CONTROL_STATE);
  } else {
    set_azimuth(solar_tracker);
  }
}

void elevation_control_state(solar_tracker_t * solar_tracker) {
  digitalWrite(LED_AZIMUTH_PIN, LOW);
  digitalWrite(LED_ELEVATION_PIN, HIGH);

  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, STOP_STATE);
  } else if (!solar_tracker->azimuth_done) {
    set_state(solar_tracker, AZIMUTH_CONTROL_STATE);
  } else {
    set_elevation(solar_tracker);
  }
}

void stop_state(solar_tracker_t * solar_tracker) {

  stop_motors(solar_tracker);

  solar_tracker->stop_leds_state = !solar_tracker->stop_leds_state;
  digitalWrite(LED_AZIMUTH_PIN, solar_tracker->stop_leds_state);
  digitalWrite(LED_ELEVATION_PIN, LOW);

  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, AZIMUTH_CONTROL_STATE);
  }
}

/* state machine */

void state_machine_evolution(solar_tracker_t * solar_tracker) {
  switch(solar_tracker->state) {
    case INIT_STATE:
      init_state(solar_tracker);
    break;
    case AZIMUTH_CONTROL_STATE:
      azimuth_control_state(solar_tracker);
    break;
    case ELEVATION_CONTROL_STATE:
      elevation_control_state(solar_tracker);
    break;
    case STOP_STATE:
      stop_state(solar_tracker);
    break;
    default:
      LOG_ERROR("Not allowed state!");
    break;
  }

  /* set button pressed last */
  solar_tracker->button_pressed_last = solar_tracker->button_pressed;

  /* clear button pressed */
  solar_tracker->button_pressed = false;
}