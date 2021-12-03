/*
 * Leds for signalling working state of system.
 * Button for start and stop system.
 *
 */

#define STEP_MS             (1000)

#define LOG_ALWAYS(s)       Serial.print(String(s) + "\n\r");
#define LOG_ERROR(s)        LOG_ALWAYS(s)

#define DEBUG

#ifdef DEBUG
    #define LOG_DEBUG(s)    LOG_ALWAYS(s)
#else
    #define LOG_DEBUG(s)
#endif

/* photo resistor analog pins */
const int SENSOR_ORANGE_PIN = A5, SENSOR_GREEN_PIN  = A4, SENSOR_BLUE_PIN   = A3, SENSOR_WHITE_PIN  = A2;

/* photoresistors values values */
float sensor_orange, sensor_green, sensor_blue, sensor_white;

/* button press state */
bool button_pressed;

typedef enum {
  INIT_STATE = 0,
  ELEVATION_CONTROL_STATE,
  AZIMUTH_CONTROL_STATE,
  STOP_STATE
} solar_tracker_state_t;

typedef struct {
  solar_tracker_state_t state;
} solar_tracker_state_machine_t;

solar_tracker_state_machine_t solat_starcker_state_machine;

void setup() {
  Serial.begin(9600);

  LOG_ALWAYS("Solar panle tracker system started.");
}

void loop() {
  read_photoresistors();

  update_state_machine();

  control_servomotors();

  delay(STEP_MS);
}

void read_photoresistors(void) {

  // read analog input of photoresistors
  sensor_orange = analogRead(SENSOR_ORANGE_PIN);
  sensor_green = analogRead(SENSOR_GREEN_PIN);
  sensor_blue = analogRead(SENSOR_BLUE_PIN);
  sensor_white = analogRead(SENSOR_WHITE_PIN);

  // print sensors values
  String photo_resistor_output = String("Sensor orange:"   + String(sensor_orange, DEC)  +
                                        "Sensor green:"    + String(sensor_green, DEC)   +
                                        "Sensor blue:"     + String(sensor_blue, DEC)    +
                                        "Sensor white:"    + String(sensor_white, DEC)   );
  LOG_DEBUG(photo_resistor_output);
}

void update_state_machine(void) {

  
}

void control_servomotors(void) {


}
