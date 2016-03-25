#include "window.h"

static int battery_level;
static Layer *battery_layer;
static GFont time_font, date_font, day_font;
static TextLayer *time_layer, *date_layer, *day_layer;
static char time_text[8], day_text[10], date_text[18];
static char *time_format;
static char *nomes_dia[] = { "domingo", "luns", "martes", "mércores", "xoves", "venres", "sábado" };
static char *nomes_mes[] = { "de xaneiro", "de febreiro", "de marzo", "de abril", "de maio", "de xuño", "de xullo",
                               "de agosto", "de setembro", "de outubro", "de novembro", "de decembro" };

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int width = (int)(int)(((int)battery_level * 144) / 100);
  if (config_get(INVERTIR_CORES) == 0) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    graphics_context_set_fill_color(ctx, GColorWhite);
  } else {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_fill_rect(ctx, bounds, 0, GCornerNone);
    graphics_context_set_fill_color(ctx, GColorBlack);
  }
  graphics_fill_rect(ctx, GRect(0, 0, width, bounds.size.h), 0, GCornerNone); 
}

static void battery_callback(BatteryChargeState state){ 
  battery_level = state.charge_percent; 
}

static void time_callback(struct tm *tick_time, TimeUnits units_changed) {
  int mes = tick_time->tm_mon;
  int dia = tick_time->tm_wday;
  strftime(date_text, 10, "%e ", tick_time);
  strcat(date_text, nomes_mes[mes]);
  strcpy(day_text, nomes_dia[dia]);
  
  if (config_get(MOSTRAR_DIA) == 1) {
      text_layer_set_text(day_layer, day_text);
  }

  if (config_get(MOSTRAR_DATA) == 1) {
      text_layer_set_text(date_layer, date_text);
  }

  if (clock_is_24h_style()) { time_format = "%R"; } else { time_format = "%I:%M"; }
  strftime(time_text, sizeof(time_text), time_format, tick_time);

  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  text_layer_set_text(time_layer, time_text);
}

static void load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // mostra a hora (formato 00:00) // 12 ou 24 horas
  if((config_get(MOSTRAR_DATA) == 0) && (config_get(MOSTRAR_DIA) == 0)) {
    time_layer = text_layer_create(GRect(0, 45, bounds.size.w, 70));
  } else {
    time_layer = text_layer_create(GRect(0, 64, bounds.size.w, 70)); // esta non se move
  }
  time_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_60);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, time_font);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));
  
  if(config_get(MOSTRAR_DIA) == 1) {
    // mostra o día da semana (luns...domingo)
    if(config_get(MOSTRAR_DATA) == 1) {
      day_layer = text_layer_create(GRect(0, 12, 138, 34));
    } else {
      day_layer = text_layer_create(GRect(0, 34, 138, 34));
    }
    day_font  = font_load(RESOURCE_ID_ROBOTO_BOLD_26);
    text_layer_set_text_alignment(day_layer, GTextAlignmentRight);
    text_layer_set_font(day_layer, day_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  }
  if(config_get(MOSTRAR_DATA) == 1) {
    // mostra a data (formato "1 de enero")
    date_layer = text_layer_create(GRect(0, 38, 138, 28));
    date_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_21);
    text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
    text_layer_set_font(date_layer, date_font);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  }
  // invirte as cores da esfera en base á configuracion:
  if (config_get(INVERTIR_CORES) == 0) {
    text_layer_set_text_color(time_layer, GColorWhite);
    text_layer_set_text_color(day_layer, GColorWhite);
    text_layer_set_text_color(date_layer, GColorWhite);
    text_layer_set_background_color(time_layer, GColorBlack);
    text_layer_set_background_color(day_layer, GColorBlack);
    text_layer_set_background_color(date_layer, GColorBlack);
  } else {
    text_layer_set_text_color(time_layer, GColorBlack);
    text_layer_set_text_color(day_layer, GColorBlack);
    text_layer_set_text_color(date_layer, GColorBlack);  
    text_layer_set_background_color(time_layer, GColorWhite);
    text_layer_set_background_color(day_layer, GColorWhite);
    text_layer_set_background_color(date_layer, GColorWhite);
  } 
  // mostra o porcentaxe de carga de bateria
  if (config_get(MOSTRAR_CARGA) == 1) {
    battery_layer = layer_create(GRect(0, (bounds.size.h - 4), bounds.size.w, 4));
    layer_set_update_proc(battery_layer, battery_update_proc);
    layer_add_child(window_get_root_layer(window), battery_layer);
    layer_mark_dirty(battery_layer);
  }
}

static void unload(Window *window) {
    text_layer_destroy(time_layer); 
    fonts_unload_custom_font(time_font);
    if(config_get(MOSTRAR_DATA) == 1) {
      text_layer_destroy(date_layer); 
      fonts_unload_custom_font(date_font);
    }
    
    if(config_get(MOSTRAR_DIA) == 1) {
      text_layer_destroy(day_layer); 
      fonts_unload_custom_font(day_font);
    }
    window_destroy(watchface);
}

void main_window_push() {
  // inicializamos a ventana para o reloxo e establecemos a cor de fondo
  watchface = window_create();
  if (config_get(INVERTIR_CORES) == 0) { 
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

  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);
  time_callback(tm_struct, (TimeUnits) NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, time_callback);
  
  if (config_get(MOSTRAR_CARGA) == 1) {
    battery_callback(battery_state_service_peek());
    battery_state_service_subscribe(battery_callback);
  }
}