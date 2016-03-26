#include "common.h"
#include "config.h"
#include "window.h"

static void inbox_callback(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  while(t) {
    persist_write_bool(t->key, strcmp(t->value->cstring, "true") == 0 ? true : false);
    t = dict_read_next(iter);
  } 
    config_init();
    main_window_push();
}

int main() {
  app_message_register_inbox_received(inbox_callback);
  app_message_open(124, 124);
  config_init();
  main_window_push();
  app_event_loop();
}