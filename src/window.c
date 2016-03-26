#include "window.h"

static Window *watchface;
static Layer *battery_layer;
static GFont time_font, date_font, day_font;
static TextLayer *time_layer, *date_layer, *day_layer;

static int battery_level;
static char *time_format;
static char *nomes_dia[] = { "domingo", "luns", "martes", "mércores", "xoves", "venres", "sábado" };
static char *nomes_mes[] = { "de xaneiro", "de febreiro", "de marzo", "de abril", "de maio", "de xuño", "de xullo",
                               "de agosto", "de setembro", "de outubro", "de novembro", "de decembro" };
static char time_text[8], day_text[10], date_text[18];
static bool saida_sucia = false;

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int width;
  
  if (battery_level < 20) {
    width = 14;  
  } else {
    width = (int)(float)(((float)battery_level * 144.0F) / 100.0F);
  }

  if (!config_get(INVERTIR_CORES)) {
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
  
  if (config_get(MOSTRAR_DIA)) {
      text_layer_set_text(day_layer, day_text);
  }

  if (config_get(MOSTRAR_DATA)) {
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
  LOG("------ ARRANQUE DA APP ------");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // declaramos todos os recursos, con independencia de se os vamos a usar ou non...
  time_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_60);
  date_font = font_load(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_21);
  day_font  = font_load(RESOURCE_ID_ROBOTO_BOLD_26);

  // ----> comeza declaración dinámica de capas
  if((!config_get(MOSTRAR_DATA)) && (!config_get(MOSTRAR_DIA))) {
    time_layer = text_layer_create(GRect(0, 48, bounds.size.w, 70));
  } else {
    time_layer = text_layer_create(GRect(0, 60, bounds.size.w, 70));
  } 
  if(config_get(MOSTRAR_DIA)) {
    date_layer = text_layer_create(GRect(0, 40, 138, 28));
    text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
  } else {
    date_layer = text_layer_create(GRect(0, 38, bounds.size.w, 28));
    text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);    
  }  
  if(config_get(MOSTRAR_DATA)) {
    day_layer = text_layer_create(GRect(0, 12, 138, 34));
    text_layer_set_text_alignment(day_layer, GTextAlignmentRight);
  } else {
    day_layer = text_layer_create(GRect(0, 34, bounds.size.w, 34));
    text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
  }
  battery_layer = layer_create(GRect(0, (bounds.size.h - 4), bounds.size.w, 4));
  // ----< fin da declaración dinámica de capas

  // configuramos a cor de fondo e de texto para hora, dia da semana e día do mes:
  if (!config_get(INVERTIR_CORES)) {
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

  // engadimos a 1ª capa a nosa ventana
  text_layer_set_font(time_layer, time_font);
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  // engadimos as capas á ventana en función dos axustes:
  if (config_get(MOSTRAR_DATA)) {
    text_layer_set_font(date_layer, date_font); 
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  }
  if (config_get(MOSTRAR_DIA)) {
    text_layer_set_font(day_layer, day_font); 
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_layer));
  }
  if (config_get(MOSTRAR_CARGA)) {
    layer_set_update_proc(battery_layer, battery_update_proc);
    layer_add_child(window_get_root_layer(window), battery_layer);
    layer_mark_dirty(battery_layer);
  }
  // mostramos os punteiros das seguintes estructuras:
  LOG("Ventana principal:   %p", window_layer);
  LOG("Capa de hora:    %p", time_layer); 
  LOG("Capa do día:    %p", day_layer);
  LOG("Capa da data:    %p", time_layer);
  LOG("Capa de bateria:    %p", battery_layer); 
}

static void unload(Window *window) {
  if(!saida_sucia) { // evita deinicializar por duplicado
  LOG("------ SAIDA DA APP ------");
  text_layer_destroy(time_layer);
  text_layer_destroy(date_layer);  
  text_layer_destroy(day_layer); 
  layer_destroy(battery_layer);
  fonts_unload_custom_font(time_font);
  fonts_unload_custom_font(date_font);
  fonts_unload_custom_font(day_font);
  LOG("destruindo ventana");
  window_destroy(watchface);
  } // non sair dúas veces seguidas:
  saida_sucia = true;
}

void main_window_push() {
  watchface = window_create();
  if (!config_get(INVERTIR_CORES)) { 
    window_set_background_color(watchface, GColorBlack); 
  } else { 
    window_set_background_color(watchface, GColorWhite); 
  }

  window_set_window_handlers(watchface, (WindowHandlers) {
     .load = load, 
     .unload = unload 
   });
  window_stack_push(watchface, false);
  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);
  time_callback(tm_struct, (TimeUnits) NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, time_callback);
  if (config_get(MOSTRAR_CARGA)) {
    battery_callback(battery_state_service_peek());
    battery_state_service_subscribe(battery_callback);
  }
}