#include "common.h" // elementos comúns - incluir primeiro
#include "window.h" // interface de usuario -- window.load e window.unload 

int main() {
  // inicializamos a ventana para o reloxo e establecemos a cor de fondo
  watchface = window_create();
  if (color == 0) { 
    window_set_background_color(watchface, GColorBlack); 
  } else { 
    window_set_background_color(watchface, GColorWhite); 
  }

  // establecemos os asistentes para inicializar e deinicializar o reloxo:
  window_set_window_handlers(watchface, (WindowHandlers) {
     .load = load, 
     .unload = unload 
   });
  
  // o noso reloxo non ten animación
  window_stack_push(watchface, false);

  // obtemos hora e data por primeira vez:
  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);
  time_callback(tm_struct, (TimeUnits) NULL);

  // obtemos carga de bateria por primeira vez:
  battery_callback(battery_state_service_peek());

  // suscribímonos aos eventos do sistema para hora/data e carga de batería:
  tick_timer_service_subscribe(MINUTE_UNIT, time_callback);
  battery_state_service_subscribe(battery_callback);

  // esperamos polos eventos do sistema:
  app_event_loop();
}