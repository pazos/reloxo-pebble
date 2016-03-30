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
  DEBUG("evento-> bateria-> obtendo porcentaxe");
  porcentaxe_bateria = state.charge_percent;
}

static void eventos_tempo(struct tm *tempo, TimeUnits units_changed) {
  DEBUG("evento-> tempo");
  int mes = tempo->tm_mon;
  int dia = tempo->tm_wday;
  DEBUG("-------> compoñendo cadenas de texto");
  strftime(texto_data, 10, "%e ", tempo);
  strcat(texto_data, nomes_mes[mes]);
  strcpy(texto_dia, nomes_dia[dia]);
  if (clock_is_24h_style()) { formato_tempo = "%R"; } else { formato_tempo = "%I:%M"; }
  strftime(texto_hora, sizeof(texto_hora), formato_tempo, tempo);  
  if (!clock_is_24h_style() && (texto_hora[0] == '0')) { memmove(texto_hora, &texto_hora[1], sizeof(texto_hora) - 1); }
  DEBUG("         [%s] %s, %s", texto_hora, texto_dia, texto_data);
  text_layer_set_text(capa_hora, texto_hora);
  text_layer_set_text(capa_data, texto_data);
  text_layer_set_text(capa_dia, texto_dia);
}

static bool condicion[AXUSTES];
static void mensaxeria(DictionaryIterator *iter, void *context) {
  DEBUG("evento-> mensaxeria");
  // obtemos da bandexa de entrada o valor de verdade para as condicións
  // DIA, DATA, CARGA e INVERTIR
  DEBUG("-------> analizando o diccionario");
  Tuple *t_dia = dict_find(iter, DIA);
  if (t_dia) { condicion[DIA] = t_dia->value->int8 == 1; }
  Tuple *t_data = dict_find(iter, DATA);
  if (t_data) { condicion[DATA] = t_data->value->int8 == 1; }
  Tuple *t_carga = dict_find(iter, CARGA);
  if (t_carga) { condicion[CARGA] = t_carga->value->int8 ==1; }
  Tuple *t_invertir = dict_find(iter, INVERTIR);
  if (t_invertir) { condicion[INVERTIR] = t_invertir->value->int8 ==1; }

  DEBUG("valores> dia: %d, data: %d, carga: %d, invertir: %d", condicion[DIA],
         condicion[DATA], condicion[CARGA], condicion[INVERTIR]);
  
  // gardamos as condicións V/F no almacenamento permanente.
  for (int i = 0; i < AXUSTES; i++) { persist_write_bool(i, condicion[i]); }


  // TODO 
  // obtemos da bandea de entrada o valor enteiro para as variables
  // FORMATO_DATA, FORMATO_DIA

  // gardamos os enteiros no almacenamento permanente


  // obtemos da bandexa de entrada as cadeas para as variables
  // FONTE_HORA, FONTE_DATA, FONTE_DIA, FONDO_PANTALLA

  // gardamos as cadeas no almacenamento permanente
  DEBUG("configuracion gardada, aplicando.");
  window_stack_pop(esfera);
  iniciar_esfera();
}

void iniciar_eventos(){
  app_message_register_inbox_received(mensaxeria);
  app_message_open(38, 38); // engade 176B de memoria non liberable...
  time_t now = time(NULL);
  struct tm *tm_struct = localtime(&now);
  eventos_tempo(tm_struct, (TimeUnits) NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, eventos_tempo); // engade 24B de memoria non liberable
  eventos_bateria(battery_state_service_peek());
  battery_state_service_subscribe(eventos_bateria);
}