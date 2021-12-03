/*
 * NOTES:
 * Leds for signalling working state of system.
 * Button for start and stop system.
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

/* servos pwm pins */
const int SERVO_AZIMUTH_PIN = 10, SERVO_ELEVATION_PIN = 11;

/* leds pins */
const int LED_AZIMUTH_PIN = 12, LED_ELEVATION_PIN = 13;

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

solar_tracker_t solar_tracker = {.state = INIT_STATE,
                                 .sensor_orange = 0, .sensor_green = 0, .sensor_blue = 0, .sensor_white = 0,
                                 .button_pressed = false,
                                 .azimuth_done = false
                                };

void setup() {
  /* init serial com port */
  Serial.begin(9600);
  /* init interrupt for button */
  attachInterrupt(digitalPinToInterrupt(2), button_interrupt, RISING);	// Configure interrupt
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
      stop_motors();
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
  solar_tracker.button_pressed = false;
}

void control_servomotors(void) {

}
