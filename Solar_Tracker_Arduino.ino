/*
 * Leds for signalling working state of system.
 * Button for start and stop system.
 *
 */

#define STEP_MS             (1000)

#define LOG_ALWAYS(s)       Serial.print("[ALWAYS]: " + String(s) + "\n\r");
#define LOG_ERROR(s)        Serial.print("[ERROR]: " + String(s) + "\n\r");

#define DEBUG

#ifdef DEBUG
    #define LOG_DEBUG(s)    Serial.print("[DEBUG]: " + String(s) + "\n\r");
#else
    #define LOG_DEBUG(s)
#endif

/* photo resistor analog pins */
const int SENSOR_ORANGE_PIN = A5, SENSOR_GREEN_PIN  = A4, SENSOR_BLUE_PIN   = A3, SENSOR_WHITE_PIN  = A2;

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
  int sensor_orange, sensor_green, sensor_blue, sensor_white;
  /* start/stop button state */
  bool button_pressed;
  /* azimuth set state */
  bool azimuth_done;
} solar_tracker_t;

solar_tracker_t solar_tracker;

void setup() {
  Serial.begin(9600);
}

void loop() {
  read_photoresistors();
  state_machine_evolution();
  control_servomotors();
  delay(STEP_MS);
}

void read_photoresistors(void) {

  // read analog input of photoresistors
  solar_tracker.sensor_orange = analogRead(SENSOR_ORANGE_PIN);
  solar_tracker.sensor_green = analogRead(SENSOR_GREEN_PIN);
  solar_tracker.sensor_blue = analogRead(SENSOR_BLUE_PIN);
  solar_tracker.sensor_white = analogRead(SENSOR_WHITE_PIN);

  // print sensors values
  LOG_DEBUG(String("Sensor orange: "   + String(solar_tracker.sensor_orange, DEC)  +
                   " Sensor green: "    + String(solar_tracker.sensor_green, DEC)   +
                   " Sensor blue: "     + String(solar_tracker.sensor_blue, DEC)    +
                   " Sensor white: "    + String(solar_tracker.sensor_white, DEC)
                  )
            );
}

void set_state(solar_tracker_state_t state) {
  if (state < LAST_STATE) {
    LOG_DEBUG("New state: " + String(state, DEC));
    solar_tracker.state = state;
  } else {
    LOG_ERROR("Set wrong state. Got to stop state");
    solar_tracker.state = STOP_STATE;
  }
}

void state_machine_evolution(void) {

  switch(solar_tracker.state) {
    case INIT_STATE: {
      if(solar_tracker.button_pressed) {
        set_state(AZIMUTH_CONTROL_STATE);
      }
    }
    break;
    case AZIMUTH_CONTROL_STATE: {
      if(solar_tracker.button_pressed) {
        set_state(STOP_STATE);
      } else if (solar_tracker.azimuth_done) {
        set_state(ELEVATION_CONTROL_STATE);
      } else {
        set_azimuth();
      }
    }
    break;
    case ELEVATION_CONTROL_STATE: {
      if(solar_tracker.button_pressed) {
        set_state(STOP_STATE);
      } else if (!solar_tracker.azimuth_done) {
        set_state(AZIMUTH_CONTROL_STATE);
      } else {
        set_elevation();
      }
    }
    break;
    case STOP_STATE: {
      if(solar_tracker.button_pressed) {
        set_state(AZIMUTH_CONTROL_STATE);
      }
    }
    break;
    default:
      LOG_ERROR("Not allowed state!");
    break;
  }

  /* clear button pressed */
  solar_tracker.button_pressed = FALSE;
}

void control_servomotors(void) {


}
