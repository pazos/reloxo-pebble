#include "eventos.h"

extern Window *esfera;
extern TextLayer *capa_hora, *capa_dia, *capa_data;
static char texto_hora[8], texto_dia[10], texto_data[18];
static char *formato_tempo;
static char *nomes_dia[] = { "domingo", "luns", "martes", "mércores", "xoves", "venres", "sábado" };
static char *nomes_mes[] = { "de xaneiro", "de febreiro", "de marzo", "de abril", "de maio", "de xuño",
                  "de xullo", "de agosto", "de setembro", "de outubro", "de novembro", "de decembro" };

int porcentaxe_bateria;
static void eventos_bateria(BatteryChargeState state){ 
  porcentaxe_bateria = state.charge_percent;
}

static void eventos_tempo(struct tm *tempo, TimeUnits units_changed) {
  int mes = tempo->tm_mon;
  int dia = tempo->tm_wday;
  strftime(texto_data, 10, "%e ", tempo);
  strcat(texto_data, nomes_mes[mes]);
  strcpy(texto_dia, nomes_dia[dia]);
  if (clock_is_24h_style()) { formato_tempo = "%R"; } else { formato_tempo = "%I:%M"; }
  strftime(texto_hora, sizeof(texto_hora), formato_tempo, tempo);  
  if (!clock_is_24h_style() && (texto_hora[0] == '0')) { memmove(texto_hora, &texto_hora[1], sizeof(texto_hora) - 1); }
  text_layer_set_text(capa_hora, texto_hora);
  text_layer_set_text(capa_data, texto_data);
  text_layer_set_text(capa_dia, texto_dia);
}

void mensaxeria(DictionaryIterator *iter, void *context) {
  Tuple *t = dict_read_first(iter);
  while(t) {
    persist_write_bool(t->key, strcmp(t->value->cstring, "true") == 0 ? true : false);
    t = dict_read_next(iter);
  } 
    LOG("configuracion gardada, aplicando.");
    window_stack_pop(esfera);
    iniciar_esfera();
}

void iniciar_eventos(){
  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);
  eventos_tempo(tm_struct, (TimeUnits) NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, eventos_tempo);
  eventos_bateria(battery_state_service_peek());
  battery_state_service_subscribe(eventos_bateria);
}