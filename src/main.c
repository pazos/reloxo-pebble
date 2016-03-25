#include "common.h" // elementos comúns - incluir primeiro
#include "window.h" // interface de usuario -- window.load e window.unload
#include "config.h" // configuracion via web a traves da app de pebble
#include <stdio.h>
// recepcion de mensaxes de entrada (recibe e aplica a configuracion)
static void inbox_callback(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  while(t) {
    persist_write_bool(t->key, strcmp(t->value->cstring, "true") == 0 ? true : false);
    t = dict_read_next(iter);
  }
  config_init();
  window_stack_remove(watchface, false);
  main_window_push();
}

int main() {
  app_message_register_inbox_received(inbox_callback);
  app_message_open(124, 124);
  config_init();
  main_window_push();  
  app_event_loop();
}