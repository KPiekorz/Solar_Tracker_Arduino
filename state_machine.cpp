#include "state_machine.h"
#include <math.h>

/* static helper function */

static photosensor_t get_more_illuminated_sensor(solar_tracker_t * solar_tracker, photosensor_t photsesor_a, photosensor_t photsesor_b) {

  if (solar_tracker->photosensors[photsesor_a] < solar_tracker->photosensors[photsesor_b]) {
    return photsesor_a;
  }

  return photsesor_b;
}

static photosensor_t get_the_most_illuminated_sensor(solar_tracker_t * solar_tracker) {

  photosensor_t p1 =  get_more_illuminated_sensor(solar_tracker, PHOTOSENSOR_WHITE, PHOTOSENSOR_GREEN);
  photosensor_t p2 =  get_more_illuminated_sensor(solar_tracker, PHOTOSENSOR_BLUE, PHOTOSENSOR_ORANGE);

  return get_more_illuminated_sensor(p1, p2);
}

static bool is_equal_illuminated_sensors(solar_tracker_t * solar_tracker, photosensor_t photsesor_a, photosensor_t photsesor_b) {

  if (abs(solar_tracker->photosensors[photsesor_a] - solar_tracker->photosensors[photsesor_b]) < PHOTOSENSOR_ERROR)
  {
    return true;
  }

  return false;
}

static bool is_azimuth_done(solar_tracker_t * solar_tracker) {
  if (is_equal_illuminated_sensors(solar_tracker, PHOTOSENSOR_WHITE, PHOTOSENSOR_GREEN) ||
      is_equal_illuminated_sensors(solar_tracker, PHOTOSENSOR_BLUE, PHOTOSENSOR_ORANGE)) {
    return true;
  }

  return false;
}

static bool is_elevation_done(solar_tracker_t * solar_tracker) {
  if (is_equal_illuminated_sensors(solar_tracker, PHOTOSENSOR_WHITE, PHOTOSENSOR_BLUE) ||
      is_equal_illuminated_sensors(solar_tracker, PHOTOSENSOR_GREEN, PHOTOSENSOR_ORANGE)) {
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

    if (is_azimuth_done(solar_tracker)) {
      solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_STOP;
    } else {
      switch(get_the_most_illuminated_sensor(solar_tracker)) {
        case PHOTOSENSOR_GREEN:
        case PHOTOSENSOR_ORANGE:
          solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_MOVE_RIGHT;
        break;
        case PHOTOSENSOR_WHITE:
        case PHOTOSENSOR_BLUE:
          solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_MOVE_LEFT;
        break;
        default:
          LOG_ERROR("Wrong sensor in set azimuth.");
        break;
      }
    }
}

/**
 * @brief Set the elevation object
 * Only base on read from elevation photoresistors set moving direction of elevation servomotor
 * @param solar_tracker solar tracker object
 */
static void set_elevation(solar_tracker_t * solar_tracker) {

  if (is_elevation_done(solar_tracker)) {
    solar_tracker->elevation_servo_state = SERVO_ELEVATION_STOP;
  } else {

  }
}

static void stop_motors(solar_tracker_t * solar_tracker) {
    solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_STOP;
    solar_tracker->elevation_servo_state = SERVO_ELEVATION_STOP;
}

static bool is_button_pressed(solar_tracker_t * solar_tracker) {
  if (solar_tracker->button_pressed_last == false && solar_tracker->button_pressed == true) {
    return true;
  }

  return false;
}

/* states implementation */

void init_state(solar_tracker_t * solar_tracker) {

  /* always make sure to stop both motor in init stop */
  stop_motors(solar_tracker);

  /* leds state signalling */
  solar_tracker->init_leds_state = !solar_tracker->init_leds_state;
  digitalWrite(LED_AZIMUTH_PIN, LOW);
  digitalWrite(LED_ELEVATION_PIN, solar_tracker->init_leds_state);

  /* state  acttion to perform */
  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, AZIMUTH_CONTROL_STATE);
  }
}

void azimuth_control_state(solar_tracker_t * solar_tracker) {

  /* always stop elevation servo in azimuth control state */
  solar_tracker->elevation_servo_state = SERVO_ELEVATION_STOP;

  /* leds state signalling */
  digitalWrite(LED_AZIMUTH_PIN, HIGH);
  digitalWrite(LED_ELEVATION_PIN, LOW);

  /* state  acttion to perform */
  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, STOP_STATE);
  } else if (is_azimuth_done(solar_tracker)) {
    set_state(solar_tracker, ELEVATION_CONTROL_STATE);
  } else {
    set_azimuth(solar_tracker);
  }
}

void elevation_control_state(solar_tracker_t * solar_tracker) {

  /* always stop azimuth servo in elevation control state */
  solar_tracker->azimuth_servo_state = SERVO_AZIMUTH_STOP;

  /* leds state signalling */
  digitalWrite(LED_AZIMUTH_PIN, LOW);
  digitalWrite(LED_ELEVATION_PIN, HIGH);

  /* state  acttion to perform */
  if(is_button_pressed(solar_tracker)) {
    set_state(solar_tracker, STOP_STATE);
  } else if (!is_azimuth_done(solar_tracker)) {
    set_state(solar_tracker, AZIMUTH_CONTROL_STATE);
  } else {
    set_elevation(solar_tracker);
  }
}

void stop_state(solar_tracker_t * solar_tracker) {

  /* always stop both motors in stop state */
  stop_motors(solar_tracker);

  /* leds state signalling */
  solar_tracker->stop_leds_state = !solar_tracker->stop_leds_state;
  digitalWrite(LED_AZIMUTH_PIN, solar_tracker->stop_leds_state);
  digitalWrite(LED_ELEVATION_PIN, LOW);

  /* state  acttion to perform */
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