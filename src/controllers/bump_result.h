#pragma once

void bump_result_init();
void bump_result_deinit();
void bump_result_show();

void bump_result_setname(char *newName);
void bump_result_setsuccess(bool isSuccessful);
void timer_callback(void *data);