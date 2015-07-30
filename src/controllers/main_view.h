#pragma once

#define KEY_GET_DATA 22

void main_view_init();
void main_view_deinit();
void main_view_show();

char *translate_error(AppMessageResult result);
void load_item_data(DictionaryIterator *iterator);
char** str_split(char* a_str, const char a_delim);