#include <pebble.h>

#include "controllers/connect_device.h"
#include "controllers/main_view.h"
#include "controllers/detail_view.h"
#include "controllers/bump_prep.h"
#include "controllers/bump_result.h"

#include "handlers/app_message_handler.h"

static void init(void) {
  connect_device_init();
  main_view_init();
  detail_view_init();
  bump_prep_init();
  bump_result_init();
  message_handler_init();

  connect_device_show();
}

static void deinit(void) {
  message_handler_deinit();
  connect_device_deinit();
  main_view_deinit();
  detail_view_deinit();
  bump_prep_deinit();
  bump_result_deinit();
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushing digital clock window...");

  app_event_loop();
  deinit();
}
