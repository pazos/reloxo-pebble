#include "comunicacion.h"

// lectura do buzón de entrada, esperando cambios nos axustes dende a app de pebble
static void inbox_callback(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  while(t) {
    persist_write_bool(t->key, strcmp(t->value->cstring, "true") == 0 ? true : false);
    t = dict_read_next(iter);
  } // configuración gardada

    cargar_configuracion(); // obtemos a nova configuración en base aos parámetros establecidos
    iniciar_esfera();       // refrescamos a ventana principal para que os cambios sexan visibles
}

void cargar_comunicacions() {
  app_message_register_inbox_received(inbox_callback);
  app_message_open(200, 700);
