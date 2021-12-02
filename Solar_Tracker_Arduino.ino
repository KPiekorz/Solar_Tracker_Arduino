/*
 * Leds for signalling working state of system.
 * Button for start and stop system.
 *
 */

#define STEP_MS 1000

#define DEBUG

#ifdef DEBUG
    #define LOG_DEBUG(s)      Serial.print(s);
#else
    #define LOG_DEBUG(s)
#endif

#define LOG_ERROR(s)          Serial.print(s);

/* photo resistor analog pins */
const int SENSOR_ORANGE_PIN = A5, SENSOR_GREEN_PIN  = A4, SENSOR_BLUE_PIN   = A3, SENSOR_WHITE_PIN  = A2;

/* photoresistors values values */
float sensor_orange, sensor_green, sensor_blue, sensor_white;

/* button press state */
bool button_pressed;

void setup() {

  Serial.begin(9600);
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
  String photo_resistor_output = String(" Sensor orange:"   + String(sensor_orange, DEC)  +
                                        " Sensor green:"    + String(sensor_green, DEC)   +
                                        " Sensor blue:"     + String(sensor_blue, DEC)    +
                                        " Sensor white:"    + String(sensor_white, DEC)   +
                                        "\n");
  LOG_DEBUG(photo_resistor_output);
}

void update_state_machine(void) {


}

void control_servomotors(void) {


}
