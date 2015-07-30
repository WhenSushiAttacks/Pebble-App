#include <pebble.h>

#include "handlers/app_message_handler.h"

int accelFilter[3];

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer_1[128];
  snprintf(s_buffer_1, sizeof(s_buffer_1), "%d,%d,%d", data[0].x, data[0].y, data[0].z - accelFilter[2]);
  // X makes sure the pebble is facing up, y is forward motion, z is height
  if(
    (data[0].x < -650 || (data[0].x > 1200 && data[0].x < 1350))
    &&
    ((data[0].y < 500 && data[0].y > 0) || (data[0].y < -200 && data[0].y > -1100))
    &&
    (data[0].z - accelFilter[2]) < -300 && (data[0].z - accelFilter[2]) > -700
    ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,s_buffer_1);
    send_app_message(KEY_BUMP_DATA, s_buffer_1);
  } else if(
    (data[0].y < -650 || (data[0].y > 1200 && data[0].y < 1350))
    &&
    ((data[0].x < 500 && data[0].x > 0) || (data[0].x < -200 && data[0].x > -1100))
    &&
    (data[0].z - accelFilter[2]) < -300 && (data[0].z - accelFilter[2]) > -1300
    ) {
    APP_LOG(APP_LOG_LEVEL_DEBUG,s_buffer_1);
    send_app_message(KEY_BUMP_DATA, s_buffer_1);
  }
  accelFilter[0] = data[0].x;
  accelFilter[1] = data[0].y;
  accelFilter[2] = data[0].z;
}

void gesture_handler_init() {
  //Enable accelerometer access
  int num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);
}

void gesture_handler_deinit() {
  accel_data_service_unsubscribe();
}