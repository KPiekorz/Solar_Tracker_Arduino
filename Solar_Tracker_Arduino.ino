/*
 * NOTES:
 * Leds for signalling working state of system.
 * Button for start and stop system.
 */

#define STEP_MS             (500)

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

/* servos pwm pins */
const int SERVO_AZIMUTH_PIN = 10, SERVO_ELEVATION_PIN = 11;

/* servos default angles */
const int SERVO_AZIMUTH_DEFAULT_ANGLE = 90;
const int SERVO_ELEVATION_DEFAULT_ANGLE = 90;

/* leds pins */
const int LED_ELEVATION_PIN = 12, LED_AZIMUTH_PIN = 13;

/* button pin */
const int BUTTTON_PIN = 2;

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
  /* start/stop button state last */
  bool button_pressed_last;
  /* azimuth set state */
  bool azimuth_done;
  /* servo angle */
  int azimuth_angle, elevation_angle;
  /* stop and init leds last state */
  int stop_leds_state, init_leds_state;
} solar_tracker_t;

solar_tracker_t solar_tracker = {.state = INIT_STATE,
                                 .sensor_orange = 0, .sensor_green = 0, .sensor_blue = 0, .sensor_white = 0,
                                 .button_pressed = false,
                                 .button_pressed_last = false,
                                 .azimuth_done = false,
                                 .azimuth_angle = SERVO_AZIMUTH_DEFAULT_ANGLE, .elevation_angle = SERVO_ELEVATION_DEFAULT_ANGLE,
                                 .stop_leds_state = LOW, .init_leds_state = LOW,
                                };

void setup() {
  /* init serial com port */
  Serial.begin(9600);
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
  read_photoresistors();
  state_machine_evolution();
  control_servomotors();
  delay(STEP_MS);
}

/* interrupt */

void button_interrupt(void) {
  LOG_DEBUG("Button pressed.");
  solar_tracker.button_pressed = true;
}

/* global function */

void read_photoresistors(void) {
  /* read analog input of photoresistors */
  solar_tracker.sensor_orange = analogRead(SENSOR_ORANGE_PIN);
  solar_tracker.sensor_green = analogRead(SENSOR_GREEN_PIN);
  solar_tracker.sensor_blue = analogRead(SENSOR_BLUE_PIN);
  solar_tracker.sensor_white = analogRead(SENSOR_WHITE_PIN);

  /* print sensors values */
  LOG_DEBUG(String("Sensor orange: "   + String(solar_tracker.sensor_orange, DEC)  +
                   " Sensor green: "   + String(solar_tracker.sensor_green, DEC)   +
                   " Sensor blue: "    + String(solar_tracker.sensor_blue, DEC)    +
                   " Sensor white: "   + String(solar_tracker.sensor_white, DEC)
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

void set_azimuth(void) {
    // sensorValue = analogRead(A0);
    // outputValue = map(sensorValue, 0, 1023, 0, 255);
}

void set_elevation(void) {

}

void stop_motors(void) {
  // leave servos in current position
}

inline bool is_button_pressed(void) {
  if (solar_tracker.button_pressed_last == false && solar_tracker.button_pressed == true) {
    return true;
  }

  return false;
}

void state_machine_evolution(void) {
  switch(solar_tracker.state) {
    case INIT_STATE: {

      solar_tracker.init_leds_state = !solar_tracker.init_leds_state;
      digitalWrite(LED_AZIMUTH_PIN, LOW);
      digitalWrite(LED_ELEVATION_PIN, solar_tracker.init_leds_state);

      if(is_button_pressed()) {
        set_state(AZIMUTH_CONTROL_STATE);
      }
    }
    break;
    case AZIMUTH_CONTROL_STATE: {

      digitalWrite(LED_AZIMUTH_PIN, HIGH);
      digitalWrite(LED_ELEVATION_PIN, LOW);

      if(is_button_pressed()) {
        set_state(STOP_STATE);
      } else if (solar_tracker.azimuth_done) {
        set_state(ELEVATION_CONTROL_STATE);
      } else {
        set_azimuth();
      }
    }
    break;
    case ELEVATION_CONTROL_STATE: {

      digitalWrite(LED_AZIMUTH_PIN, LOW);
      digitalWrite(LED_ELEVATION_PIN, HIGH);

      if(is_button_pressed()) {
        set_state(STOP_STATE);
      } else if (!solar_tracker.azimuth_done) {
        set_state(AZIMUTH_CONTROL_STATE);
      } else {
        set_elevation();
      }
    }
    break;
    case STOP_STATE: {

      solar_tracker.stop_leds_state = !solar_tracker.stop_leds_state;
      digitalWrite(LED_AZIMUTH_PIN, solar_tracker.stop_leds_state);
      digitalWrite(LED_ELEVATION_PIN, LOW);

      stop_motors();
      if(is_button_pressed()) {
        set_state(AZIMUTH_CONTROL_STATE);
      }
    }
    break;
    default:
      LOG_ERROR("Not allowed state!");
    break;
  }

  /* set button pressed last */
  solar_tracker.button_pressed_last = solar_tracker.button_pressed;

  /* clear button pressed */
  solar_tracker.button_pressed = false;
}

void control_servomotors(void) {
  // first read old angle of servo and try move servo really slowlly...
}
