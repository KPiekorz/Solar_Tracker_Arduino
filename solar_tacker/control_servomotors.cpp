#include <Servo.h>

#include "control_servomotors.h"

static Servo servo_azimuth, servo_elevation;
static int azimuth_servo_angle = DEFAULT_SERVOS_ANGLE, elevatin_servo_angle = DEFAULT_SERVOS_ANGLE;

/* static helper function */

static int increment_servo_angle(int angle) {
    if ((angle + SERVO_ANGLE_INTERVAL) > SERVO_MAX_ANGLE) {
        return SERVO_MAX_ANGLE;
    }

    return angle + SERVO_ANGLE_INTERVAL;
}

static int decrease_servo_angle(int angle) {
    if ((angle - SERVO_ANGLE_INTERVAL) < SERVO_MIN_ANGLE) {
        return SERVO_MIN_ANGLE;
    }

    return angle - SERVO_ANGLE_INTERVAL;
}

static void update_servo_angle(solar_tracker_t * solar_tracker, int servo_pin) {

    switch(servo_pin) {
        case SERVO_AZIMUTH_PIN:
            if(solar_tracker->azimuth_servo_state != SERVO_AZIMUTH_STOP) {
                if (solar_tracker->azimuth_servo_state == SERVO_AZIMUTH_MOVE_RIGHT) {
                    azimuth_servo_angle = increment_servo_angle(azimuth_servo_angle);
                } else {
                    azimuth_servo_angle = decrease_servo_angle(azimuth_servo_angle);
                }

                /* set new serwo angle */
                servo_azimuth.write(azimuth_servo_angle);

                /* store new servo angle to flash... */

            }
        break;
        case SERVO_ELEVATION_PIN:
            if(solar_tracker->elevation_servo_state != SERVO_ELEVATION_STOP) {
                if (solar_tracker->elevation_servo_state == SERVO_ELEVATION_MOVE_UP) {
                    elevatin_servo_angle = increment_servo_angle(elevatin_servo_angle);
                } else {
                    elevatin_servo_angle = decrease_servo_angle(elevatin_servo_angle);
                }

                /* set new serwo angle */
                servo_elevation.write(elevatin_servo_angle);

                /* store new servo angle to flash... */

            }
        break;
        default:
            LOG_ERROR("Wrong servo pin.");
        break;
    }

}

/* global function */

void setup_servomotors(void) {
    servo_azimuth.attach(SERVO_AZIMUTH_PIN);
    servo_elevation.attach(SERVO_ELEVATION_PIN);

    /* read of old servo angle from flash memory */

}

void control_servomotors(solar_tracker_t * solar_tracker) {
    update_servo_angle(solar_tracker, SERVO_AZIMUTH_PIN);
    update_servo_angle(solar_tracker, SERVO_ELEVATION_PIN);

    LOG_DEBUG("Azimuth servo angle: " + String(azimuth_servo_angle, DEC) + " Elevation servo angle: " + String(elevatin_servo_angle, DEC));
}