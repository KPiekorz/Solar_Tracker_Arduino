
/* 
const int SENSOR_ORANGE_PIN = A5;
const int SENSOR_GREEN_PIN  = A4;
const int SENSOR_BLUE_PIN   = A3;
const int SENSOR_WHITE_PIN  = A2;

void setup() {

  Serial.begin(9600);
}

void loop() {

  float sensor_orange = analogRead(SENSOR_ORANGE_PIN);
  float sensor_green = analogRead(SENSOR_GREEN_PIN);
  float sensor_blue = analogRead(SENSOR_BLUE_PIN);
  float sensor_white = analogRead(SENSOR_WHITE_PIN);

  String photo_resistor_output = String(" Sensor orange:"   + String(sensor_orange, DEC)  +
                                        " Sensor green:"    + String(sensor_green, DEC)   +
                                        " Sensor blue:"     + String(sensor_blue, DEC)    +
                                        " Sensor white:"    + String(sensor_white, DEC)   +
                                        "\n");
  Serial.print(photo_resistor_output);

  delay(1000);
}
