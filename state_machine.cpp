#include "state_machine.h"

/* static helper function */

static void set_state(solar_tracker_t * solar_tracker, solar_tracker_state_t state) {
  if (state < LAST_STATE) {
    LOG_DEBUG("New state: " + String(state, DEC));
    solar_tracker->state = state;
  } else {
    LOG_ERROR("Set wrong state. Got to stop state");
    solar_tracker->state = STOP_STATE;
  }
}

static void set_azimuth(solar_tracker_t * solar_tracker) {
    UNUSED(solar_tracker);
    // sensorValue = analogRead(A0);
    // outputValue = map(sensorValue, 0, 1023, 0, 255);
}

static void set_elevation(solar_tracker_t * solar_tracker) {
    UNUSED(solar_tracker);
}

static void stop_motors(solar_tracker_t * solar_tracker) {
    UNUSED(solar_tracker);
    // leave servos in current position
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