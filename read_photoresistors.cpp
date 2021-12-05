
#include "read_photoresistors.h"

void read_photoresistors(solar_tracker_t * solar_tracker) {
  /* read analog input of photoresistors */
  solar_tracker->sensor_orange = analogRead(SENSOR_ORANGE_PIN);
  solar_tracker->sensor_green = analogRead(SENSOR_GREEN_PIN);
  solar_tracker->sensor_blue = analogRead(SENSOR_BLUE_PIN);
  solar_tracker->sensor_white = analogRead(SENSOR_WHITE_PIN);

  /* print sensors values */
  LOG_DEBUG(String("Sensor orange: "   + String(solar_tracker->sensor_orange, DEC)  +
                   " Sensor green: "   + String(solar_tracker->sensor_green, DEC)   +
                   " Sensor blue: "    + String(solar_tracker->sensor_blue, DEC)    +
                   " Sensor white: "   + String(solar_tracker->sensor_white, DEC)
                  )
            );
}