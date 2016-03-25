#include "common.h" // elementos comúns - incluir primeiro
#include "config.h" // configuracion via web a traves da app de pebble
#include "window.h" // interface de usuario -- window.load e window.unload



// recepcion de mensaxes de entrada (recibe e aplica a configuracion)
static void inbox_callback(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  while(t) {
    persist_write_bool(t->key, strcmp(t->value->cstring, "true") == 0 ? true : false);
    t = dict_read_next(iter);
  } // ------- axustes gardados
  rexistro("axustes gardados correctamente: actualizando esfera...")
  config_init(); // recargamos a configuracion da esfera.
  window_stack_remove(watchface, false); // eliminamos a ventana do stack
  main_window_push();  // actualizamos a ventana coa nova configuracion.
  rexistro("esfera actualizada!")
}

int main() {
  // suscribimonos aos mensaxes de entrada do sistema
  app_message_register_inbox_received(inbox_callback);
  app_message_open(124, 124);

  // obtemos os valores de configuracion
  config_init();

  // inicializamos a ventana e mandamola ao stack
  main_window_push();  
  
  // esperamos por eventos do sistema
  rexistro("esperando por eventos do sistema");
  app_event_loop();
}