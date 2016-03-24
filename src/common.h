#include <pebble.h>
#include <stdio.h>
#include <time.h>

// algúns macros úties:
#define font_load(resource) fonts_load_custom_font(resource_get_handle(resource))
#define rexistro(...) app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)

// nomes en galego para dias da semana e meses:
static char *nomes_dia[] = { "domingo", "luns", "martes", "mércores", "xoves", "venres", "sábado" };
static char *nomes_mes[] = { "de xaneiro", "de febreiro", "de marzo", "de abril", "de maio", "de xuño", "de xullo",
                               "de agosto", "de setembro", "de outubro", "de novembro", "de decembro" };

// interface de usuario. (ventana principal capas e fontes)
static Window *watchface;
static Layer *battery_layer;
static GFont time_font, date_font, day_font;
static TextLayer *time_layer, *date_layer, *day_layer;

// Cadenas de texto e configuración das cores e a batería
static int color = 1;
static int battery_level; 
static char *time_format;
static char time_text[8], day_text[10], date_text[18], new_date_text[18];

static void battery_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  int width = (int)(int)(((int)battery_level * 144) / 100);
  if (color == 0) {
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
  strftime(new_date_text, sizeof(date_text), "%e ", tick_time);
  strcat(new_date_text, nomes_mes[mes]);
  if (strncmp(new_date_text, date_text, sizeof(date_text)) != 0) {
    int dia = tick_time->tm_wday;
    strcpy(day_text, nomes_dia[dia]);
    strncpy(date_text, new_date_text, sizeof(date_text));
    text_layer_set_text(day_layer, day_text);
    text_layer_set_text(date_layer, date_text);
  }
  if (clock_is_24h_style()) { time_format = "%R"; } else { time_format = "%I:%M"; }
  strftime(time_text, sizeof(time_text), time_format, tick_time);

  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  text_layer_set_text(time_layer, time_text);
}