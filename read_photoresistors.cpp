
#include "read_photoresistors.h"

void read_photoresistors(solar_tracker_t * solar_tracker) {
  /* read analog input of photoresistors */
  solar_tracker->photosensors[PHOTOSENSOR_ORANGE] = analogRead(SENSOR_ORANGE_PIN);
  solar_tracker->photosensors[PHOTOSENSOR_GREEN] = analogRead(SENSOR_GREEN_PIN);
  solar_tracker->photosensors[PHOTOSENSOR_BLUE] = analogRead(SENSOR_BLUE_PIN);
  solar_tracker->photosensors[PHOTOSENSOR_WHITE] = analogRead(SENSOR_WHITE_PIN);

  /* print sensors values */
  LOG_DEBUG(String("Sensor orange: "   + String(solar_tracker->photosensors[PHOTOSENSOR_ORANGE], DEC)  +
                   " Sensor green: "   + String(solar_tracker->photosensors[PHOTOSENSOR_GREEN], DEC)   +
                   " Sensor blue: "    + String(solar_tracker->photosensors[PHOTOSENSOR_BLUE], DEC)    +
                   " Sensor white: "   + String(solar_tracker->photosensors[PHOTOSENSOR_WHITE], DEC)
                  )
            );
}