#pragma once

#define KEY_PING 10
#define KEY_PONG 11
#define KEY_GET_DATA 22
#define KEY_GET_DESC 23
#define KEY_BUMP_DATA 24

void message_handler_init();
void message_handler_deinit();

char *translate_error(AppMessageResult result);
void send_app_message(int key, char* value);